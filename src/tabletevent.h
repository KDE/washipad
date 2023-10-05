// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

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
