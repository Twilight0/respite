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

#include "src/dbus/respite-dbus.h"

#include "src/gst/respite-gst.h"

#include "src/misc/respite-provider-player.h"

#include "src/respite-medialist.h"
#include "src/respite-plugin-player.h"
#include "src/respite-plugins-manager.h"


static void respite_plugin_player_iface_init(RespiteProviderPlayerIface *iface);

static void respite_plugin_player_finalize(GObject *object);

struct RespitePluginPlayerPrivate {
    GtkWidget *gst;
    GtkWidget *box;

    gboolean packed;
    gboolean fullscreen;
};

G_DEFINE_TYPE_WITH_CODE(RespitePluginPlayer, respite_plugin_player, G_TYPE_OBJECT,
                        G_ADD_PRIVATE(RespitePluginPlayer)
                        G_IMPLEMENT_INTERFACE(RESPITE_TYPE_PROVIDER_PLAYER,
                                              respite_plugin_player_iface_init))

static void
respite_plugin_player_send_message(const gchar *message) {
    DBusGProxy *proxy;

    proxy = respite_get_proxy(RESPITE_DBUS_PATH, RESPITE_DBUS_INTERFACE);

    dbus_g_proxy_call_no_reply(proxy, message,
                                G_TYPE_INVALID,
                                G_TYPE_INVALID);

    g_object_unref(proxy);
}

static GtkWidget *
respite_plugin_player_get_main_window(RespiteProviderPlayer *provider) {
    RespitePluginPlayer *player;

    player = RESPITE_PLUGIN_PLAYER(provider);
    if (GTK_IS_WIDGET (player->priv->gst)) {
        return gtk_widget_get_toplevel (player->priv->gst);
    }
    return NULL;
}

static void
respite_plugin_player_pack_widget(RespiteProviderPlayer *provider, GtkWidget *widget,
                                 const gchar *title, ParolePluginContainer container_type) {
    RespitePluginPlayer *player;
    RespitePluginsManager *manager;

    manager = respite_plugins_manager_get();
    player = RESPITE_PLUGIN_PLAYER(provider);

    g_return_if_fail(player->priv->packed == FALSE);

    player->priv->packed = TRUE;
    player->priv->box = widget;

    respite_plugins_manager_pack(manager, widget, title, container_type);
}

static RespiteState
respite_plugin_player_get_state(RespiteProviderPlayer *provider) {
    RespitePluginPlayer *player;

    player = RESPITE_PLUGIN_PLAYER(provider);

    return respite_gst_get_state (RESPITE_GST (player->priv->gst));
}

static const RespiteStream *
respite_plugin_player_get_stream(RespiteProviderPlayer *provider) {
    RespitePluginPlayer *player;

    player = RESPITE_PLUGIN_PLAYER(provider);

    return respite_gst_get_stream (RESPITE_GST (player->priv->gst));
}

static gboolean
respite_plugin_player_play_uri(RespiteProviderPlayer *provider, const gchar *uri) {
    RespiteMediaList *list;

    list = RESPITE_MEDIA_LIST(respite_media_list_get());
    respite_media_list_open_uri(list, uri);

    g_object_unref(list);

    return TRUE;
}

static gboolean
respite_plugin_player_pause(RespiteProviderPlayer *provider) {
    RespitePluginPlayer *player;

    player = RESPITE_PLUGIN_PLAYER(provider);

    respite_gst_pause(RESPITE_GST(player->priv->gst));

    return TRUE;
}

static gboolean
respite_plugin_player_resume(RespiteProviderPlayer *provider) {
    RespitePluginPlayer *player;

    player = RESPITE_PLUGIN_PLAYER(provider);

    respite_gst_resume(RESPITE_GST(player->priv->gst));

    return TRUE;
}

static gboolean
respite_plugin_player_stop(RespiteProviderPlayer *provider) {
    RespitePluginPlayer *player;

    player = RESPITE_PLUGIN_PLAYER(provider);

    respite_gst_stop(RESPITE_GST(player->priv->gst));

    return TRUE;
}

static gboolean
respite_plugin_player_play_previous(RespiteProviderPlayer *provider) {
    respite_plugin_player_send_message("PrevTrack");

    return TRUE;
}

static gboolean
respite_plugin_player_play_next(RespiteProviderPlayer *provider) {
    respite_plugin_player_send_message("NextTrack");

    return TRUE;
}

static gboolean
respite_plugin_player_seek(RespiteProviderPlayer *provider, gdouble pos) {
    RespitePluginPlayer *player;

    player = RESPITE_PLUGIN_PLAYER(provider);

    respite_gst_seek(RESPITE_GST(player->priv->gst), pos);

    return TRUE;
}

static gboolean
respite_plugin_player_volume_up(RespiteProviderPlayer *provider) {
    respite_plugin_player_send_message("RaiseVolume");

    return TRUE;
}

static gboolean
respite_plugin_player_volume_down(RespiteProviderPlayer *provider) {
    respite_plugin_player_send_message("LowerVolume");

    return TRUE;
}

static gdouble
respite_plugin_player_get_stream_position(RespiteProviderPlayer *provider) {
    RespitePluginPlayer *player;
    gdouble position = 0;

    player = RESPITE_PLUGIN_PLAYER(provider);

    position = respite_gst_get_stream_position(RESPITE_GST(player->priv->gst));

    /* Return as microseconds */
    return position*1000000.0;
}

static void respite_plugin_player_open_media_chooser(RespiteProviderPlayer *provider) {
    RespiteMediaList *list;

    list = RESPITE_MEDIA_LIST(respite_media_list_get());
    respite_media_list_open(list);

    g_object_unref(list);
}

static GSimpleAction* respite_plugin_player_get_action(RespiteProviderPlayer *provider, RespitePlayerAction action) {
    return respite_player_get_action (action);
}

static gboolean respite_plugin_player_get_fullscreen(RespiteProviderPlayer *provider) {
    RespitePluginPlayer *player;

    player = RESPITE_PLUGIN_PLAYER(provider);

    return player->priv->fullscreen;
}

static gboolean respite_plugin_player_set_fullscreen(RespiteProviderPlayer *provider, gboolean fullscreen) {
    GtkWidget *window;

    window = respite_plugin_player_get_main_window(provider);

    if (fullscreen)
        gtk_window_fullscreen(GTK_WINDOW(window));
    else
        gtk_window_unfullscreen(GTK_WINDOW(window));

    return TRUE;
}

static void respite_plugin_player_iface_init(RespiteProviderPlayerIface *iface) {
    iface->get_main_window = respite_plugin_player_get_main_window;
    iface->pack = respite_plugin_player_pack_widget;
    iface->get_state = respite_plugin_player_get_state;
    iface->get_stream = respite_plugin_player_get_stream;
    iface->play_uri = respite_plugin_player_play_uri;
    iface->pause = respite_plugin_player_pause;
    iface->resume = respite_plugin_player_resume;
    iface->stop = respite_plugin_player_stop;
    iface->play_previous = respite_plugin_player_play_previous;
    iface->play_next = respite_plugin_player_play_next;
    iface->seek = respite_plugin_player_seek;
    iface->volume_up = respite_plugin_player_volume_up;
    iface->volume_down = respite_plugin_player_volume_down;
    iface->get_stream_position = respite_plugin_player_get_stream_position;
    iface->open_media_chooser = respite_plugin_player_open_media_chooser;
    iface->get_action = respite_plugin_player_get_action;
    iface->get_fullscreen = respite_plugin_player_get_fullscreen;
    iface->set_fullscreen = respite_plugin_player_set_fullscreen;
}

static void
respite_plugin_player_media_state_changed_cb(RespiteGst *gst, const RespiteStream *stream,
                                            RespiteState state, RespitePluginPlayer *player) {
    g_signal_emit_by_name(G_OBJECT(player), "state-changed", stream, state);
}

static void
respite_plugin_player_media_tag_cb(RespiteGst *gst, const RespiteStream *stream, RespitePluginPlayer *player) {
    g_signal_emit_by_name(G_OBJECT(player), "tag-message", stream);
}

static void
respite_plugin_player_media_seeked_cb(RespiteGst *gst, RespitePluginPlayer *player) {
    g_signal_emit_by_name(G_OBJECT(player), "seeked");
}

static gboolean
respite_plugin_player_window_state_event(GtkWidget *widget,
                                        GdkEventWindowState *event,
                                        RespitePluginPlayer *player) {
    if (event->new_window_state & GDK_WINDOW_STATE_FULLSCREEN)
        player->priv->fullscreen = TRUE;
    else
        player->priv->fullscreen = FALSE;
    return FALSE;
}

static void
respite_plugin_player_class_init(RespitePluginPlayerClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = respite_plugin_player_finalize;
}

static void
respite_plugin_player_init(RespitePluginPlayer *player) {
    GtkWidget *window;
    player->priv = respite_plugin_player_get_instance_private(player);

    player->priv->gst = respite_gst_get();

    g_signal_connect_object(G_OBJECT(player->priv->gst), "media-state",
                            G_CALLBACK(respite_plugin_player_media_state_changed_cb), player, 0);

    g_signal_connect_object(G_OBJECT(player->priv->gst), "media-tag",
                            G_CALLBACK(respite_plugin_player_media_tag_cb), player, 0);

    g_signal_connect_object(G_OBJECT(player->priv->gst), "media-seeked",
                            G_CALLBACK(respite_plugin_player_media_seeked_cb), player, 0);

    player->priv->fullscreen = FALSE;
    window = GTK_WIDGET(gtk_widget_get_toplevel(player->priv->gst));
    g_signal_connect_object(G_OBJECT(window), "window-state-event",
                            G_CALLBACK(respite_plugin_player_window_state_event), player, 0);

    player->priv->packed = FALSE;
    player->priv->box = NULL;
}

static void
respite_plugin_player_finalize(GObject *object) {
    RespitePluginPlayer *player;

    player = RESPITE_PLUGIN_PLAYER(object);

    if (player->priv->packed && player->priv->box != NULL)
        gtk_widget_destroy(player->priv->box);

    G_OBJECT_CLASS(respite_plugin_player_parent_class)->finalize(object);
}

RespitePluginPlayer *
respite_plugin_player_new(void) {
    RespitePluginPlayer *player = NULL;

    player = g_object_new(RESPITE_TYPE_PLUGIN_PLAYER, NULL);

    return player;
}
