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

#ifndef SRC_RESPITE_PLUGINS_MANAGER_H_
#define SRC_RESPITE_PLUGINS_MANAGER_H_

#include <glib-object.h>

#include "src/misc/respite-provider-player.h"

G_BEGIN_DECLS

#define RESPITE_TYPE_PLUGINS_MANAGER        (respite_plugins_manager_get_type () )
#define RESPITE_PLUGINS_MANAGER(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), RESPITE_TYPE_PLUGINS_MANAGER, RespitePluginsManager))
#define RESPITE_IS_PLUGINS_MANAGER(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), RESPITE_TYPE_PLUGINS_MANAGER))

typedef struct RespitePluginsManagerPrivate RespitePluginsManagerPrivate;

typedef struct {
    GObject                         parent;
    RespitePluginsManagerPrivate    *priv;
} RespitePluginsManager;

typedef struct {
    GObjectClass                    parent_class;
} RespitePluginsManagerClass;

GType                               respite_plugins_manager_get_type   (void) G_GNUC_CONST;

RespitePluginsManager               *respite_plugins_manager_new        (gboolean load_plugins);

RespitePluginsManager               *respite_plugins_manager_get        (void);

void                                respite_plugins_manager_load       (RespitePluginsManager *manager);

void                                respite_plugins_manager_pack       (RespitePluginsManager *manager,
                                                                       GtkWidget *widget,
                                                                       const gchar *title,
                                                                       ParolePluginContainer container);

G_END_DECLS

#endif /* SRC_RESPITE_PLUGINS_MANAGER_H_ */
