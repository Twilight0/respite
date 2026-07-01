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

#ifndef SRC_MISC_RESPITE_FILE_H_
#define SRC_MISC_RESPITE_FILE_H_

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _RespiteFile        RespiteFile;
typedef struct _RespiteFileClass   RespiteFileClass;
typedef struct _RespiteFilePrivate RespiteFilePrivate;

#define RESPITE_TYPE_FILE        (respite_file_get_type () )
#define RESPITE_FILE(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), RESPITE_TYPE_FILE, RespiteFile))
#define RESPITE_IS_FILE(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), RESPITE_TYPE_FILE))
G_DEFINE_AUTOPTR_CLEANUP_FUNC(RespiteFile, g_object_unref)


/**
 * RespiteFile:
 *
 * File instance used by Parole.
 *
 * Since: 0.2
 */
struct _RespiteFile {
    GObject             parent;
    RespiteFilePrivate  *priv;
};

struct _RespiteFileClass {
    GObjectClass        parent_class;
};

GType           respite_file_get_type                (void) G_GNUC_CONST;

RespiteFile     *respite_file_new                     (const gchar *filename);

RespiteFile     *respite_file_new_with_display_name   (const gchar *filename,
                                                     const gchar *display_name);

RespiteFile     *respite_file_new_cdda_track          (const gint track_num,
                                                     const gchar *display_name);

RespiteFile     *respite_file_new_dvd_chapter         (gint chapter_num,
                                                     const gchar *display_name);

void            respite_file_set_custom_subtitles    (const RespiteFile *file, gchar *suburi);

void            respite_file_set_dvd_chapter         (const RespiteFile *file, gint dvd_chapter);

gint            respite_file_get_dvd_chapter         (const RespiteFile *file);

const gchar
*respite_file_get_directory           (const RespiteFile *file) G_GNUC_PURE;

const gchar
*respite_file_get_file_name           (const RespiteFile *file) G_GNUC_PURE;

const gchar
*respite_file_get_display_name        (const RespiteFile *file) G_GNUC_PURE;

const gchar
*respite_file_get_uri                 (const RespiteFile *file) G_GNUC_PURE;

const gchar
*respite_file_get_content_type        (const RespiteFile *file) G_GNUC_PURE;

const gchar
*respite_file_get_custom_subtitles    (const RespiteFile *file) G_GNUC_PURE;


G_END_DECLS

#endif /* SRC_MISC_RESPITE_FILE_H_ */
