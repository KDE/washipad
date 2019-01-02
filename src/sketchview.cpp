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

#include "sketchview.h"

Q_GLOBAL_STATIC(SketchView*, s_instance);

SketchViewHandler::SketchViewHandler(QObject *parent)
    : QObject(parent)
{
    connect(SketchView::instance(), &SketchView::tabletEventReceived,
            this, &SketchViewHandler::onTabletEventReceived);
}

Stroke SketchViewHandler::createStroke(Stroke::Type type, const QColor &color) const
{
    auto result = Stroke{};
    result.setType(type);
    result.setColor(color);
    return result;
}

StrokeSample SketchViewHandler::createSample(const QPointF &position, qreal width) const
{
    return {position, width};
}

TabletEvent SketchViewHandler::point() const
{
    return m_point;
}

bool SketchViewHandler::isPressed() const
{
    return m_pressed;
}

void SketchViewHandler::onTabletEventReceived(QTabletEvent *event)
{
    m_point = TabletEvent::create(event);
    emit pointChanged(m_point);

    if (event->type() == QEvent::TabletPress && !m_pressed) {
        m_pressed = true;
        emit pressedChanged(m_pressed);
    } else if (event->type() == QEvent::TabletRelease && m_pressed) {
        m_pressed = false;
        emit pressedChanged(m_pressed);
    }
}

SketchView *SketchView::instance()
{
    return *s_instance;
}

SketchView::SketchView(QWindow *parent)
    : QQuickView(parent)
{
    if (*s_instance)
        qFatal("There must be only one SketchView instance");

    *s_instance = this;
}

void SketchView::tabletEvent(QTabletEvent *event)
{
    event->accept();

    if (m_lastType == event->type()) {
        const auto length = (event->globalPosF() - m_lastGlobalPos).manhattanLength();
        constexpr auto lengthThreshold =  4.0_r;

        if (length < lengthThreshold)
            return;
    }

    m_lastType = event->type();
    m_lastGlobalPos = event->globalPos();
    emit tabletEventReceived(event);
}
