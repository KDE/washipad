// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

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

StrokeSample SketchViewHandler::createSample(const QVector2D &position, float width) const
{
    return {position, width};
}

Event SketchViewHandler::point() const
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

void SketchViewHandler::setPressed(const bool pressed)
{
    if (m_pressed == pressed) {
        return;
    }
    m_pressed = pressed;
    emit pressedChanged(m_pressed);
}

void SketchViewHandler::mouseMoved(const float x, const float y, const int button)
{
    m_point = MouseEvent::create(x, y, button);
    emit pointChanged(m_point);
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
        const auto length = (event->globalPos() - m_lastGlobalPos).manhattanLength();
        constexpr auto lengthThreshold =  4.0;

        if (length < lengthThreshold)
            return;
    }

    m_lastType = event->type();
    m_lastGlobalPos = event->globalPos();
    emit tabletEventReceived(event);
}
