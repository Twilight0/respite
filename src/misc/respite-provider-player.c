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

#include "src/misc/respite-enum-types.h"
#include <glib/gi18n.h>
#include "src/misc/respite-marshal.h"

#include "src/misc/respite-provider-player.h"

G_DEFINE_INTERFACE(RespiteProviderPlayer, respite_provider_player, G_TYPE_OBJECT)

static void respite_provider_player_default_init(RespiteProviderPlayerInterface *klass) {
    static gboolean initialized = FALSE;

    if (G_UNLIKELY(!initialized)) {
        /**
         * RespiteProviderPlayerIface::state-changed:
         * @player: the object which received the signal
         * @stream: a #RespiteStream
         * @state: the new state
         *
         * Issued when the Parole state changed.
         *
         * Since: 0.2
         **/
        g_signal_new("state-changed",
                        G_TYPE_FROM_INTERFACE(klass),
                        G_SIGNAL_RUN_LAST,
                        G_STRUCT_OFFSET(RespiteProviderPlayerIface, state_changed),
                        NULL, NULL,
                        respite_marshal_VOID__OBJECT_ENUM,
                        G_TYPE_NONE, 2,
                        RESPITE_TYPE_STREAM, RESPITE_TYPE_STATE);

        /**
         * RespiteProviderPlayerIface::tag-message:
         * @player: the object which received the signal
         * @stream: a #RespiteStream
         *
         * Indicates that the stream tags were found and ready to be read.
         *
         * Since: 0.2
         **/
        g_signal_new("tag-message",
                        G_TYPE_FROM_INTERFACE(klass),
                        G_SIGNAL_RUN_LAST,
                        G_STRUCT_OFFSET(RespiteProviderPlayerIface, tag_message),
                        NULL, NULL,
                        g_cclosure_marshal_VOID__OBJECT,
                        G_TYPE_NONE, 1, RESPITE_TYPE_STREAM);

        /**
         * RespiteProviderPlayerIface::seeked:
         * @player: the object which received the signal
         * @value: the seeked position
         *
         * Notifies when the stream has been manually advanced.
         *
         * Since: 0.6
         **/
        g_signal_new("seeked",
                        G_TYPE_FROM_INTERFACE(klass),
                        G_SIGNAL_RUN_LAST,
                        G_STRUCT_OFFSET(RespiteProviderPlayerIface, seeked),
                        NULL, NULL,
                        g_cclosure_marshal_VOID__DOUBLE,
                        G_TYPE_NONE, 1, G_TYPE_DOUBLE);

        initialized = TRUE;
    }
}

/**
 * respite_provider_player_get_main_window:
 * @player: a #RespiteProviderPlayer
 *
 * Ask the Player to get the Parole main window.
 *
 * Returns: a #GtkWidget window
 *
 * Since: 0.2
 **/
GtkWidget *respite_provider_player_get_main_window(RespiteProviderPlayer *player) {
    GtkWidget *window = NULL;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), NULL);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->get_main_window) {
        window = (*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->get_main_window)(player);
    }
    return window;
}

/**
 * respite_provider_player_pack:
 * @player: a #RespiteProviderPlayer
 * @widget: a #GtkWidget
 * @title: title
 * @container: a #ParolePluginContainer
 *
 * Ask the player to pack a widget in the playlist notebook if PAROLE_PLUGIN_CONTAINER_PLAYLIST
 * is specified or in the main window notebook if PAROLE_PLUGIN_CONTAINER_MAIN_VIEW is specified.
 *
 * This function can be called once, the Player is responsible on removing the widget in
 * case the plugin was unloaded.
 *
 *
 * Since: 0.2
 **/
void respite_provider_player_pack(RespiteProviderPlayer *player, GtkWidget *widget,
                                 const gchar *title, ParolePluginContainer container) {
    g_return_if_fail(RESPITE_IS_PROVIDER_PLAYER(player));

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->pack) {
        (*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->pack)(player, widget, title, container);
    }
}

/**
 * respite_provider_player_get_state:
 * @player: a #RespiteProviderPlayer
 *
 * Get the current state of the player.
 *
 * Returns: a #RespiteState
 *
 *
 * Since: 0.2
 **/
RespiteState respite_provider_player_get_state(RespiteProviderPlayer *player) {
    RespiteState state = PAROLE_STATE_STOPPED;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), PAROLE_STATE_STOPPED);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->get_state) {
        state =(*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->get_state)(player);
    }

    return state;
}

/**
 * respite_provider_player_get_stream:
 * @player: a #RespiteProviderPlayer
 *
 * Get the #RespiteStream object.
 *
 * Returns: the #RespiteStream object
 *
 * Since: 0.2
 **/
const RespiteStream *respite_provider_player_get_stream(RespiteProviderPlayer *player) {
    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), NULL);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->get_stream) {
        return (*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->get_stream) (player);
    }

    return NULL;
}

/**
 * respite_provider_player_play_uri:
 * @player: a #RespiteProviderPlayer
 * @uri: uri
 *
 * Issue a play command on the backend for the given uri, note this function
 * can be called only of the Parole current state is PAROLE_STATE_STOPPED.
 *
 * Returning TRUE doesn't mean that the funtion succeeded to change the state of the player,
 * the state change is indicated asynchronously by #RespiteProviderPlayerIface::state-changed signal.
 *
 * Returns: TRUE if the command is processed, FALSE otherwise
 *
 * Since: 0.2
 **/
gboolean respite_provider_player_play_uri(RespiteProviderPlayer *player, const gchar *uri) {
    gboolean ret = FALSE;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), FALSE);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->play_uri) {
        ret =(*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->play_uri)(player, uri);
    }
    return ret;
}


/**
 * respite_provider_player_pause:
 * @player: a #RespiteProviderPlayer
 *
 * Issue a pause command to the backend, this function can be called when the state of the player
 * is PAROLE_STATE_PLAYING.
 *
 * Returning TRUE doesn't mean that the funtion succeeded to change the state of the player,
 * the state change is indicated asynchronously by #RespiteProviderPlayerIface::state-changed signal.
 *
 * Returns: TRUE if the command is processed, FALSE otherwise
 *
 *
 * Since: 0.2
 **/
gboolean respite_provider_player_pause(RespiteProviderPlayer *player) {
    gboolean ret = FALSE;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), FALSE);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->pause) {
        ret =(*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->pause)(player);
    }

    return ret;
}


/**
 * respite_provider_player_resume:
 * @player: a #RespiteProviderPlayer
 *
 *
 * Issue a resume command to the player, this function can be called when
 * the current state of the player is PAROLE_STATE_PAUSED.
 *
 * Returning TRUE doesn't mean that the funtion succeeded to change the state of the player,
 * the state change is indicated asynchronously by #RespiteProviderPlayerIface::state-changed signal.
 *
 * Returns: TRUE if the command is processed, FALSE otherwise
 *
 *
 * Since: 0.2
 **/
gboolean respite_provider_player_resume(RespiteProviderPlayer *player) {
    gboolean ret = FALSE;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), FALSE);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->resume) {
        ret =(*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->resume)(player);
    }

    return ret;
}


/**
 * respite_provider_player_stop:
 * @player: a #RespiteProviderPlayer
 *
 * Issue a stop command to the player.
 *
 * Returning TRUE doesn't mean that the funtion succeeded to change the state of the player,
 * the state change is indicated asynchronously by #RespiteProviderPlayerIface::state-changed signal.
 *
 * Returns: TRUE if the command is processed, FALSE otherwise
 *
 * Since: 0.2
 **/
gboolean respite_provider_player_stop(RespiteProviderPlayer *player) {
    gboolean ret = FALSE;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), FALSE);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->stop) {
        ret =(*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->stop)(player);
    }

    return ret;
}


/**
 * respite_provider_player_play_previous:
 * @player: a #RespiteProviderPlayer
 *
 * Issue a play previous command to the player.
 *
 * Returns: TRUE if the command is processed, FALSE otherwise
 *
 * Since: 0.6
 **/
gboolean respite_provider_player_play_previous(RespiteProviderPlayer *player) {
    gboolean ret = FALSE;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), FALSE);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->play_previous) {
        ret =(*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->play_previous)(player);
    }

    return ret;
}


/**
 * respite_provider_player_play_next:
 * @player: a #RespiteProviderPlayer
 *
 * Issue a play next command to the player.
 *
 * Returns: TRUE if the command is processed, FALSE otherwise
 *
 * Since: 0.6
 **/
gboolean respite_provider_player_play_next(RespiteProviderPlayer *player) {
    gboolean ret = FALSE;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), FALSE);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->play_next) {
        ret =(*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->play_next)(player);
    }

    return ret;
}


/**
 * respite_provider_player_seek:
 * @player: a #RespiteProviderPlayer
 * @pos: position to seek
 *
 *
 * Issue a seek command.
 *
 * Returns: TRUE if the seek command succeeded, FALSE otherwise
 *
 *
 * Since: 0.2
 **/
gboolean respite_provider_player_seek(RespiteProviderPlayer *player, gdouble pos) {
    gboolean ret = FALSE;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), FALSE);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->seek) {
        ret =(*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->seek)(player, pos);
    }

    return ret;
}


/**
 * respite_provider_player_volume_up
 * @player: a #RespiteProviderPlayer
 *
 * Issue a volume up command to the player.
 *
 * Returns: TRUE if the command is processed, FALSE otherwise
 *
 * Since: 4.18.1
 **/
gboolean respite_provider_player_volume_up(RespiteProviderPlayer *player) {
    gboolean ret = FALSE;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), FALSE);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->volume_up) {
        ret =(*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->volume_up)(player);
    }

    return ret;
}


/**
 * respite_provider_player_volume_down
 * @player: a #RespiteProviderPlayer
 *
 * Issue a volume down command to the player.
 *
 * Returns: TRUE if the command is processed, FALSE otherwise
 *
 * Since: 4.18.1
 **/
gboolean respite_provider_player_volume_down(RespiteProviderPlayer *player) {
    gboolean ret = FALSE;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), FALSE);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->volume_down) {
        ret =(*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->volume_down)(player);
    }

    return ret;
}

/**
 * respite_provider_player_get_stream_position:
 * @player: a #RespiteProviderPlayer
 *
 * Get the stream position (in microseconds) for Parole.
 *
 * Returns: a #gdouble containing the current stream position in microseconds
 *
 * Since: 0.6
 **/
gdouble respite_provider_player_get_stream_position(RespiteProviderPlayer *player) {
    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), 0);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->get_stream_position) {
        return (*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->get_stream_position) (player);
    }

    return FALSE;
}


/**
 * respite_provider_player_open_media_chooser:
 * @player: a #RespiteProviderPlayer
 *
 * Ask Parole to open its media chooser dialog.
 *
 * Since: 0.2
 **/
void respite_provider_player_open_media_chooser(RespiteProviderPlayer *player) {
    g_return_if_fail(RESPITE_IS_PROVIDER_PLAYER(player));

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->open_media_chooser) {
        (*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->open_media_chooser)(player);
    }
}

/**
 * respite_provider_player_get_action:
 * @player: a #RespiteProviderPlayer
 * @action: the #RespitePlayerAction to retrieve
 *
 * Get GSimpleAction from Parole.
 *
 * Returns: the requested #GSimpleAction
 *
 *
 * Since: 0.6
 **/
GSimpleAction *respite_provider_player_get_action(RespiteProviderPlayer *player, RespitePlayerAction action) {
    GSimpleAction *ret = NULL;;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), NULL);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->get_action) {
        ret =(*RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->get_action)(player, action);
    }

    return ret;
}

/**
 * respite_provider_player_get_fullscreen:
 * @player: a #RespiteProviderPlayer
 *
 * Get fullscreen status for Parole.
 *
 * Returns: TRUE if the player window is fullscreen, FALSE otherwise
 *
 * Since: 0.6
 **/
gboolean respite_provider_player_get_fullscreen(RespiteProviderPlayer *player) {
    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), FALSE);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->get_fullscreen) {
        return (*RESPITE_PROVIDER_PLAYER_GET_INTERFACE (player)->get_fullscreen) (player);
    }

    return FALSE;
}

/**
 * respite_provider_player_set_fullscreen:
 * @player: a #RespiteProviderPlayer
 * @fullscreen: TRUE for fullscreen, FALSE for unfullscreen
 *
 * Set fullscreen status for Parole.
 *
 * Returns: TRUE if the fullscreen command succeeded, FALSE otherwise
 *
 * Since: 0.6
 **/
gboolean respite_provider_player_set_fullscreen(RespiteProviderPlayer *player, gboolean fullscreen) {
    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLAYER(player), FALSE);

    if (RESPITE_PROVIDER_PLAYER_GET_INTERFACE(player)->get_stream) {
        return (*RESPITE_PROVIDER_PLAYER_GET_INTERFACE (player)->set_fullscreen) (player, fullscreen);
    }

    return FALSE;
}
