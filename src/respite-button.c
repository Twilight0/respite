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


/*
 * Based on code from gpm-button (gnome power manager)
 * Copyright (C) 2006-2007 Richard Hughes <richard@hughsie.com>
 *
 */

#ifdef HAVE_XF86_KEYSYM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/X.h>
#include <X11/XF86keysym.h>

#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#include <glib.h>
#include <glib/gi18n.h>

#include "src/misc/respite-debug.h"

#include "src/enum-gtypes.h"
#include "src/respite-button.h"

static void respite_button_finalize(GObject *object);

static struct {
    RespiteButtonKey    key;
    guint              key_code;
} respite_key_map[RESPITE_KEY_NUMBERS] = { {0, 0}, };

struct RespiteButtonPrivate {
    GdkScreen   *screen;
    GdkWindow   *window;
};

enum {
    BUTTON_PRESSED,
    LAST_SIGNAL
};

#define DUPLICATE_SHUTDOWN_TIMEOUT 4.0f

static guint signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE_WITH_PRIVATE(RespiteButton, respite_button, G_TYPE_OBJECT)

/**
 * respite_button_get_key:
 * @keycode : an #int representing a key on a keyboard
 *
 * Check if the pressed key is mapped to a function in Parole.
 **/
static guint
respite_button_get_key(unsigned int keycode) {
    RespiteButtonKey key = RESPITE_KEY_UNKNOWN;

    guint i;

    for (i = 0; i < G_N_ELEMENTS(respite_key_map); i++) {
        if ( respite_key_map[i].key_code == keycode )
            key = respite_key_map[i].key;
    }

    return key;
}

/**
 * respite_button_filter_x_events:
 * @xevent : a #GdkXEvent to filter
 * @ev     : the #GdkEvent passed by the callback function
 * @data   : user-data passed by the callback function
 *
 * Filter X events for keypresses, and pass the keypresses on to be processed.
 **/
static GdkFilterReturn
respite_button_filter_x_events(GdkXEvent *xevent, GdkEvent *ev, gpointer data) {
    RespiteButtonKey key;
    RespiteButton *button;

    XEvent *xev = (XEvent *) xevent;

    if ( xev->type != KeyPress )
        return GDK_FILTER_CONTINUE;

    key = respite_button_get_key(xev->xkey.keycode);

    if ( key != RESPITE_KEY_UNKNOWN ) {
        button = (RespiteButton *) data;

        RESPITE_DEBUG_ENUM("Key press", key, RESPITE_TYPE_BUTTON_KEY);

        g_signal_emit(G_OBJECT(button), signals[BUTTON_PRESSED], 0, key);
        return GDK_FILTER_REMOVE;
    }

    return GDK_FILTER_CONTINUE;
}

/**
 * respite_button_grab_keystring:
 * @button  : the #RespiteButton instance to handle keypresses
 * @keycode : the #int representing the pressed key on the keyboard
 *
 * Attempt to get the pressed key and modifier keys.
 *
 * Return value: %TRUE on success, else %FALSE
 **/
static gboolean
respite_button_grab_keystring(RespiteButton *button, guint keycode) {
    GdkDisplay *display;
    guint ret;
    guint modmask = 0;

    display = gdk_display_get_default();

    G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    gdk_error_trap_push();
    G_GNUC_END_IGNORE_DEPRECATIONS

    ret = XGrabKey(GDK_DISPLAY_XDISPLAY(display), keycode, modmask,
                    gdk_x11_window_get_xid(button->priv->window), True,
                    GrabModeAsync, GrabModeAsync);

    if ( ret == BadAccess ) {
        g_warning("Failed to grab modmask=%u, keycode=%li",
                    modmask, (long int) keycode);
        return FALSE;
    }

    ret = XGrabKey(GDK_DISPLAY_XDISPLAY(display), keycode, LockMask | modmask,
                    gdk_x11_window_get_xid(button->priv->window), True,
                    GrabModeAsync, GrabModeAsync);

    if (ret == BadAccess) {
        g_warning("Failed to grab modmask=%u, keycode=%li",
               LockMask | modmask, (long int) keycode);
        return FALSE;
    }

    G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    gdk_flush();
    gdk_error_trap_pop_ignored();
    G_GNUC_END_IGNORE_DEPRECATIONS

    return TRUE;
}

/**
 * respite_button_xevent_key:
 * @button : the #RespiteButton instance to handle keypresses
 * @keysym : an #int representing the keysym to be converted to a keycode
 * @key    : the #RespiteButtonKey that represents the pressed key
 *
 * Attempt to map the key and keycode to the respite_key_map.
 *
 * Return value: %TRUE on success, else %FALSE
 **/
static gboolean
respite_button_xevent_key(RespiteButton *button, guint keysym , RespiteButtonKey key) {
    guint keycode = XKeysymToKeycode(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()), keysym);

    if ( keycode == 0 ) {
        g_warning("could not map keysym %x to keycode\n", keysym);
        return FALSE;
    }

    if (!respite_button_grab_keystring(button, keycode)) {
        g_warning("Failed to grab %u\n", keycode);
        return FALSE;
    }

    RESPITE_DEBUG_ENUM_FULL(key, RESPITE_TYPE_BUTTON_KEY, "Grabbed key %li ", (long int)keycode);

    respite_key_map[key].key_code = keycode;
    respite_key_map[key].key = key;

    return TRUE;
}

/**
 * respite_button_setup:
 * @button : the #RespiteButton instance to handle keypresses
 *
 * Setup Parole's keyboard mappings.
 **/
static void
respite_button_setup(RespiteButton *button) {
    button->priv->screen = gdk_screen_get_default();
    button->priv->window = gdk_screen_get_root_window(button->priv->screen);

    respite_button_xevent_key(button, XF86XK_AudioPlay, RESPITE_KEY_AUDIO_PLAY);
    respite_button_xevent_key(button, XF86XK_AudioStop, RESPITE_KEY_AUDIO_STOP);
    respite_button_xevent_key(button, XF86XK_AudioPrev, RESPITE_KEY_AUDIO_PREV);
    respite_button_xevent_key(button, XF86XK_AudioNext, RESPITE_KEY_AUDIO_NEXT);

    gdk_window_add_filter(button->priv->window,
                            respite_button_filter_x_events, button);
}

/**
 * respite_button_class_init:
 * @klass: a #RespiteButtonClass instance
 *
 * Initialize a #RespiteButtonClass instance.
 **/
static void
respite_button_class_init(RespiteButtonClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    signals[BUTTON_PRESSED] =
        g_signal_new("button-pressed",
                      RESPITE_TYPE_BUTTON,
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET(RespiteButtonClass, button_pressed),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__ENUM,
                      G_TYPE_NONE, 1, RESPITE_TYPE_BUTTON_KEY);

    object_class->finalize = respite_button_finalize;
}

/**
 * respite_button_init:
 * @button : a #RespiteButton instance
 *
 * Initialize a #RespiteButton instance.
 **/
static void
respite_button_init(RespiteButton *button) {
    button->priv = respite_button_get_instance_private(button);

    button->priv->screen = NULL;
    button->priv->window = NULL;

    respite_button_setup(button);
}

/**
 * respite_button_finalize:
 * @object : a base #GObject to be made into a #RespiteButton object
 *
 * Finalize a #RespiteButton object.
 **/
static void
respite_button_finalize(GObject *object) {
    G_OBJECT_CLASS(respite_button_parent_class)->finalize(object);
}

/**
 * respite_button_new:
 *
 * Create a new #RespiteButton instance.
 **/
RespiteButton *
respite_button_new(void) {
    RespiteButton *button = NULL;

    button = g_object_new(RESPITE_TYPE_BUTTON, NULL);

    return button;
}

#endif /*HAVE_XF86_KEYSYM*/
