/*
 * * Copyright (C) 2017-2020 Sean Davis <bluesabre@xfce.org>
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

#ifndef SRC_RESPITE_SHORTCUTS_H_
#define SRC_RESPITE_SHORTCUTS_H_

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define RESPITE_TYPE_SHORTCUTS       (respite_shortcuts_get_type () )
G_DECLARE_FINAL_TYPE(RespiteShortcuts, respite_shortcuts, RESPITE, SHORTCUTS, GObject)

RespiteShortcuts     *respite_shortcuts            (GtkWidget *parent);

G_END_DECLS

#endif /* SRC_RESPITE_SHORTCUTS_H_ */
