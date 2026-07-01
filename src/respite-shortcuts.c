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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib/gi18n.h>

#include "src/common/respite-rc-utils.h"

#include "src/common/respite-rc-utils.h"

#include "src/respite-builder.h"

#include "src/respite-shortcuts.h"

static void respite_shortcuts_finalize(GObject *object);

struct _RespiteShortcuts {
    GObject             parent;
};

G_DEFINE_TYPE(RespiteShortcuts, respite_shortcuts, G_TYPE_OBJECT)

/*
static void
respite_shortcuts_close_cb (GtkShortcutsWindow *window, RespiteShortcuts *self)
{
    gtk_widget_destroy (GTK_WIDGET (window));
}
*/

static void
respite_shortcuts_class_init(RespiteShortcutsClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = respite_shortcuts_finalize;
}

static void
respite_shortcuts_init(RespiteShortcuts *self) {
}

static void
respite_shortcuts_finalize(GObject *object) {
    G_OBJECT_CLASS(respite_shortcuts_parent_class)->finalize(object);
}

RespiteShortcuts *respite_shortcuts(GtkWidget *parent) {
    RespiteShortcuts *self;
    GtkWidget *window;
    GtkBuilder *builder;

    self = g_object_new(RESPITE_TYPE_SHORTCUTS, NULL);

    builder = respite_builder_new_from_resource("/org/gnome/respite/shortcuts.ui");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "shortcuts"));

    if ( parent )
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent));

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);

    g_signal_connect(window, "delete-event",
                      G_CALLBACK(gtk_widget_destroy), NULL);

    g_signal_connect(window, "close",
                      G_CALLBACK(gtk_widget_destroy), NULL);

    gtk_widget_show_all(window);
    g_object_unref(builder);

    return self;
}
