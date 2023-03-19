// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "event.h"

#include <QTabletEvent>
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
#include <QPointingDevice>
#endif

Event TabletEvent::create(QTabletEvent *event)
{
    return {
        static_cast<float>(event->posF().x()),
        static_cast<float>(event->posF().y()),
        static_cast<float>(event->pressure()),
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
        (event->pointerType() == QPointingDevice::PointerType::Eraser) ? Pointer::Eraser : Pointer::Pen
#else
        (event->pointerType() == QTabletEvent::Eraser) ? Pointer::Eraser : Pointer::Pen
#endif
    };
}

Event MouseEvent::create(const float x, const float y, const int button)
{
    return {
        static_cast<float>(x),
        static_cast<float>(y),
        static_cast<float>(0.5f),
        (button == Qt::RightButton) ? Pointer::Eraser : Pointer::Pen
    };
}
