/*
 * * Copyright (C) 2009-2011 Ali <aliov@xfce.org>
 * * Copyright (C) 2012-2017 Simon Steinbeiß <ochosi@xfce.org>
 * * Copyright (C) 2012-2020 Sean Davis <bluesabre@xfce.org>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <glib.h>
#include <gtk/gtk.h>
#include <gio/gio.h>
#include <libxapp/xapp-status-icon.h>

#include "src/misc/respite.h"

#include "src/plugins/tray/tray-provider.h"

static void   tray_provider_iface_init(RespiteProviderPluginIface *iface);
static void   tray_provider_finalize(GObject                   *object);


struct _TrayProviderClass {
    GObjectClass parent_class;
};

struct _TrayProvider {
    GObject                 parent;
    RespiteProviderPlayer   *player;
    XAppStatusIcon         *tray;
    GtkWidget              *window;
    gulong                  sig;

    RespiteState             state;
    GtkWidget              *menu;
};

RESPITE_DEFINE_TYPE_WITH_CODE(TrayProvider,
                                tray_provider,
                                G_TYPE_OBJECT,
                                RESPITE_IMPLEMENT_INTERFACE(RESPITE_TYPE_PROVIDER_PLUGIN,
                                tray_provider_iface_init));

static void
exit_activated_cb(TrayProvider *tray) {
    GdkEventAny ev;

    g_clear_pointer(&tray->menu, gtk_widget_destroy);

    ev.type = GDK_DELETE;
    ev.window = gtk_widget_get_window(tray->window);
    ev.send_event = TRUE;

    g_signal_handler_disconnect(tray->window, tray->sig);
    gtk_main_do_event((GdkEvent *) &ev);
}

static void
toggle_pause(TrayProvider *tray) {
    if ( tray->state == PAROLE_STATE_PLAYING )
        respite_provider_player_pause(tray->player);
    else if ( tray->state == PAROLE_STATE_PAUSED )
        respite_provider_player_resume(tray->player);
}

static void
play_pause_activated_cb(TrayProvider *tray) {
    g_clear_pointer(&tray->menu, gtk_widget_destroy);
    toggle_pause(tray);
}

static void
previous_activated_cb(TrayProvider *tray) {
    g_clear_pointer(&tray->menu, gtk_widget_destroy);
    respite_provider_player_play_previous(tray->player);
}

static void
next_activated_cb(TrayProvider *tray) {
    g_clear_pointer(&tray->menu, gtk_widget_destroy);
    respite_provider_player_play_next(tray->player);
}

static void
open_activated_cb(TrayProvider *tray) {
    respite_provider_player_open_media_chooser(tray->player);
}

static GtkWidget *
build_tray_menu(TrayProvider *tray) {
    GtkWidget *menu, *mi;

    menu = gtk_menu_new();

    mi = gtk_menu_item_new_with_mnemonic(tray->state == PAROLE_STATE_PLAYING ? _("_Pause") : _("_Play"));
    gtk_widget_set_sensitive(mi, TRUE);
    gtk_widget_show(mi);
    g_signal_connect_swapped(mi, "activate", G_CALLBACK(play_pause_activated_cb), tray);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi);

    mi = gtk_menu_item_new_with_mnemonic(_("P_revious Track"));
    gtk_widget_set_sensitive(mi, TRUE);
    gtk_widget_show(mi);
    g_signal_connect_swapped(mi, "activate", G_CALLBACK(previous_activated_cb), tray);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi);

    mi = gtk_menu_item_new_with_mnemonic(_("_Next Track"));
    gtk_widget_set_sensitive(mi, TRUE);
    gtk_widget_show(mi);
    g_signal_connect_swapped(mi, "activate", G_CALLBACK(next_activated_cb), tray);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi);

    mi = gtk_separator_menu_item_new();
    gtk_widget_show(mi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi);

    mi = gtk_menu_item_new_with_mnemonic(_("_Open"));
    gtk_widget_show(mi);
    g_signal_connect_swapped(mi, "activate", G_CALLBACK(open_activated_cb), tray);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi);

    mi = gtk_separator_menu_item_new();
    gtk_widget_show(mi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi);

    mi = gtk_menu_item_new_with_mnemonic(_("_Quit"));
    gtk_widget_set_sensitive(mi, TRUE);
    gtk_widget_show(mi);
    g_signal_connect_swapped(mi, "activate", G_CALLBACK(exit_activated_cb), tray);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), mi);

    return menu;
}

static void
tray_activate_cb(XAppStatusIcon *icon, TrayProvider *tray) {
    if (!gtk_widget_get_visible(tray->window) || !gtk_window_is_active(GTK_WINDOW(tray->window)))
        gtk_window_present(GTK_WINDOW(tray->window));
    else
        gtk_widget_hide(tray->window);
}

static void
state_changed_cb(RespiteProviderPlayer *player, const RespiteStream *stream, RespiteState state, TrayProvider *tray) {
    tray->state = state;

    if ( tray->menu ) {
        g_clear_pointer(&tray->menu, gtk_widget_destroy);
        tray->menu = build_tray_menu(tray);
        xapp_status_icon_set_primary_menu(tray->tray, GTK_MENU(tray->menu));
    }
}

static void
icon_theme_changed_cb (GtkIconTheme *icon_theme, TrayProvider *tray) {
    xapp_status_icon_set_icon_name(tray->tray, "org.gnome.Respite");
}

static gboolean
read_entry_bool(const gchar *entry, gboolean fallback) {
    GSettings *settings;
    gboolean ret_val;

    settings = g_settings_new("org.gnome.Respite.Plugins");
    ret_val = g_settings_get_boolean(settings, entry);

    g_object_unref(settings);

    return ret_val;
}

static void
write_entry_bool(const gchar *entry, gboolean value) {
    GSettings *settings;

    settings = g_settings_new("org.gnome.Respite.Plugins");
    g_settings_set_boolean(settings, entry, value);

    g_object_unref(settings);
}

static void
hide_on_delete_toggled_cb(GtkWidget *widget, gpointer tray) {
    gboolean toggled;
    toggled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
    write_entry_bool("minimize-to-tray", toggled);
}

static void
configure_plugin(TrayProvider *tray, GtkWidget *widget) {
    GtkWidget *dialog;
    GtkWidget *content_area;

    GtkWidget *hide_on_delete;
    gboolean hide_on_delete_b;

    dialog = gtk_dialog_new_with_buttons(_("Tray icon plugin"),
                                            GTK_WINDOW(widget),
                                            GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                            _("Close"),
                                            GTK_RESPONSE_CANCEL,
                                            NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    hide_on_delete_b = read_entry_bool("minimize-to-tray", TRUE);
    hide_on_delete = gtk_check_button_new_with_label(_("Always minimize to tray when window is closed"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(hide_on_delete), hide_on_delete_b);

    g_signal_connect(hide_on_delete, "toggled",
              G_CALLBACK(hide_on_delete_toggled_cb), NULL);

    gtk_box_pack_start(GTK_BOX(content_area), hide_on_delete, TRUE, TRUE, 0);

    g_signal_connect(dialog, "response",
              G_CALLBACK(gtk_widget_destroy), NULL);

    gtk_widget_show_all(dialog);
}

static void
action_on_hide_confirmed_cb(GtkWidget *widget, gpointer data) {
    gboolean toggled;

    toggled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));

    write_entry_bool("remember-quit-action", toggled);
}

static gboolean
button_press_event_cb(XAppStatusIcon *icon, GdkEventButton *event, TrayProvider *tray) {
    if (event->button == GDK_BUTTON_MIDDLE)
      toggle_pause(tray);

    return FALSE;
}

static gboolean
scroll_event_cb(XAppStatusIcon *icon, GdkEventScroll *event, TrayProvider *tray) {
    switch (event->direction) {
        case GDK_SCROLL_DOWN:
        case GDK_SCROLL_LEFT:
            respite_provider_player_volume_down(tray->player);
            break;
        case GDK_SCROLL_UP:
        case GDK_SCROLL_RIGHT:
            respite_provider_player_volume_up(tray->player);
            break;
        default:
            break;
    }
    return FALSE;
}

static gboolean
delete_event_cb(GtkWidget *widget, GdkEvent *ev, TrayProvider *tray) {
    GtkWidget *dialog, *check, *content_area, *button;
    GtkWidget *minimize, *img;
    gboolean confirmed, ret_val = TRUE, minimize_to_tray;

    confirmed = read_entry_bool("remember-quit-action", FALSE);
    minimize_to_tray = read_entry_bool("minimize-to-tray", TRUE);

    if ( confirmed ) {
        return minimize_to_tray ? gtk_widget_hide_on_delete (widget) : FALSE;
    }

    dialog = gtk_message_dialog_new(GTK_WINDOW(widget),
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_NONE,
                                    NULL);

    gchar *markup = g_strdup_printf("<big><b>%s</b></big>", _("Are you sure you want to quit?"));
    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(dialog), markup);
    g_free(markup);

    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
            _("Respite can be minimized to the system tray instead."));

    minimize = gtk_dialog_add_button(GTK_DIALOG(dialog),
                                     _("Minimize to tray"),
                                     GTK_RESPONSE_OK);
    img = gtk_image_new_from_icon_name("go-down-symbolic", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(minimize), img);

    button = gtk_dialog_add_button(GTK_DIALOG(dialog),
                                   _("Cancel"),
                                   GTK_RESPONSE_CANCEL);
    img = gtk_image_new_from_icon_name("process-stop-symbolic", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(button), img);

    button = gtk_dialog_add_button(GTK_DIALOG(dialog),
                                   _("Quit"),
                                   GTK_RESPONSE_CLOSE);
    img = gtk_image_new_from_icon_name("application-exit-symbolic", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(button), img);

    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    check = gtk_check_button_new_with_mnemonic(_("Remember my choice"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check), FALSE);

    g_signal_connect(check, "toggled",
              G_CALLBACK(action_on_hide_confirmed_cb), NULL);

    gtk_box_pack_start(GTK_BOX(content_area), check, TRUE, TRUE, 0);

    gtk_widget_set_margin_start(GTK_WIDGET(check), 3);

    gtk_widget_show_all(GTK_WIDGET(dialog));

    switch (gtk_dialog_run(GTK_DIALOG(dialog))) {
        case GTK_RESPONSE_OK:
            {
            gtk_widget_hide_on_delete(widget);
            confirmed = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check));
            if ( confirmed )
                write_entry_bool("minimize-to-tray", TRUE);
            break;
            }
        case GTK_RESPONSE_CLOSE:
            {
            confirmed = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check));
            if ( confirmed )
                write_entry_bool("minimize-to-tray", FALSE);
            ret_val = FALSE;
            }
            break;
        case GTK_RESPONSE_CANCEL:
            break;
        default:
            break;
    }

    gtk_widget_destroy(dialog);
    return ret_val;
}

static gboolean tray_provider_is_configurable(RespiteProviderPlugin *plugin) {
    return TRUE;
}

static void
tray_provider_set_player(RespiteProviderPlugin *plugin, RespiteProviderPlayer *player) {
    TrayProvider *tray;

    tray = TRAY_PROVIDER(plugin);

    tray->player = player;

    tray->state = PAROLE_STATE_STOPPED;

    tray->window = respite_provider_player_get_main_window(player);

    tray->tray = xapp_status_icon_new_with_name("respite");
    xapp_status_icon_set_icon_name(tray->tray, "org.gnome.Respite");
    xapp_status_icon_set_tooltip_text(tray->tray, _("Respite Media Player"));

    tray->menu = build_tray_menu(tray);
    xapp_status_icon_set_primary_menu(tray->tray, GTK_MENU(tray->menu));

    g_signal_connect(tray->tray, "activate",
              G_CALLBACK(tray_activate_cb), tray);

    g_signal_connect(tray->tray, "button-press-event",
              G_CALLBACK(button_press_event_cb), tray);

    g_signal_connect(tray->tray, "scroll-event",
              G_CALLBACK(scroll_event_cb), tray);

    tray->sig = g_signal_connect_object(tray->window, "delete-event",
              G_CALLBACK(delete_event_cb), tray, 0);

    g_signal_connect(player, "state-changed",
              G_CALLBACK(state_changed_cb), tray);

    g_signal_connect_object(gtk_icon_theme_get_default(), "changed",
              G_CALLBACK(icon_theme_changed_cb), tray, 0);
}

static void
tray_provider_configure(RespiteProviderPlugin *provider, GtkWidget *parent) {
    TrayProvider *tray;
    tray = TRAY_PROVIDER(provider);
    configure_plugin(tray, parent);
}

static void
tray_provider_iface_init(RespiteProviderPluginIface *iface) {
    iface->set_player = tray_provider_set_player;
    iface->configure = tray_provider_configure;
    iface->get_is_configurable = tray_provider_is_configurable;
}

static void tray_provider_class_init(TrayProviderClass *klass) {
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

    gobject_class->finalize = tray_provider_finalize;
}

static void tray_provider_init(TrayProvider *provider) {
    provider->player = NULL;
}

static void tray_provider_finalize(GObject *object) {
    TrayProvider *tray;

    tray = TRAY_PROVIDER(object);

    g_clear_pointer(&tray->menu, gtk_widget_destroy);
    g_clear_object(&tray->tray);

    G_OBJECT_CLASS(tray_provider_parent_class)->finalize(object);
}
