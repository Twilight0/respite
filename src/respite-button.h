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

#ifndef SRC_RESPITE_BUTTON_H_
#define SRC_RESPITE_BUTTON_H_

#include <glib-object.h>

typedef enum {
    RESPITE_KEY_UNKNOWN,
    RESPITE_KEY_AUDIO_PLAY,
    RESPITE_KEY_AUDIO_STOP,
    RESPITE_KEY_AUDIO_PREV,
    RESPITE_KEY_AUDIO_NEXT,
    RESPITE_KEY_NUMBERS,
} RespiteButtonKey;

#ifdef HAVE_XF86_KEYSYM

G_BEGIN_DECLS

#define RESPITE_TYPE_BUTTON   (respite_button_get_type () )
#define RESPITE_BUTTON(o)     (G_TYPE_CHECK_INSTANCE_CAST((o), RESPITE_TYPE_BUTTON, RespiteButton))
#define RESPITE_IS_BUTTON(o)  (G_TYPE_CHECK_INSTANCE_TYPE((o), RESPITE_TYPE_BUTTON))

typedef struct RespiteButtonPrivate RespiteButtonPrivate;

typedef struct {
    GObject                 parent;
    RespiteButtonPrivate     *priv;
} RespiteButton;

typedef struct {
    GObjectClass            parent_class;

    void                    (*button_pressed)       (RespiteButton *button,
                                                     RespiteButtonKey type);
} RespiteButtonClass;

GType                       respite_button_get_type  (void) G_GNUC_CONST;

RespiteButton               *respite_button_new       (void);

G_END_DECLS

#endif /*HAVE_XF86_KEYSYM*/

#endif /* SRC_RESPITE_BUTTON_H_ */
