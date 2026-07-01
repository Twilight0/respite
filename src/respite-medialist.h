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

#ifndef SRC_RESPITE_MEDIALIST_H_
#define SRC_RESPITE_MEDIALIST_H_

#include <gtk/gtk.h>

#include "src/misc/respite-file.h"

G_BEGIN_DECLS

#define RESPITE_TYPE_MEDIA_LIST        (respite_media_list_get_type () )
#define RESPITE_MEDIA_LIST(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), RESPITE_TYPE_MEDIA_LIST, RespiteMediaList))
#define RESPITE_IS_MEDIA_LIST(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), RESPITE_TYPE_MEDIA_LIST))

enum {
    STATE_COL,
    NAME_COL,
    LENGTH_COL,
    DATA_COL,
    ENTRY_COL,
    SORT_COL,
    COL_NUMBERS
};

enum {
    RESPITE_MEDIA_STATE_NONE,
    RESPITE_MEDIA_STATE_PAUSED,
    RESPITE_MEDIA_STATE_PLAYING
};

enum {
    RESPITE_MEDIA_LIST_PLAYLIST_VIEW_STANDARD,
    RESPITE_MEDIA_LIST_PLAYLIST_VIEW_DISC
};

typedef struct RespiteMediaListPrivate RespiteMediaListPrivate;

typedef struct {
    GtkBox                      parent;
    RespiteMediaListPrivate     *priv;
} RespiteMediaList;

typedef struct {
    GtkBoxClass     parent_class;

    void            (*media_activated)              (RespiteMediaList *list,
                                                     GtkTreeRowReference *row);

    void            (*media_cursor_changed)         (RespiteMediaList *list,
                                                     gboolean media_selected);

    void            (*uri_opened)                   (RespiteMediaList *list,
                                                     const gchar *uri);

    void            (*gst_dvd_nav_message)          (RespiteMediaList *list,
                                                     gint gst_dvd_nav_message);

    void            (*iso_opened)                   (RespiteMediaList *list,
                                                     const gchar *filename);

    void            (*dvd_chapter_count)            (RespiteMediaList *list,
                                                     gint chapter_count);

    void            (*key_fwd_event)                (RespiteMediaList *list,
                                                     gpointer *event);
} RespiteMediaListClass;

GType               respite_media_list_get_type      (void) G_GNUC_CONST;

GtkWidget          *respite_media_list_get           (void);

void                respite_media_list_load          (RespiteMediaList *list);

void
respite_media_list_set_playlist_view                 (RespiteMediaList *list,
                                                     gint view);

void
respite_media_list_clear_disc_list                   (RespiteMediaList *list);

void                respite_media_list_clear_list    (RespiteMediaList *list);

gboolean            respite_media_list_add_by_path   (RespiteMediaList *list,
                                                     const gchar *path,
                                                     gboolean emit);

gboolean
respite_media_list_is_selected_row                   (RespiteMediaList *list);

gboolean            respite_media_list_is_empty      (RespiteMediaList *list);

gint
respite_media_list_get_playlist_count                (RespiteMediaList *list);

GtkTreeRowReference
*respite_media_list_get_first_row                    (RespiteMediaList *list);

GtkTreeRowReference
*respite_media_list_get_selected_row                 (RespiteMediaList *list);

RespiteFile
*respite_media_list_get_selected_file                (RespiteMediaList *list);

void                respite_media_list_select_row    (RespiteMediaList *list,
                                                     GtkTreeRowReference *row);

GtkTreeRowReference
*respite_media_list_get_next_row                     (RespiteMediaList *list,
                                                     GtkTreeRowReference *row,
                                                     gboolean repeat);

GtkTreeRowReference
*respite_media_list_get_prev_row                     (RespiteMediaList *list,
                                                     GtkTreeRowReference *row,
                                                     gboolean repeat);

void
respite_media_list_set_row_playback_state            (RespiteMediaList *list,
                                                     GtkTreeRowReference *row,
                                                     gint state);

gchar              *respite_media_list_get_row_name  (RespiteMediaList *list,
                                                     GtkTreeRowReference *row);

void                respite_media_list_set_row_name  (RespiteMediaList *list,
                                                     GtkTreeRowReference *row,
                                                     const gchar *name);

void
respite_media_list_set_row_length                    (RespiteMediaList *list,
                                                     GtkTreeRowReference *row,
                                                     const gchar *length);

guint
respite_media_list_get_row_entry_order               (RespiteMediaList *list,
                                                     GtkTreeRowReference *row);

void                respite_media_list_open          (RespiteMediaList *list);

void                respite_media_list_open_location (RespiteMediaList *list);

void                respite_media_list_open_uri      (RespiteMediaList *list,
                                                     const gchar *uri);

gboolean            respite_media_list_add_files     (RespiteMediaList *list,
                                                     gchar **filenames,
                                                     gboolean enqueue);

void
respite_media_list_add_cdda_tracks                   (RespiteMediaList *list,
                                                     gint n_tracks);

void
respite_media_list_add_dvd_chapters                  (RespiteMediaList *list,
                                                     gint n_chapters);

GtkTreeRowReference
*respite_media_list_get_row_n                        (RespiteMediaList *list,
                                                     gint wanted_row);

void                respite_media_list_save_list     (RespiteMediaList *list);

void                respite_media_list_save_cb       (GtkWidget *widget,
                                                     RespiteMediaList *list);

void
respite_media_list_connect_repeat_action             (RespiteMediaList *list,
                                                     GSimpleAction *simple);

void
respite_media_list_connect_shuffle_action            (RespiteMediaList *list,
                                                     GSimpleAction *simple);


G_END_DECLS

#endif /* SRC_RESPITE_MEDIALIST_H_ */
