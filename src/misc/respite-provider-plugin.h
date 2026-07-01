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

#ifndef SRC_MISC_RESPITE_PROVIDER_PLUGIN_H_
#define SRC_MISC_RESPITE_PROVIDER_PLUGIN_H_

#include <gtk/gtk.h>

#include "src/misc/respite-stream.h"
#include "src/misc/respite-provider-player.h"

G_BEGIN_DECLS

#define RESPITE_TYPE_PROVIDER_PLUGIN                 (respite_provider_plugin_get_type ())
#define RESPITE_PROVIDER_PLUGIN_GET_INTERFACE(provider) RESPITE_PROVIDER_PLUGIN_GET_IFACE(provider)
G_DECLARE_INTERFACE(RespiteProviderPlugin, respite_provider_plugin, RESPITE, PROVIDER_PLUGIN, GObject)

/**
 * RespiteProviderPluginIface:
 *
 * Interface for #RespiteProviderPlugin
 *
 * The methods of this interface should be overridden by the plugin, the Parole
 * player calls these methods to determine if the plugin is configurable, to ask
 * the plugin to open its configuration dialog or to set the
 * #RespiteProviderPlayer that the plugin can use to get access to various
 * functionalities of the player.
 *
 * Since: 0.2
 */
typedef RespiteProviderPluginInterface RespiteProviderPluginIface;

struct _RespiteProviderPluginInterface {
    GTypeInterface  __parent__;

    /*< public >*/
    gboolean     (*get_is_configurable)             (RespiteProviderPlugin *provider);

    void         (*configure)                       (RespiteProviderPlugin *provider,
                                                     GtkWidget *parent);

    void         (*set_player)                      (RespiteProviderPlugin *provider,
                                                     RespiteProviderPlayer *player);
};

gboolean
respite_provider_plugin_get_is_configurable          (RespiteProviderPlugin *provider);

void             respite_provider_plugin_configure   (RespiteProviderPlugin *provider,
                                                     GtkWidget *parent);

void             respite_provider_plugin_set_player  (RespiteProviderPlugin *provider,
                                                     RespiteProviderPlayer *player);

G_END_DECLS

#endif /* SRC_MISC_RESPITE_PROVIDER_PLUGIN_H_ */
