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

#ifndef SRC_GST_RESPITE_GST_H_
#define SRC_GST_RESPITE_GST_H_

#include <glib-object.h>
#include <gst/gst.h>
#include <gtk/gtk.h>

#include "src/misc/respite-enum-types.h"
#include "src/misc/respite-stream.h"

G_BEGIN_DECLS

#define RESPITE_TYPE_GST        (respite_gst_get_type () )
#define RESPITE_GST(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), RESPITE_TYPE_GST, RespiteGst))
#define RESPITE_IS_GST(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), RESPITE_TYPE_GST))

typedef enum {
    RESPITE_ASPECT_RATIO_NONE,
    RESPITE_ASPECT_RATIO_AUTO,
    RESPITE_ASPECT_RATIO_SQUARE,
    RESPITE_ASPECT_RATIO_4_3,
    RESPITE_ASPECT_RATIO_16_9,
    RESPITE_ASPECT_RATIO_DVB
} RespiteAspectRatio;

enum {
    GST_DVD_TITLE_MENU,
    GST_DVD_ROOT_MENU,
    GST_DVD_SUBPICTURE_MENU,
    GST_DVD_AUDIO_MENU,
    GST_DVD_ANGLE_MENU,
    GST_DVD_CHAPTER_MENU
};

typedef struct RespiteGstPrivate RespiteGstPrivate;

typedef struct {
    GtkWidget           parent;
    RespiteGstPrivate   *priv;
} RespiteGst;

typedef struct {
    GtkWidgetClass  parent_class;

    void        (*media_state)              (RespiteGst *gst,
                                             const RespiteStream *stream,
                                             RespiteState state);

    void        (*media_progressed)         (RespiteGst *gst,
                                             const RespiteStream *stream,
                                             gint64 value);

    void        (*media_seeked)             (RespiteGst *gst,
                                             const RespiteStream *stream);

    void        (*buffering)                (RespiteGst *gst,
                                             const RespiteStream *stream,
                                             gint percentage);

    void        (*media_tag)                (RespiteGst *gst,
                                             const RespiteStream *stream);

    void        (*error)                    (RespiteGst *gst,
                                             const gchar *error);

    void        (*dvd_chapter_change)       (RespiteGst *gst,
                                             gint dvd_chapter_change);

    void        (*dvd_chapter_count_change) (RespiteGst *gst,
                                             gint dvd_chapter_change);
} RespiteGstClass;

GType       respite_gst_get_type             (void) G_GNUC_CONST;
GtkWidget  *respite_gst_new                  (gpointer conf_obj);
GtkWidget  *respite_gst_get                  (void);

void        respite_gst_set_custom_subtitles (RespiteGst *gst,
                                             const gchar* sub_file);

gchar*      respite_gst_get_file_uri         (RespiteGst *gst);

void        respite_gst_play_uri             (RespiteGst *gst,
                                             const gchar *uri,
                                             const gchar *subtitles);

void        respite_gst_play_pipe            (RespiteGst *gst,
                                             const gchar *url);

void        respite_gst_play_device_uri      (RespiteGst *gst,
                                             const gchar *uri,
                                             const gchar *device);

void        respite_gst_pause                (RespiteGst *gst);
void        respite_gst_resume               (RespiteGst *gst);
void        respite_gst_stop                 (RespiteGst *gst);
void        respite_gst_terminate            (RespiteGst *gst);
void        respite_gst_shutdown             (RespiteGst *gst);

void        respite_gst_seek                 (RespiteGst *gst,
                                             gdouble pos);

void        respite_gst_set_volume           (RespiteGst *gst,
                                             gdouble value);

gdouble     respite_gst_get_volume           (RespiteGst *gst);

RespiteState respite_gst_get_state            (RespiteGst *gst);
GstState    respite_gst_get_gst_state        (RespiteGst *gst);
GstState    respite_gst_get_gst_target_state (RespiteGst *gst);

void
respite_gst_send_navigation_command          (RespiteGst *gst,
                                             gint command);

void        respite_gst_next_dvd_chapter     (RespiteGst *gst);
void        respite_gst_prev_dvd_chapter     (RespiteGst *gst);

void        respite_gst_set_dvd_chapter      (RespiteGst *gst,
                                             gint chapter);

gint        respite_gst_get_num_tracks       (RespiteGst *gst);

void        respite_gst_seek_cdda            (RespiteGst *gst,
                                             guint track_num);

gint
respite_gst_get_current_cdda_track           (RespiteGst *gst);

RespiteMediaType
respite_gst_get_current_stream_type          (RespiteGst *gst);

gint64      respite_gst_get_stream_duration  (RespiteGst *gst);
gint64      respite_gst_get_stream_position  (RespiteGst *gst);

gboolean    respite_gst_get_is_xvimage_sink  (RespiteGst *gst);

void        respite_gst_set_cursor_visible   (RespiteGst *gst,
                                             gboolean visible);

GstElement *respite_gst_video_sink           (RespiteGst *gst);

GList *     gst_get_lang_list_for_type      (RespiteGst * gst,
                                             const gchar * type_name);

gboolean    gst_get_has_vis                 (RespiteGst *gst);
gboolean    gst_get_has_video               (RespiteGst *gst);

void        gst_set_current_audio_track     (RespiteGst *gst,
                                             gint track_no);

void        gst_set_current_subtitle_track  (RespiteGst *gst,
                                             gint track_no);

const RespiteStream
*respite_gst_get_stream                      (RespiteGst *gst);

G_END_DECLS

#endif /* SRC_GST_RESPITE_GST_H_ */
