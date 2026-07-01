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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib/gi18n.h>
#include "src/misc/respite-debug.h"

#include "src/misc/respite-provider-plugin.h"

#include "src/respite-module.h"


static void     respite_provider_module_plugin_init(RespiteProviderPluginIface  *iface);

G_DEFINE_TYPE_WITH_CODE(RespiteProviderModule, respite_provider_module, G_TYPE_TYPE_MODULE,
                        G_IMPLEMENT_INTERFACE(RESPITE_TYPE_PROVIDER_PLUGIN, respite_provider_module_plugin_init))

static gboolean
respite_module_load(GTypeModule *gtype_module) {
    RespiteProviderModule *module;

    module = RESPITE_PROVIDER_MODULE(gtype_module);

    module->library = g_module_open(gtype_module->name, G_MODULE_BIND_LOCAL);

    if (G_UNLIKELY(module->library == NULL)) {
        g_critical("Failed to load plugin : %s", g_module_error());
        return FALSE;
    }

    if ( !g_module_symbol(module->library, "respite_plugin_initialize", (gpointer)&module->initialize) ||
         !g_module_symbol(module->library, "respite_plugin_shutdown", (gpointer)&module->shutdown)) {
        g_critical("Plugin %s missing required symbols", gtype_module->name);
        g_module_close(module->library);
        return FALSE;
    }

    TRACE("Loading module %s", gtype_module->name);

    module->provider_type = (*module->initialize) (module);

    TRACE("Finished loading module %s", gtype_module->name);

    return TRUE;
}

static void
respite_module_unload(GTypeModule *gtype_module) {
    RespiteProviderModule *module;

    module = RESPITE_PROVIDER_MODULE(gtype_module);

    TRACE("Unloading module %s", gtype_module->name);

    (*module->shutdown) ();

    g_module_close(module->library);
    module->initialize = NULL;
    module->shutdown = NULL;
    module->library = NULL;
    module->provider_type = G_TYPE_INVALID;
    module->use_count = 0;
    module->active = FALSE;
}

gboolean respite_provider_module_use(RespiteProviderModule *module) {
    if (g_type_module_use(G_TYPE_MODULE(module))) {
        module->use_count++;
        module->active = TRUE;
        return TRUE;
    }
    return FALSE;
}

void respite_provider_module_unuse(RespiteProviderModule *module) {
    module->use_count--;
    if (module->use_count == 0) {
        module->active = FALSE;
    }
}


static void
respite_provider_module_class_init(RespiteProviderModuleClass *klass) {
    GTypeModuleClass *gtype_module_class;

    gtype_module_class = G_TYPE_MODULE_CLASS(klass);

    gtype_module_class->load   = respite_module_load;
    gtype_module_class->unload = respite_module_unload;
}

static gboolean
respite_provider_module_get_is_configurable(RespiteProviderPlugin *plugin) {
    RespiteProviderModule *module;

    module = RESPITE_PROVIDER_MODULE(plugin);

    if ( module->instance )
        return respite_provider_plugin_get_is_configurable (module->instance);

    return FALSE;
}

static void
respite_provider_module_configure(RespiteProviderPlugin *plugin, GtkWidget *parent) {
    RespiteProviderModule *module;

    module = RESPITE_PROVIDER_MODULE(plugin);

    if ( module->instance )
        respite_provider_plugin_configure(module->instance, parent);
}

static void
respite_provider_module_plugin_init(RespiteProviderPluginIface *iface) {
    iface->get_is_configurable = respite_provider_module_get_is_configurable;
    iface->configure = respite_provider_module_configure;
}

static void
respite_provider_module_init(RespiteProviderModule *module) {
    module->library = NULL;
    module->library_name = NULL;
    module->initialize = NULL;
    module->shutdown = NULL;
    module->active = FALSE;
    module->instance = NULL;
    module->desktop_file = NULL;
    module->provider_type = G_TYPE_INVALID;

    module->player = NULL;
}

RespiteProviderModule *
respite_provider_module_new(const gchar *filename, const gchar *desktop_file, const gchar *library_name) {
    RespiteProviderModule *module = NULL;

    module = g_object_new(RESPITE_TYPE_PROVIDER_MODULE, NULL);

    g_type_module_set_name(G_TYPE_MODULE(module), filename);

    module->desktop_file = g_strdup(desktop_file);
    g_object_set_data_full(G_OBJECT(module), "desktop-file",
                 module->desktop_file, (GDestroyNotify) g_free);

    module->library_name = g_strdup(library_name);
    g_object_set_data_full(G_OBJECT(module), "library-name",
                 module->library_name, (GDestroyNotify) g_free);

    return module;
}

/**
 * respite_provider_module_new_plugin:
 * @module : The #RespiteProviderModule that is being initialized
 *
 * Initialize the #RespiteProviderModule plugin. Return #TRUE if successful.
 **/
gboolean respite_provider_module_new_plugin(RespiteProviderModule *module) {
    TRACE("start");

    g_return_val_if_fail(RESPITE_IS_PROVIDER_MODULE(module), FALSE);

#ifdef debug
    g_return_val_if_fail(module->active == TRUE, FALSE);
    g_return_val_if_fail(module->instance == NULL, FALSE);
    g_return_val_if_fail(module->player == NULL, FALSE);
#endif

    module->instance = g_object_new(module->provider_type, NULL);
    g_return_val_if_fail(RESPITE_IS_PROVIDER_PLUGIN(RESPITE_PROVIDER_PLUGIN(module->instance)), FALSE);

    module->player = respite_plugin_player_new();
    respite_provider_plugin_set_player(RESPITE_PROVIDER_PLUGIN(module->instance), RESPITE_PROVIDER_PLAYER(module->player));

    return TRUE;
}

void respite_provider_module_free_plugin(RespiteProviderModule *module) {
    TRACE("start");

    g_return_if_fail(RESPITE_IS_PROVIDER_MODULE(module));

    g_clear_object(&module->instance);
    g_clear_object(&module->player);
}

gboolean
respite_provider_module_get_is_active(RespiteProviderModule *module) {
    g_return_val_if_fail(RESPITE_IS_PROVIDER_MODULE(module), FALSE);

    return module->active;
}
