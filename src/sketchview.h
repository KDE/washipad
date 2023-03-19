// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#ifndef SKETCHVIEW_H
#define SKETCHVIEW_H

#include <QQuickView>

#include "event.h"

class SketchViewHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Event point READ point NOTIFY pointChanged)
    Q_PROPERTY(bool pressed READ isPressed WRITE setPressed NOTIFY pressedChanged)
public:
    explicit SketchViewHandler(QObject *parent = nullptr);

    Q_INVOKABLE Stroke createStroke(Stroke::Type type, const QColor &color) const;
    Q_INVOKABLE StrokeSample createSample(const QVector2D &position, float width) const;
    Q_INVOKABLE void setPressed(const bool pressed);
    Q_INVOKABLE void mouseMoved(const float x, const float y, const int button);

    Event point() const;
    bool isPressed() const;

signals:
    void pointChanged(const Event &point);
    void pressedChanged(bool pressed);

private slots:
    void onTabletEventReceived(QTabletEvent *event);

private:
    Event m_point;
    bool m_active = false;
    bool m_pressed = false;
};

class SketchView : public QQuickView
{
    Q_OBJECT
public:
    static SketchView *instance();

    explicit SketchView(QWindow *parent = nullptr);

    void tabletEvent(QTabletEvent *event) override;

signals:
    void tabletEventReceived(QTabletEvent *event);

private:
    QPoint m_lastGlobalPos;
    QEvent::Type m_lastType = QEvent::None;
};

#endif // SKETCHVIEW_H
