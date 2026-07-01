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

#include <glib.h>
#include <glib/gstdio.h>

#include "respite-rc-utils.h"

static gchar *
respite_get_cache_path(const gchar *relpath, gboolean create_dirs) {
    gchar *cache_dir;
    gchar *full_path;
    cache_dir = g_build_filename(g_get_user_cache_dir(), "respite", NULL);
    if (create_dirs)
        g_mkdir_with_parents(cache_dir, 0700);
    full_path = g_build_filename(cache_dir, relpath, NULL);
    g_free(cache_dir);
    return full_path;
}

gchar **respite_get_history_full(const gchar *relpath) {
    gchar **lines = NULL;
    gchar *history = NULL;
    gchar *contents = NULL;
    gsize length = 0;

    history = respite_get_cache_path(relpath, FALSE);

    if (history && g_file_get_contents(history, &contents, &length, NULL)) {
        lines = g_strsplit(contents, "\n", -1);
        g_free(contents);
    }

    g_free(history);

    return lines;
}

gchar **respite_get_history(void) {
    return respite_get_history_full (RESPITE_HISTORY_FILE);
}

void respite_insert_line_history(const gchar *line) {
    respite_insert_line_history_full(RESPITE_HISTORY_FILE, line);
}

void respite_insert_line_history_full(const gchar *relpath, const gchar *line) {
    gchar *history = NULL;
    FILE *f;

    history = respite_get_cache_path(relpath, TRUE);

    if (history && (f = fopen(history, "a")) != NULL) {
        fprintf(f, "%s\n", line);
        fclose(f);
    } else {
        g_warning("Unable to open cache file");
    }

    g_free(history);
}

void respite_clear_history_file(void) {
    respite_clear_history_file_full(RESPITE_HISTORY_FILE);
}

void respite_clear_history_file_full(const gchar *relpath) {
    gchar *history = NULL;

    history = respite_get_cache_path(relpath, FALSE);

    if ( history ) {
        g_unlink (history);
    }
}