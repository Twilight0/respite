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

#ifndef SRC_MISC_RESPITE_STREAM_H_
#define SRC_MISC_RESPITE_STREAM_H_

#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

#define RESPITE_TYPE_STREAM        (respite_stream_get_type () )
#define RESPITE_STREAM(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), RESPITE_TYPE_STREAM, RespiteStream))
#define RESPITE_IS_STREAM(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), RESPITE_TYPE_STREAM))

/**
 * RespiteMediaType:
 * @PAROLE_MEDIA_TYPE_UNKNOWN: Unknown media type
 * @PAROLE_MEDIA_TYPE_LOCAL_FILE: File found on hard disk
 * @PAROLE_MEDIA_TYPE_CDDA: Audio CD
 * @PAROLE_MEDIA_TYPE_VCD: Video CD
 * @PAROLE_MEDIA_TYPE_SVCD: Super Video CD
 * @PAROLE_MEDIA_TYPE_DVD: DVD
 * @PAROLE_MEDIA_TYPE_DVB: Digital Video Broadcast
 * @PAROLE_MEDIA_TYPE_REMOTE: File stored remotely
 *
 * Media types.
 *
 **/
typedef enum {
    PAROLE_MEDIA_TYPE_UNKNOWN,
    PAROLE_MEDIA_TYPE_LOCAL_FILE,
    PAROLE_MEDIA_TYPE_CDDA,
    PAROLE_MEDIA_TYPE_VCD,
    PAROLE_MEDIA_TYPE_SVCD,
    PAROLE_MEDIA_TYPE_DVD,
    PAROLE_MEDIA_TYPE_DVB,
    PAROLE_MEDIA_TYPE_REMOTE
} RespiteMediaType;

/**
 * RespiteState:
 * @PAROLE_STATE_STOPPED: Playback is stopped
 * @PAROLE_STATE_PLAYBACK_FINISHED: Playback has finished
 * @PAROLE_STATE_ABOUT_TO_FINISH: Playback is about to finish
 * @PAROLE_STATE_PAUSED: Playback is paused
 * @PAROLE_STATE_PLAYING: Playback is active
 *
 * Media playback states.
 *
 **/
typedef enum {
    PAROLE_STATE_STOPPED = 0,
    PAROLE_STATE_PLAYBACK_FINISHED,
    PAROLE_STATE_ABOUT_TO_FINISH,
    PAROLE_STATE_PAUSED,
    PAROLE_STATE_PLAYING
} RespiteState;

/**
 * RespiteStream:
 *
 * This object contains all the information describing the current processed
 * stream by Parole, this object is used with the callback function the
 * #RespiteProviderPlayerIface::state-changed signal of the player, the plugin
 * shouldn't take reference and all the properties are read-only for the plugins.
 *
 * Since: 0.2
 */
typedef struct _RespiteStream        RespiteStream;
typedef struct _RespiteStreamClass   RespiteStreamClass;
typedef struct _RespiteStreamPrivate RespiteStreamPrivate;
G_DEFINE_AUTOPTR_CLEANUP_FUNC(RespiteStream, g_object_unref)

struct _RespiteStream {
    GObject              parent;
    RespiteStreamPrivate *priv;
};

struct _RespiteStreamClass {
    GObjectClass         parent_class;
};

GType                   respite_stream_get_type          (void) G_GNUC_CONST;

void                    respite_stream_set_image         (GObject *object,
                                                         GdkPixbuf *pixbuf);

GdkPixbuf              *respite_stream_get_image         (GObject *object);

RespiteStream           *respite_stream_new               (void);

void                    respite_stream_init_properties   (RespiteStream *stream);

G_END_DECLS

#endif /* SRC_MISC_RESPITE_STREAM_H_ */
