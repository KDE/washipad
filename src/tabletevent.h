/* This file is part of Washi Pad

   Copyright 2018 Kevin Ottens <ervin@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
   USA.
*/

#ifndef TABLETEVENT_H
#define TABLETEVENT_H

#include "stroke.h"

class QTabletEvent;

class TabletEvent
{
    Q_GADGET
    Q_PROPERTY(float x MEMBER x)
    Q_PROPERTY(float y MEMBER y)
    Q_PROPERTY(float pressure MEMBER pressure)
    Q_PROPERTY(Pointer pointer MEMBER pointer)
public:
    enum class Pointer
    {
        Pen,
        Eraser,
    };
    Q_ENUM(Pointer)

    static TabletEvent create(QTabletEvent *event);

    float x = 0.0f;
    float y = 0.0f;
    float pressure = 0.0f;
    Pointer pointer = Pointer::Pen;
};

#endif // TABLETEVENT_H
