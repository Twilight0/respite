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

#ifndef SRC_RESPITE_CONF_H_
#define SRC_RESPITE_CONF_H_

#include <glib-object.h>

G_BEGIN_DECLS;

#define RESPITE_TYPE_CONF             (respite_conf_get_type () )
G_DECLARE_FINAL_TYPE(RespiteConf, respite_conf, RESPITE, CONF, GObject)

RespiteConf              *respite_conf_new                (void);

gboolean                 respite_conf_get_property_bool  (RespiteConf *conf,
                                                         const gchar *name);

gchar                  **respite_conf_read_entry_list    (RespiteConf *conf,
                                                         const gchar *name);

void                     respite_conf_write_entry_list   (RespiteConf *conf,
                                                         const gchar *name,
                                                         gchar **value);

G_END_DECLS;

#endif /* SRC_RESPITE_CONF_H_ */
