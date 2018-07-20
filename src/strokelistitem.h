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

#ifndef STROKELISTITEM_H
#define STROKELISTITEM_H

#include <memory>

#include <QImage>
#include <QQuickPaintedItem>

#include "stroke.h"

class SketchModel;

class StrokeListItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(Stroke::Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(SketchModel* model READ model WRITE setModel NOTIFY modelChanged)
public:
    explicit StrokeListItem(QQuickItem *parent = nullptr);

    Q_INVOKABLE void addStroke(const Stroke &stroke);
    Q_INVOKABLE void eraseArea(const QPointF &center, qreal radius);

    void paint(QPainter *painter) override;

    Stroke::Type type() const;
    SketchModel *model() const;

public slots:
    void setType(Stroke::Type type);
    void setModel(SketchModel *model);

signals:
    void typeChanged(Stroke::Type type);
    void modelChanged(SketchModel *model);

private:
    Stroke::Type m_type = Stroke::Type::Outline;
    SketchModel *m_model = nullptr;
};

#endif // STROKELISTITEM_H
