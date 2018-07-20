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

#ifndef STROKE_H
#define STROKE_H

#include <QColor>
#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QVector>

qreal operator ""_r(long double value);

class StrokeSample
{
    Q_GADGET
    Q_PROPERTY(QPointF position MEMBER position)
    Q_PROPERTY(qreal width MEMBER width)
public:
    QPointF position;
    qreal width = 1.0_r;

    bool operator==(const StrokeSample &other) const;
};

class Stroke
{
    Q_GADGET
    Q_PROPERTY(Stroke::Type type READ type WRITE setType)
    Q_PROPERTY(QColor color READ color WRITE setColor)
public:
    enum class Type
    {
        Outline,
        Fill
    };
    Q_ENUM(Type)

    Type type() const;
    void setType(Type type);

    QColor color() const;
    void setColor(const QColor &color);

    QVector<StrokeSample> samples() const;
    Q_INVOKABLE void addSample(const StrokeSample &sample);
    void addSamples(const QVector<StrokeSample> &samples);

    QVector<Stroke> eraseArea(const QPointF &center, qreal radius);

    Q_INVOKABLE QRectF boundingRect() const;

private:
    Type m_type = Type::Outline;
    QColor m_color = Qt::black;
    QVector<StrokeSample> m_samples;
    mutable QRectF m_boundingRectCache;
};

#endif // STROKE_H
