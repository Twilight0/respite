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

#ifndef SRC_COMMON_PAROLE_SCREENSAVER_H_
#define SRC_COMMON_PAROLE_SCREENSAVER_H_

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define RESPITE_TYPE_SCREENSAVER        (respite_screen_saver_get_type () )
#define RESPITE_SCREEN_SAVER(o)         (G_TYPE_CHECK_INSTANCE_CAST ((o), RESPITE_TYPE_SCREENSAVER, RespiteScreenSaver))
#define RESPITE_IS_SCREENSAVER(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), RESPITE_TYPE_SCREENSAVER))

typedef struct {
    GObject         parent;
} RespiteScreenSaver;

typedef struct {
    GObjectClass    parent_class;
} RespiteScreenSaverClass;

GType               respite_screen_saver_get_type    (void) G_GNUC_CONST;
RespiteScreenSaver  *respite_screen_saver_new         (void);

void                respite_screen_saver_inhibit     (RespiteScreenSaver *saver, GtkWindow *window);

void                respite_screen_saver_uninhibit   (RespiteScreenSaver *saver, GtkWindow *window);

G_END_DECLS

#endif /* SRC_COMMON_PAROLE_SCREENSAVER_H_ */
