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

#ifndef SRC_RESPITE_PLAYER_H_
#define SRC_RESPITE_PLAYER_H_

#include <glib-object.h>

#include "src/respite-medialist.h"

G_BEGIN_DECLS

#define RESPITE_TYPE_PLAYER        (respite_player_get_type () )
#define RESPITE_PLAYER(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), RESPITE_TYPE_PLAYER, RespitePlayer))
#define RESPITE_IS_PLAYER(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), RESPITE_TYPE_PLAYER))

typedef struct RespitePlayerPrivate RespitePlayerPrivate;

typedef struct {
    GObject                     parent;
    RespitePlayerPrivate        *priv;
} RespitePlayer;

typedef struct {
    GObjectClass                parent_class;

    void                        (*shuffle_toggled)              (RespitePlayer *player,
                                                                 gboolean shuffle_toggled);

    void                        (*repeat_toggled)               (RespitePlayer *player,
                                                                 gboolean repeat_toggled);

    void                        (*gst_dvd_nav_message)          (RespitePlayer *player,
                                                                 gint gst_dvd_nav_message);
} RespitePlayerClass;

typedef enum {
    RESPITE_PLAYER_ACTION_PREVIOUS,
    RESPITE_PLAYER_ACTION_NEXT,
    RESPITE_PLAYER_ACTION_PLAYPAUSE
} RespitePlayerAction;

GType                           respite_player_get_type          (void) G_GNUC_CONST;
RespitePlayer                   *respite_player_new               (const gchar *client_id);

RespiteMediaList                *respite_player_get_media_list    (RespitePlayer *player);

void                            respite_player_play_uri_disc     (RespitePlayer *player,
                                                                 const gchar *uri,
                                                                 const gchar *device);

void                            respite_player_terminate         (RespitePlayer *player);

void                            respite_player_embedded          (RespitePlayer *player);

void                            respite_player_full_screen       (RespitePlayer *player,
                                                                 gboolean fullscreen);

GSimpleAction                  *respite_player_get_action(RespitePlayerAction action);


G_END_DECLS

#endif /* SRC_RESPITE_PLAYER_H_ */
