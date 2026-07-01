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

#include <glib/gi18n.h>
#include "src/misc/respite-provider-plugin.h"

G_DEFINE_INTERFACE(RespiteProviderPlugin, respite_provider_plugin, G_TYPE_OBJECT)

static void respite_provider_plugin_default_init(RespiteProviderPluginInterface *klass) {
}

/**
 * respite_provider_plugin_get_is_configurable:
 * @provider: a #RespiteProviderPlugin
 *
 * Check if the plugin is configurable.
 *
 * Returns: TRUE if the plugin is configurable, FALSE otherwise
 *
 *
 * Since: 0.2
 **/
gboolean respite_provider_plugin_get_is_configurable(RespiteProviderPlugin *provider) {
    gboolean configurable = FALSE;

    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLUGIN(provider), FALSE);

    if (RESPITE_PROVIDER_PLUGIN_GET_INTERFACE(provider)->get_is_configurable) {
        configurable =(*RESPITE_PROVIDER_PLUGIN_GET_INTERFACE(provider)->get_is_configurable)(provider);
    }

    return configurable;
}

/**
 * respite_provider_plugin_configure:
 * @provider: a #RespiteProviderPlugin
 * @parent: a #GtkWidget parent window
 *
 * Open the plugin configuration dialog.
 *
 *
 * Since: 0.2
 **/
void respite_provider_plugin_configure(RespiteProviderPlugin *provider, GtkWidget *parent) {
    g_return_if_fail(RESPITE_IS_PROVIDER_PLUGIN(provider));

    if (RESPITE_PROVIDER_PLUGIN_GET_INTERFACE(provider)->configure) {
        (*RESPITE_PROVIDER_PLUGIN_GET_INTERFACE(provider)->configure)(provider, parent);
    }
}

/**
 * respite_provider_plugin_set_player:
 * @provider: a #RespiteProviderPlugin
 * @player: a #RespiteProviderPlayer
 *
 * The player calls this method on the iface_init funtion implemented by the plugin
 * to set the #RespiteProviderPlayer, don't take any reference of the Player.
 *
 * Since: 0.2
 **/
void respite_provider_plugin_set_player(RespiteProviderPlugin *provider, RespiteProviderPlayer *player) {
    if (RESPITE_PROVIDER_PLUGIN_GET_INTERFACE(provider)->set_player) {
        (*RESPITE_PROVIDER_PLUGIN_GET_INTERFACE(provider)->set_player)(provider, player);
    }
}
