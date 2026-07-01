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

#ifndef SRC_RESPITE_MODULE_H_
#define SRC_RESPITE_MODULE_H_

#include <glib-object.h>

#include "src/misc/respite.h"

#include "src/respite-plugin-player.h"

G_BEGIN_DECLS

#define RESPITE_TYPE_PROVIDER_MODULE             (respite_provider_module_get_type () )
G_DECLARE_FINAL_TYPE(RespiteProviderModule, respite_provider_module, RESPITE, PROVIDER_MODULE, GTypeModule)

struct _RespiteProviderModule {
    GTypeModule              parent;

    GModule                *library;
    gchar                  *library_name;
    RespitePluginPlayer     *player;

    GType                   (*initialize)   (RespiteProviderModule *module);

    void                    (*shutdown)     (void);

    GType                   provider_type;
    gboolean                active;
    gpointer                instance;
    gchar                  *desktop_file;

    gulong                  use_count;
};

RespiteProviderModule       *respite_provider_module_new             (const gchar *filename,
                                                                    const gchar *desktop_file,
                                                                    const gchar *library_name);

gboolean                    respite_provider_module_new_plugin      (RespiteProviderModule *module);

void                        respite_provider_module_free_plugin     (RespiteProviderModule *module);

gboolean                    respite_provider_module_get_is_active   (RespiteProviderModule *module);

gboolean                    respite_provider_module_use             (RespiteProviderModule *module);
void                        respite_provider_module_unuse           (RespiteProviderModule *module);

G_END_DECLS

#endif /* SRC_RESPITE_MODULE_H_ */
