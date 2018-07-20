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

#ifndef SKETCHVIEW_H
#define SKETCHVIEW_H

#include <QQuickView>

#include "tabletevent.h"

class SketchViewHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TabletEvent point READ point NOTIFY pointChanged)
    Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged)
public:
    explicit SketchViewHandler(QObject *parent = nullptr);

    Q_INVOKABLE Stroke createStroke(Stroke::Type type, const QColor &color) const;
    Q_INVOKABLE StrokeSample createSample(const QPointF &position, qreal width) const;

    TabletEvent point() const;
    bool isPressed() const;

signals:
    void pointChanged(const TabletEvent &point);
    void pressedChanged(bool pressed);

private slots:
    void onTabletEventReceived(QTabletEvent *event);

private:
    TabletEvent m_point;
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
};

#endif // SKETCHVIEW_H
