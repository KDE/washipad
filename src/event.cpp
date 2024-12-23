// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "event.h"

Event Event::create(const float x, const float y, const float pressure, const Pointer pointer)
{
    return {
        x,
        y,
        pressure,
        pointer
    };
}

#include "moc_event.cpp"
