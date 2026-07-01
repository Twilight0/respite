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

#if !defined (__RESPITE_H_INSIDE__) && !defined (RESPITE_COMPILATION)
#error "Only <respite.h> can be included directly."
#endif

#ifndef SRC_MISC_RESPITE_PROVIDER_PLAYER_H_
#define SRC_MISC_RESPITE_PROVIDER_PLAYER_H_

#include <gtk/gtk.h>

#include "src/misc/respite-stream.h"

#include "src/respite-player.h"

G_BEGIN_DECLS

#define RESPITE_TYPE_PROVIDER_PLAYER                 (respite_provider_player_get_type ())
#define RESPITE_PROVIDER_PLAYER_GET_IFACE(provider) RESPITE_PROVIDER_PLAYER_GET_INTERFACE(provider)
G_DECLARE_INTERFACE(RespiteProviderPlayer, respite_provider_player, RESPITE, PROVIDER_PLAYER, GObject)

/**
 * RespiteProviderPlayerIface:
 *
 * Interface for #RespiteProviderPlayer
 *
 * The player interface that the plugins should use in order to, issue playback
 * commands to the Parole GStreamer backend or to get information about the
 * current playback status.
 *
 * Since: 0.2
 */
typedef RespiteProviderPlayerInterface RespiteProviderPlayerIface;

/**
 * ParolePluginContainer:
 * @PAROLE_PLUGIN_CONTAINER_PLAYLIST: Playlist container
 * @PAROLE_PLUGIN_CONTAINER_MAIN_VIEW: Main view container
 *
 * Available plugin widget containers.
 *
 **/
typedef enum {
    PAROLE_PLUGIN_CONTAINER_PLAYLIST,
    PAROLE_PLUGIN_CONTAINER_MAIN_VIEW
} ParolePluginContainer;



struct _RespiteProviderPlayerInterface {
    GTypeInterface __parent__;

    /*< private >*/
    GtkWidget   *(*get_main_window)             (RespiteProviderPlayer *player);

    void        (*pack)                         (RespiteProviderPlayer *player,
                                                 GtkWidget *widget,
                                                 const gchar *title,
                                                 ParolePluginContainer container);

    RespiteState (*get_state)                    (RespiteProviderPlayer *player);

    const RespiteStream *(*get_stream)           (RespiteProviderPlayer *player);

    gboolean     (*play_uri)                    (RespiteProviderPlayer *player,
                                                 const gchar *uri);

    gboolean     (*pause)                       (RespiteProviderPlayer *player);

    gboolean     (*resume)                      (RespiteProviderPlayer *player);

    gboolean     (*stop)                        (RespiteProviderPlayer *player);

    gboolean     (*play_previous)               (RespiteProviderPlayer *player);

    gboolean     (*play_next)                   (RespiteProviderPlayer *player);

    gboolean     (*seek)                        (RespiteProviderPlayer *player,
                                                 gdouble pos);

    gboolean     (*volume_up)                   (RespiteProviderPlayer *player);

    gboolean     (*volume_down)                 (RespiteProviderPlayer *player);

    gdouble      (*get_stream_position)         (RespiteProviderPlayer *player);

    gboolean     (*get_fullscreen)              (RespiteProviderPlayer *player);

    gboolean     (*set_fullscreen)              (RespiteProviderPlayer *player,
                                                 gboolean fullscreen);

    void         (*open_media_chooser)          (RespiteProviderPlayer *player);

    GSimpleAction*   (*get_action)              (RespiteProviderPlayer *player,
                                                 RespitePlayerAction action);

    /*< signals >*/
    void     (*tag_message)                     (RespiteProviderPlayer *player,
                                                 const RespiteStream *stream);

    void     (*seeked)                          (RespiteProviderPlayer *player,
                                                 gdouble value);

    void     (*state_changed)                   (RespiteProviderPlayer *player,
                                                 const RespiteStream *stream,
                                                 RespiteState state);
};

GtkWidget   *respite_provider_player_get_main_window
                                                (RespiteProviderPlayer *player);

void         respite_provider_player_pack        (RespiteProviderPlayer *player,
                                                 GtkWidget *widget,
                                                 const gchar *title,
                                                 ParolePluginContainer container);

RespiteState respite_provider_player_get_state    (RespiteProviderPlayer *player);

const RespiteStream *respite_provider_player_get_stream
                                                (RespiteProviderPlayer *player);

gboolean    respite_provider_player_play_uri     (RespiteProviderPlayer *player,
                                                 const gchar *uri);

gboolean    respite_provider_player_pause        (RespiteProviderPlayer *player);

gboolean    respite_provider_player_resume       (RespiteProviderPlayer *player);

gboolean    respite_provider_player_stop         (RespiteProviderPlayer *player);

gboolean    respite_provider_player_play_previous(RespiteProviderPlayer *player);

gboolean    respite_provider_player_play_next    (RespiteProviderPlayer *player);

gboolean    respite_provider_player_seek         (RespiteProviderPlayer *player,
                                                 gdouble pos);
gboolean    respite_provider_player_volume_up    (RespiteProviderPlayer *player);

gboolean    respite_provider_player_volume_down  (RespiteProviderPlayer *player);

gdouble respite_provider_player_get_stream_position(RespiteProviderPlayer *player);

void
respite_provider_player_open_media_chooser       (RespiteProviderPlayer *player);

GSimpleAction *respite_provider_player_get_action(RespiteProviderPlayer *player, RespitePlayerAction action);

gboolean    respite_provider_player_get_fullscreen(RespiteProviderPlayer *player);

gboolean    respite_provider_player_set_fullscreen(RespiteProviderPlayer *player,
                                                 gboolean fullscreen);

G_END_DECLS

#endif /* SRC_MISC_RESPITE_PROVIDER_PLAYER_H_ */
