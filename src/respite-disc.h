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

#ifndef SRC_RESPITE_DISC_H_
#define SRC_RESPITE_DISC_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define RESPITE_TYPE_DISC        (respite_disc_get_type () )
#define RESPITE_DISC(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), RESPITE_TYPE_DISC, RespiteDisc))
#define RESPITE_IS_DISC(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), RESPITE_TYPE_DISC))

typedef struct RespiteDiscPrivate RespiteDiscPrivate;

typedef enum {
    RESPITE_DISC_UNKNOWN,
    RESPITE_DISC_CDDA,
    RESPITE_DISC_VCD,
    RESPITE_DISC_SVCD,
    RESPITE_DISC_DVD
} RespiteDiscKind;

typedef struct {
    GObject                parent;
    RespiteDiscPrivate     *priv;
} RespiteDisc;

typedef struct {
    GObjectClass       parent_class;

    void            (*disc_selected)            (RespiteDisc *disc,
                                                 const gchar *uri,
                                                 const gchar *device);

    void            (*label_changed)            (RespiteDisc *disc,
                                                 const gchar *label_changed);

} RespiteDiscClass;

GType               respite_disc_get_type        (void) G_GNUC_CONST;

RespiteDisc         *respite_disc_new             (void);

G_END_DECLS

#endif /* SRC_RESPITE_DISC_H_ */
