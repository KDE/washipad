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

#ifndef STROKEITEM_H
#define STROKEITEM_H

#include <QQuickPaintedItem>

#include "stroke.h"

class StrokeItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(Stroke stroke READ stroke WRITE setStroke NOTIFY strokeChanged)
public:
    explicit StrokeItem(QQuickItem *parent = nullptr);

    Stroke stroke() const;
    void setStroke(const Stroke &stroke);

    Q_INVOKABLE void addSample(const StrokeSample &sample);

    void paint(QPainter *painter) override;

signals:
    void strokeChanged(const Stroke &stroke);

private:
    Stroke m_stroke;
};

#endif // STROKEITEM_H
