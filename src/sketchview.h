// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#pragma once

#include "event.h"
#include "stroke.h"
#include <qqmlregistration.h>

class SketchViewHandler : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Event point READ point NOTIFY pointChanged)
    Q_PROPERTY(bool pressed READ isPressed WRITE setPressed NOTIFY pressedChanged)
public:
    explicit SketchViewHandler(QObject *parent = nullptr);

    Q_INVOKABLE Stroke createStroke(Stroke::Type type, const QColor &color) const;
    Q_INVOKABLE StrokeSample createSample(const QVector2D &position, float width) const;
    Q_INVOKABLE void setPressed(const bool pressed);
    Q_INVOKABLE void mouseMoved(const float x, const float y, const float pressure, const Event::Pointer pointerType);

    Event point() const;
    bool isPressed() const;

Q_SIGNALS:
    void pointChanged(const Event &point);
    void pressedChanged(bool pressed);

private:
    Event m_point;
    bool m_active = false;
    bool m_pressed = false;
};
