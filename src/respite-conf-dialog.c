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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib/gi18n.h>

#include "src/gst/respite-gst.h"

#include "src/misc/respite.h"

#include "src/respite-builder.h"
#include "src/respite-conf.h"
#include "src/respite-conf-dialog.h"
#include "src/respite-subtitle-encoding.h"
#include "src/respite-vis.h"

/*
 * GtkBuilder Callbacks
 */

void        respite_conf_dialog_response_cb(GtkDialog *dialog,
                                                     gint response_id,
                                                     RespiteConfDialog *self);

void
respite_conf_dialog_vis_plugin_changed_cb(GtkComboBox *widget,
                                                     RespiteConfDialog *self);

void
respite_conf_dialog_sink_plugin_changed_cb(GtkComboBox *widget,
                                                     RespiteConfDialog *self);

void        respite_conf_dialog_font_set_cb(GtkFontButton *button,
                                                     RespiteConfDialog *self);

void respite_conf_dialog_enable_vis_changed_cb(GObject *object,
                                                     GParamSpec *pspec,
                                                     gpointer *data);

void
respite_conf_dialog_subtitle_encoding_changed_cb(GtkComboBox *widget,
                                                     RespiteConfDialog *self);

void        brightness_value_changed_cb(GtkRange *range,
                                                     RespiteConfDialog *self);

void        contrast_value_changed_cb(GtkRange *range,
                                                     RespiteConfDialog *self);

void        hue_value_changed_cb(GtkRange *range,
                                                     RespiteConfDialog *self);

void        saturation_value_changed_cb(GtkRange *range,
                                                     RespiteConfDialog *self);

void        reset_color_clicked_cb(GtkButton *button,
                                                     RespiteConfDialog *self);

/*
 * End of GtkBuilder callbacks
 */

struct RespiteConfDialogPrivate {
    RespiteConf *conf;

    GHashTable *vis_plugins;

    GtkWidget  *vis_combox;
    GtkWidget  *sink_combox;
    GtkWidget  *backend_infobar;
    GtkWidget  *toggle_vis;
    GtkWidget  *toggle_subtitle;
    GtkWidget  *font_button;
    GtkWidget  *encoding;
    GtkWidget  *brightness;
    GtkWidget  *contrast;
    GtkWidget  *hue;
    GtkWidget  *saturation;
};

G_DEFINE_TYPE_WITH_PRIVATE(RespiteConfDialog, respite_conf_dialog, G_TYPE_OBJECT)

/* Destroy the dialog */
static void
respite_conf_dialog_destroy(GtkWidget *widget, RespiteConfDialog *self) {
    gtk_widget_hide(widget);
}

/* Change the various image properties */
void reset_color_clicked_cb(GtkButton *button, RespiteConfDialog *self) {
    gtk_range_set_value(GTK_RANGE(self->priv->brightness), 0);
    gtk_range_set_value(GTK_RANGE(self->priv->contrast), 0);
    gtk_range_set_value(GTK_RANGE(self->priv->hue), 0);
    gtk_range_set_value(GTK_RANGE(self->priv->saturation), 0);
}

void respite_conf_dialog_response_cb(GtkDialog *dialog, gint response_id, RespiteConfDialog *self) {
    switch (response_id) {
        case GTK_RESPONSE_HELP:
            break;
        default:
            respite_conf_dialog_destroy(GTK_WIDGET(dialog), self);
            break;
    }
}

/* Change subtitle encoding */
void respite_conf_dialog_subtitle_encoding_changed_cb(GtkComboBox *widget, RespiteConfDialog *self) {
    g_object_set(G_OBJECT(self->priv->conf),
                 "subtitle-encoding", respite_subtitle_encoding_get_selected(widget),
                 NULL);
}

/* Enable visualizations */
void respite_conf_dialog_enable_vis_changed_cb(GObject *object, GParamSpec *pspec, gpointer *data) {
    gboolean active;
    RespiteConfDialog *self;
    self = RESPITE_CONF_DIALOG(data);

    active = gtk_switch_get_active(GTK_SWITCH(GTK_WIDGET(object)));

    gtk_widget_set_sensitive(self->priv->vis_combox, active);
}

/* Generic function to change all image properties */
static void
set_effect_value(RespiteConfDialog *self, GtkRange *range, const gchar *name) {
    gint value;

    value = gtk_range_get_value(range);

    g_object_set(G_OBJECT(self->priv->conf),
                  name, value,
                  NULL);
}

/* Change brightness */
void brightness_value_changed_cb(GtkRange *range, RespiteConfDialog *self) {
    set_effect_value(self, range, "brightness");
}

/* Change contrast */
void contrast_value_changed_cb(GtkRange *range, RespiteConfDialog *self) {
    set_effect_value(self, range, "contrast");
}

/* Change hue */
void hue_value_changed_cb(GtkRange *range, RespiteConfDialog *self) {
    set_effect_value(self, range, "hue");
}

/* Change saturation */
void saturation_value_changed_cb(GtkRange *range, RespiteConfDialog *self) {
    set_effect_value(self, range, "saturation");
}

void respite_conf_dialog_vis_plugin_changed_cb(GtkComboBox *widget,  RespiteConfDialog *self) {
    gchar *active = NULL;
    GstElementFactory *f;

    GtkTreeIter iter;
    GtkTreeModel *model = gtk_combo_box_get_model(widget);

    if (gtk_combo_box_get_active_iter (widget, &iter))
        gtk_tree_model_get(model, &iter, 0, &active, -1);
    else
        return;

    f = g_hash_table_lookup(self->priv->vis_plugins, active);

    if (f) {
        g_object_set(G_OBJECT(self->priv->conf),
                      "vis-name", gst_object_get_name(GST_OBJECT(f)),
                      NULL);
    }

    g_free(active);
}

void respite_conf_dialog_sink_plugin_changed_cb(GtkComboBox *widget,  RespiteConfDialog *self) {
    gchar *active = NULL;

    GtkTreeIter iter;
    GtkTreeModel *model = gtk_combo_box_get_model(widget);

    if (gtk_combo_box_get_active_iter (widget, &iter))
        gtk_tree_model_get(model, &iter, 0, &active, -1);
    else
        return;

    if (g_strcmp0(active, "other") != 0) {
        g_object_set(G_OBJECT(self->priv->conf),
                      "videosink", g_strdup(active),
                      NULL);
    }

    gtk_widget_show(self->priv->backend_infobar);

    g_free(active);
}

/* Change playlist position */
void respite_conf_dialog_playlist_position_changed_cb(GtkComboBox *widget, RespiteConfDialog *self) {
    gchar *active_id = gtk_combo_box_get_active_id(widget);
    if (active_id) {
        g_object_set(G_OBJECT(self->priv->conf),
                     "playlist-position", active_id,
                     NULL);
    }
}

/* Change subtitle font */
void respite_conf_dialog_font_set_cb(GtkFontButton *button, RespiteConfDialog *self) {
    G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    g_object_set(G_OBJECT(self->priv->conf),
                 "subtitle-font", gtk_font_button_get_font_name(button),
                 NULL);
    G_GNUC_END_IGNORE_DEPRECATIONS
}

/* Finalize the dialog */
static void
respite_conf_dialog_finalize(GObject *object) {
    RespiteConfDialog *self;

    self = RESPITE_CONF_DIALOG(object);

    g_object_unref(self->priv->conf);
    g_hash_table_destroy(self->priv->vis_plugins);

    G_OBJECT_CLASS(respite_conf_dialog_parent_class)->finalize(object);
}

/* Initialize the config-dialog class */
static void
respite_conf_dialog_class_init(RespiteConfDialogClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = respite_conf_dialog_finalize;
}

/* Initialize the dialog */
static void
respite_conf_dialog_init(RespiteConfDialog *self) {
    self->priv = respite_conf_dialog_get_instance_private(self);
    self->priv->conf = respite_conf_new();

    self->priv->vis_plugins = respite_vis_get_plugins();
}

/* Fill the combobox with visualizations */
static void
respite_conf_dialog_add_vis_plugins(gpointer key, gpointer value, GtkWidget *combox) {
    GtkListStore *store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(combox)));
    GtkTreeIter iter;

    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, 0, (const gchar *) key, -1);

    g_object_unref(store);
}

/* Set the combobox to the default visualization plugin */
static gboolean
respite_conf_dialog_set_default_vis_plugin(GtkTreeModel *model,
                                          GtkTreePath *path,
                                          GtkTreeIter *iter,
                                          RespiteConfDialog *self) {
    GstElementFactory *f;
    gchar *vis_name;
    gchar *combox_text;
    gboolean ret = FALSE;

    g_object_get(G_OBJECT(self->priv->conf),
                  "vis-name", &vis_name,
                  NULL);

    gtk_tree_model_get(model, iter,
                        0, &combox_text,
                        -1);

    f = g_hash_table_lookup(self->priv->vis_plugins, combox_text);

    if ( g_strcmp0(vis_name, "none") == 0 ) {
        if ( g_strcmp0 (gst_object_get_name (GST_OBJECT (f)), "Goom") == 0 )
            ret = TRUE;
    } else if ( g_strcmp0(gst_object_get_name(GST_OBJECT(f)), vis_name) == 0 ) {
        ret = TRUE;
    }

    if ( ret == TRUE )
        gtk_combo_box_set_active_iter(GTK_COMBO_BOX(self->priv->vis_combox), iter);

    return ret;
}

/* Set the combobox to the default sink plugin */
static gboolean
respite_conf_dialog_set_default_sink_plugin(RespiteConfDialog *self) {
    gchar *sink_name;

    g_object_get(G_OBJECT(self->priv->conf),
                  "videosink", &sink_name,
                  NULL);

    if (g_strcmp0(sink_name, "autoimagesink") == 0) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(self->priv->sink_combox), 0);
        return TRUE;
    } else if (g_strcmp0(sink_name, "xvimagesink") == 0) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(self->priv->sink_combox), 1);
        return TRUE;
    } else if (g_strcmp0(sink_name, "ximagesink") == 0) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(self->priv->sink_combox), 2);
        return TRUE;
    } else {
        gtk_combo_box_set_active(GTK_COMBO_BOX(self->priv->sink_combox), 0);
        return FALSE;
    }
}

/* Load the rest of the settings stored in the rc file */
static void
respite_conf_dialog_set_defaults(RespiteConfDialog *self) {
    GtkTreeModel *model;
    gboolean vis_enabled;
    gchar *subtitle_font;
    gchar *subtitle_encoding;

    g_object_get(G_OBJECT(self->priv->conf),
                  "vis-enabled", &vis_enabled,
                  "subtitle-font", &subtitle_font,
                  "subtitle-encoding", &subtitle_encoding,
                  NULL);

    /* Update widget-states according to settings */
    gtk_widget_set_sensitive(self->priv->vis_combox, vis_enabled);

    model = gtk_combo_box_get_model(GTK_COMBO_BOX(self->priv->vis_combox));

    gtk_tree_model_foreach(model,
                            (GtkTreeModelForeachFunc) respite_conf_dialog_set_default_vis_plugin,
                            self);

    respite_conf_dialog_set_default_sink_plugin(self);

    respite_subtitle_encoding_set(GTK_COMBO_BOX(self->priv->encoding), subtitle_encoding);

    G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    gtk_font_button_set_font_name(GTK_FONT_BUTTON(self->priv->font_button), subtitle_font);
    G_GNUC_END_IGNORE_DEPRECATIONS

    g_free(subtitle_font);
    g_free(subtitle_encoding);
}

RespiteConfDialog *
respite_conf_dialog_new(void) {
    RespiteConfDialog *respite_conf_dialog = NULL;
    respite_conf_dialog = g_object_new(RESPITE_TYPE_CONF_DIALOG, NULL);
    return respite_conf_dialog;
}

void respite_conf_dialog_open(RespiteConfDialog *self, GtkWidget *parent) {
    GtkBuilder *builder;
    GtkWidget  *dialog;
    GtkWidget  *combox;
    gboolean    with_display;

    GtkWidget *switch_widget;
    GtkAdjustment *adjustment;
    GValue step_increment = {0};

    builder = respite_builder_new_from_resource("/org/gnome/respite/respite-settings.ui");

    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "settings-dialog"));
    combox = GTK_WIDGET(gtk_builder_get_object(builder, "vis-combobox"));

    self->priv->toggle_vis = GTK_WIDGET(gtk_builder_get_object(builder, "enable-vis"));
    self->priv->toggle_subtitle = GTK_WIDGET(gtk_builder_get_object(builder, "enable-subtitle"));
    self->priv->font_button = GTK_WIDGET(gtk_builder_get_object(builder, "fontbutton"));
    self->priv->encoding = GTK_WIDGET(gtk_builder_get_object(builder, "encoding"));

    respite_subtitle_encoding_init(GTK_COMBO_BOX(self->priv->encoding));

    g_hash_table_foreach(self->priv->vis_plugins, (GHFunc)respite_conf_dialog_add_vis_plugins, combox);

    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent));

    self->priv->vis_combox = combox;
    self->priv->sink_combox = GTK_WIDGET(gtk_builder_get_object(builder, "combobox-sink"));
    self->priv->backend_infobar = GTK_WIDGET(gtk_builder_get_object(builder, "backend-infobar"));

    respite_conf_dialog_set_defaults(self);

    with_display = respite_gst_get_is_xvimage_sink(RESPITE_GST(respite_gst_get()));

    if ( !with_display ) {
        gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(builder, "frame-display")), FALSE);
    } else {
        gint brightness_value;
        gint contrast_value;
        gint hue_value;
        gint saturation_value;

        self->priv->brightness = GTK_WIDGET(gtk_builder_get_object(builder, "brightness"));
        self->priv->contrast = GTK_WIDGET(gtk_builder_get_object(builder, "contrast"));
        self->priv->hue = GTK_WIDGET(gtk_builder_get_object(builder, "hue"));
        self->priv->saturation = GTK_WIDGET(gtk_builder_get_object(builder, "saturation"));

        gtk_range_set_range(GTK_RANGE(self->priv->brightness), -1000, 1000);
        gtk_range_set_range(GTK_RANGE(self->priv->contrast), -1000, 1000);
        gtk_range_set_range(GTK_RANGE(self->priv->saturation), -1000, 1000);
        gtk_range_set_range(GTK_RANGE(self->priv->hue), -1000, 1000);
        gtk_scale_add_mark(GTK_SCALE(self->priv->brightness), 0, GTK_POS_BOTTOM, NULL);
        gtk_scale_add_mark(GTK_SCALE(self->priv->contrast), 0, GTK_POS_BOTTOM, NULL);
        gtk_scale_add_mark(GTK_SCALE(self->priv->saturation), 0, GTK_POS_BOTTOM, NULL);
        gtk_scale_add_mark(GTK_SCALE(self->priv->hue), 0, GTK_POS_BOTTOM, NULL);

        /* Set the step increment on the sliders to make them keyboard friendly */
        g_value_init(&step_increment, G_TYPE_DOUBLE);
        g_value_set_double(&step_increment, 10.0);
        adjustment = gtk_range_get_adjustment(GTK_RANGE(self->priv->brightness));
        g_object_set_property(G_OBJECT(adjustment), "step-increment", &step_increment);
        adjustment = gtk_range_get_adjustment(GTK_RANGE(self->priv->contrast));
        g_object_set_property(G_OBJECT(adjustment), "step-increment", &step_increment);
        adjustment = gtk_range_get_adjustment(GTK_RANGE(self->priv->hue));
        g_object_set_property(G_OBJECT(adjustment), "step-increment", &step_increment);
        adjustment = gtk_range_get_adjustment(GTK_RANGE(self->priv->saturation));
        g_object_set_property(G_OBJECT(adjustment), "step-increment", &step_increment);

        g_object_get(G_OBJECT(self->priv->conf),
                      "brightness", &brightness_value,
                      "contrast", &contrast_value,
                      "hue", &hue_value,
                      "saturation", &saturation_value,
                      NULL);

        gtk_range_set_value(GTK_RANGE(self->priv->brightness), brightness_value);
        gtk_range_set_value(GTK_RANGE(self->priv->contrast), contrast_value);
        gtk_range_set_value(GTK_RANGE(self->priv->hue), hue_value);
        gtk_range_set_value(GTK_RANGE(self->priv->saturation), saturation_value);
    }

    /* General/Video/Disable screensaver while playing movies */
    switch_widget = GTK_WIDGET(gtk_builder_get_object(builder, "reset-saver"));
    g_object_bind_property(G_OBJECT(self->priv->conf), "reset-saver",
                           switch_widget, "active",
                           G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL);

    /* General/Audio/Enable visualization while playing audio file */
    switch_widget = GTK_WIDGET(gtk_builder_get_object(builder, "enable-vis"));
    g_object_bind_property(G_OBJECT(self->priv->conf), "vis-enabled",
                           switch_widget, "active",
                           G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL);
    g_signal_connect(G_OBJECT(switch_widget), "notify::active",
                     G_CALLBACK(respite_conf_dialog_enable_vis_changed_cb), self);

    /* General/Keyboard/Enable keyboard multimedia keys */
    switch_widget = GTK_WIDGET(gtk_builder_get_object(builder, "multimedia-keys"));
    g_object_bind_property(G_OBJECT(self->priv->conf), "multimedia-keys",
                           switch_widget, "active",
                           G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL);

    /* Playlist/Always replace playlist with opened files */
    switch_widget = GTK_WIDGET(gtk_builder_get_object(builder, "replace-playlist"));
    g_object_bind_property(G_OBJECT(self->priv->conf), "replace-playlist",
                           switch_widget, "active",
                           G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL);

    /* Playlist/Check and remove duplicate media entries */
    switch_widget = GTK_WIDGET(gtk_builder_get_object(builder, "remove-duplicated"));
    g_object_bind_property(G_OBJECT(self->priv->conf), "remove-duplicated",
                           switch_widget, "active",
                           G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL);

    /* Playlist/Start playing opened files */
    switch_widget = GTK_WIDGET(gtk_builder_get_object(builder, "start-playing-opened"));
    g_object_bind_property(G_OBJECT(self->priv->conf), "play-opened-files",
                           switch_widget, "active",
                           G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL);

    /* Playlist/Remember playlist */
    switch_widget = GTK_WIDGET(gtk_builder_get_object(builder, "remember-playlist"));
    g_object_bind_property(G_OBJECT(self->priv->conf), "remember-playlist",
                           switch_widget, "active",
                           G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL);

    /* Playlist/Playlist position */
    {
        GtkWidget *combo = GTK_WIDGET(gtk_builder_get_object(builder, "playlist-position"));
        gchar *position = NULL;
        g_object_get(G_OBJECT(self->priv->conf), "playlist-position", &position, NULL);
        if (position) {
            gtk_combo_box_set_active_id(GTK_COMBO_BOX(combo), position);
            g_free(position);
        }
        g_signal_connect(G_OBJECT(combo), "changed",
                         G_CALLBACK(respite_conf_dialog_playlist_position_changed_cb), self);
    }

    /* Subtitles/Automatically show subtitles when playing movie file */
    switch_widget = GTK_WIDGET(gtk_builder_get_object(builder, "enable-subtitle"));
    g_object_bind_property(G_OBJECT(self->priv->conf), "enable-subtitle",
                           switch_widget, "active",
                           G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL);

    g_signal_connect(G_OBJECT(dialog), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);

    gtk_builder_connect_signals(builder, self);

    g_object_unref(builder);

    gtk_widget_show(dialog);
}
