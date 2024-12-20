// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "sketchview.h"

SketchViewHandler::SketchViewHandler(QObject *parent)
    : QObject(parent)
{
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

void SketchViewHandler::setPressed(const bool pressed)
{
    if (m_pressed == pressed) {
        return;
    }
    m_pressed = pressed;
    Q_EMIT pressedChanged(m_pressed);
}

void SketchViewHandler::mouseMoved(const float x, const float y, float presure, const Event::Pointer pointerType)
{
    m_point = Event::create(x, y, presure, pointerType);
    Q_EMIT pointChanged(m_point);
}
