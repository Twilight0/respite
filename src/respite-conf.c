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

#include "src/gst/respite-gst.h"
#include "src/gst/gst-enum-types.h"

#include "src/enum-gtypes.h"

#include "src/respite-conf.h"
#include "src/misc/respite-debug.h"
#include "src/gst/respite-gst.h"


static gpointer respite_conf_object = NULL;

/* Property identifiers */
enum {
    PROP_0,
    /* Audio */
    PROP_VIS_ENABLED,
    PROP_VIS_NAME,
    PROP_VOLUME,
    /* Folders */
    PROP_MEDIA_CHOOSER_FOLDER,
    /* Parole General */
    PROP_MULTIMEDIA_KEYS,
    PROP_PLUGINS,
    PROP_SCAN_FOLDER_RECURSIVELY,
    /* Playlist */
    PROP_REMEMBER_PLAYLIST,
    PROP_REMOVE_DUPLICATED_PLAYLIST_ENTRIES,
    PROP_REPEAT,
    PROP_REPLACE_PLAYLIST,
    PROP_SHUFFLE,
    PROP_START_PLAYING_OPENED_FILES,
    PROP_PLAYLIST_POSITION,
    /* Subtitles */
    PROP_SUBTITLE_ENABLED,
    PROP_SUBTITLE_ENCODING,
    PROP_SUBTITLE_FONT,
    /* Video */
    PROP_VIDEO_SINK,
    PROP_ASPECT_RATIO,
    PROP_BRIGHTNESS,
    PROP_CONTRAST,
    PROP_DISABLE_SCREEN_SAVER,
    PROP_HUE,
    PROP_SATURATION,
    /* Window properties */
    PROP_WINDOW_HEIGHT,
    PROP_WINDOW_MAXIMIZED,
    PROP_WINDOW_MINIMIZED,
    PROP_WINDOW_WIDTH,
    PROP_HIDE_CONTROLS_TIMEOUT,
    PROP_ALWAYS_HIDE_MENUBAR,
    N_PROPS
};


struct _RespiteConf {
    GObject parent_instance;
    GSettings *settings;
    gulong     changed_id;
};


G_DEFINE_TYPE(RespiteConf, respite_conf, G_TYPE_OBJECT)


static void respite_conf_finalize(GObject *object);
static void respite_conf_get_property(GObject *object,
                                     guint prop_id,
                                     GValue *value,
                                     GParamSpec *pspec);
static void respite_conf_set_property(GObject *object,
                                     guint prop_id,
                                     const GValue *value,
                                     GParamSpec *pspec);


static gboolean
is_bool_key(const gchar *key) {
    return g_strcmp0(key, "vis-enabled") == 0 || g_strcmp0(key, "multimedia-keys") == 0 ||
           g_strcmp0(key, "scan-recursive") == 0 || g_strcmp0(key, "remember-playlist") == 0 ||
           g_strcmp0(key, "remove-duplicated") == 0 || g_strcmp0(key, "repeat") == 0 ||
           g_strcmp0(key, "replace-playlist") == 0 || g_strcmp0(key, "shuffle") == 0 ||
           g_strcmp0(key, "play-opened-files") == 0 || g_strcmp0(key, "enable-subtitle") == 0 ||
           g_strcmp0(key, "reset-saver") == 0 || g_strcmp0(key, "minimized") == 0 ||
           g_strcmp0(key, "window-maximized") == 0 || g_strcmp0(key, "always-hide-menubar") == 0;
}

static gboolean
is_int_key(const gchar *key) {
    return g_strcmp0(key, "volume") == 0 || g_strcmp0(key, "brightness") == 0 ||
           g_strcmp0(key, "contrast") == 0 || g_strcmp0(key, "hue") == 0 ||
           g_strcmp0(key, "saturation") == 0 || g_strcmp0(key, "window-height") == 0 ||
           g_strcmp0(key, "window-width") == 0 || g_strcmp0(key, "hide-controls-timeout") == 0;
}

static gboolean
is_enum_key(const gchar *key) {
    return g_strcmp0(key, "aspect-ratio") == 0;
}

static void
respite_conf_set_property(GObject *object,
                         guint prop_id,
                         const GValue *value,
                         GParamSpec *pspec) {
    RespiteConf *conf = RESPITE_CONF(object);
    const gchar *key = g_param_spec_get_nick(pspec);

    if (!conf->settings)
        return;

    g_signal_handler_disconnect(conf->settings, conf->changed_id);

    if (is_bool_key(key))
        g_settings_set_boolean(conf->settings, key, g_value_get_boolean(value));
    else if (is_int_key(key))
        g_settings_set_int(conf->settings, key, g_value_get_int(value));
    else if (is_enum_key(key))
        g_settings_set_enum(conf->settings, key, g_value_get_enum(value));
    else
        g_settings_set_string(conf->settings, key, g_value_get_string(value));

    conf->changed_id = g_signal_connect(conf->settings, "changed",
                                        G_CALLBACK(g_object_notify), conf);

    g_object_notify(G_OBJECT(conf), key);
}

static void
respite_conf_get_property(GObject *object,
                         guint prop_id,
                         GValue *value,
                         GParamSpec *pspec) {
    RespiteConf *conf = RESPITE_CONF(object);
    const gchar *key = g_param_spec_get_nick(pspec);

    if (!conf->settings) {
        return;
    }

    if (is_bool_key(key))
        g_value_set_boolean(value, g_settings_get_boolean(conf->settings, key));
    else if (is_int_key(key))
        g_value_set_int(value, g_settings_get_int(conf->settings, key));
    else if (is_enum_key(key))
        g_value_set_enum(value, g_settings_get_enum(conf->settings, key));
    else
        g_value_set_string(value, g_settings_get_string(conf->settings, key));
}

static void
respite_conf_finalize(GObject *object) {
    RespiteConf *conf = RESPITE_CONF(object);

    g_signal_handler_disconnect(conf->settings, conf->changed_id);
    g_object_unref(conf->settings);

    G_OBJECT_CLASS(respite_conf_parent_class)->finalize(object);
}

static void
respite_conf_class_init(RespiteConfClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = respite_conf_finalize;
    object_class->get_property = respite_conf_get_property;
    object_class->set_property = respite_conf_set_property;

    /**
     * RespiteConf:vis-enabled:
     **/
    g_object_class_install_property(object_class,
                                    PROP_VIS_ENABLED,
                                    g_param_spec_boolean("vis-enabled",
                                    NULL,
                                    NULL,
                                    FALSE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:vis-name:
     **/
    g_object_class_install_property(object_class,
                                    PROP_VIS_NAME,
                                    g_param_spec_string("vis-name",
                                    NULL,
                                    NULL,
                                    "none",
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:volume:
     **/
    g_object_class_install_property(object_class,
                                    PROP_VOLUME,
                                    g_param_spec_int("volume",
                                    NULL,
                                    NULL,
                                    0,
                                    100,
                                    50,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:media-chooser-folder:
     **/
    g_object_class_install_property(object_class,
                                    PROP_MEDIA_CHOOSER_FOLDER,
                                    g_param_spec_string("media-chooser-folder",
                                    NULL,
                                    NULL,
                                    "none",
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:multimedia-keys:
     **/
    g_object_class_install_property(object_class,
                                    PROP_MULTIMEDIA_KEYS,
                                    g_param_spec_boolean("multimedia-keys",
                                    NULL,
                                    NULL,
                                    TRUE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:plugins:
     **/
    g_object_class_install_property(object_class,
                                    PROP_PLUGINS,
                                    g_param_spec_string("plugins",
                                    NULL,
                                    NULL,
                                    "none",
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:scan-recursive:
     **/
    g_object_class_install_property(object_class,
                                    PROP_SCAN_FOLDER_RECURSIVELY,
                                    g_param_spec_boolean("scan-recursive",
                                    NULL,
                                    NULL,
                                    TRUE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:remember-playlist:
     **/
    g_object_class_install_property(object_class,
                                    PROP_REMEMBER_PLAYLIST,
                                    g_param_spec_boolean("remember-playlist",
                                    NULL,
                                    NULL,
                                    FALSE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:remove-duplicated:
     **/
    g_object_class_install_property(object_class,
                                    PROP_REMOVE_DUPLICATED_PLAYLIST_ENTRIES,
                                    g_param_spec_boolean("remove-duplicated",
                                    NULL,
                                    NULL,
                                    FALSE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:repeat:
     **/
    g_object_class_install_property(object_class,
                                    PROP_REPEAT,
                                    g_param_spec_boolean("repeat",
                                    NULL,
                                    NULL,
                                    FALSE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:replace-playlist:
     **/
    g_object_class_install_property(object_class,
                                    PROP_REPLACE_PLAYLIST,
                                    g_param_spec_boolean("replace-playlist",
                                    NULL,
                                    NULL,
                                    FALSE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:shuffle:
     **/
    g_object_class_install_property(object_class,
                                    PROP_SHUFFLE,
                                    g_param_spec_boolean("shuffle",
                                    NULL,
                                    NULL,
                                    FALSE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:play-opened-files:
     **/
    g_object_class_install_property(object_class,
                                    PROP_START_PLAYING_OPENED_FILES,
                                    g_param_spec_boolean("play-opened-files",
                                    NULL,
                                    NULL,
                                    TRUE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:enable-subtitle:
     **/
    g_object_class_install_property(object_class,
                                    PROP_SUBTITLE_ENABLED,
                                    g_param_spec_boolean("enable-subtitle",
                                    NULL,
                                    NULL,
                                    TRUE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:subtitle-encoding:
     **/
    g_object_class_install_property(object_class,
                                    PROP_SUBTITLE_ENCODING,
                                    g_param_spec_string("subtitle-encoding",
                                    NULL,
                                    NULL,
                                    "UTF-8",
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:subtitle-font:
     **/
    g_object_class_install_property(object_class,
                                    PROP_SUBTITLE_FONT,
                                    g_param_spec_string("subtitle-font",
                                    NULL,
                                    NULL,
                                    "Sans Bold 20",
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:videosink:
     **/
    g_object_class_install_property(object_class,
                                    PROP_VIDEO_SINK,
                                    g_param_spec_string("videosink",
                                    NULL,
                                    NULL,
                                    "autoimagesink",
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:aspect-ratio:
     **/
    g_object_class_install_property(object_class,
                                    PROP_ASPECT_RATIO,
                                    g_param_spec_enum("aspect-ratio",
                                    NULL,
                                    NULL,
                                    RESPITE_TYPE_ASPECT_RATIO,
                                    RESPITE_ASPECT_RATIO_AUTO,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:brightness:
     **/
    g_object_class_install_property(object_class,
                                    PROP_BRIGHTNESS,
                                    g_param_spec_int("brightness",
                                    NULL,
                                    NULL,
                                    -1000,
                                    1000,
                                    0,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:contrast:
     **/
    g_object_class_install_property(object_class,
                                    PROP_CONTRAST,
                                    g_param_spec_int("contrast",
                                    NULL,
                                    NULL,
                                    -1000,
                                    1000,
                                    0,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:reset-saver:
     **/
    g_object_class_install_property(object_class,
                                    PROP_DISABLE_SCREEN_SAVER,
                                    g_param_spec_boolean("reset-saver",
                                    NULL,
                                    NULL,
                                    TRUE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:hue:
     **/
    g_object_class_install_property(object_class,
                                    PROP_HUE,
                                    g_param_spec_int("hue",
                                    NULL,
                                    NULL,
                                    -1000,
                                    1000,
                                    0,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:saturation:
     **/
    g_object_class_install_property(object_class,
                                    PROP_SATURATION,
                                    g_param_spec_int("saturation",
                                    NULL,
                                    NULL,
                                    -1000,
                                    1000,
                                    0,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:window-height:
     **/
    g_object_class_install_property(object_class,
                                    PROP_WINDOW_HEIGHT,
                                    g_param_spec_int("window-height",
                                    NULL,
                                    NULL,
                                    1,
                                    G_MAXINT16,
                                    420,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:minimized:
     **/
    g_object_class_install_property(object_class,
                                    PROP_WINDOW_MINIMIZED,
                                    g_param_spec_boolean("minimized",
                                    NULL,
                                    NULL,
                                    FALSE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:maximized:
     **/
    g_object_class_install_property(object_class,
                                    PROP_WINDOW_MAXIMIZED,
                                    g_param_spec_boolean("window-maximized",
                                    NULL,
                                    NULL,
                                    FALSE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:window-width:
     **/
    g_object_class_install_property(object_class,
                                    PROP_WINDOW_WIDTH,
                                    g_param_spec_int("window-width",
                                    NULL,
                                    NULL,
                                    1,
                                    G_MAXINT16,
                                    760,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:hide-controls-timeout:
     **/
    g_object_class_install_property(object_class,
                                    PROP_HIDE_CONTROLS_TIMEOUT,
                                    g_param_spec_int("hide-controls-timeout",
                                    NULL,
                                    NULL,
                                    1,
                                    G_MAXINT16,
                                    2,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:always-hide-menubar:
     **/
    g_object_class_install_property(object_class,
                                    PROP_ALWAYS_HIDE_MENUBAR,
                                    g_param_spec_boolean("always-hide-menubar",
                                    NULL,
                                    NULL,
                                    FALSE,
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * RespiteConf:playlist-position:
     **/
    g_object_class_install_property(object_class,
                                    PROP_PLAYLIST_POSITION,
                                    g_param_spec_string("playlist-position",
                                    NULL,
                                    NULL,
                                    "bottom",
                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}

static void
respite_conf_init(RespiteConf *conf) {
    conf->settings = g_settings_new("org.gnome.Respite");

    conf->changed_id = g_signal_connect(conf->settings,
                                        "changed",
                                        G_CALLBACK(g_object_notify),
                                        conf);
}

/**
 * respite_conf_new:
 *
 * Create a new #RespiteConf instance
 **/
RespiteConf *
respite_conf_new(void) {
    if ( respite_conf_object != NULL ) {
        g_object_ref(respite_conf_object);
    } else {
        respite_conf_object = g_object_new(RESPITE_TYPE_CONF, NULL);
        g_object_add_weak_pointer(respite_conf_object, &respite_conf_object);
    }

    return RESPITE_CONF (respite_conf_object);
}

/**
 * respite_conf_get_property_bool:
 * @conf : a #RespiteConf instance
 * @name : the name of the property being retrieved
 *
 * Return a boolean value from a property.
 **/
gboolean
respite_conf_get_property_bool(RespiteConf *conf, const gchar *name) {
    gboolean value;

    g_object_get(G_OBJECT(conf),
                 name, &value,
                 NULL);

    return value;
}

void
respite_conf_write_entry_list(RespiteConf *conf, const gchar *name, gchar **value) {
    gchar *value_string = "";
    guint num = 0, i, count = 0;

    TRACE("START");

    num = g_strv_length(value);
    for ( i = 0; i < num; i++ ) {
        if (value[i] && g_strcmp0(value[i], "") != 0 && g_strcmp0(value[i], "none") != 0) {
            if (count == 0)
                value_string = g_strdup(value[i]);
            else
                value_string = g_strconcat(value_string, ";", value[i], NULL);
            count++;
        }
    }

    g_object_set(G_OBJECT(conf),
                  name, value_string,
                  NULL);

    if (count > 0)
        g_free(value_string);
}

gchar**
respite_conf_read_entry_list(RespiteConf *conf, const gchar *name) {
    gchar *value_string;
    gchar **ret_val = NULL;

    TRACE("START");

    g_object_get(G_OBJECT(conf),
                  name, &value_string,
                  NULL);

    if ( g_strcmp0(value_string, "") == 0 )
        return ret_val;

    ret_val = g_strsplit(value_string, ";", 0);

    return ret_val;
}
