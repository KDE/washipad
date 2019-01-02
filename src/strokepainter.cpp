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

#include "strokepainter.h"

#include <QPainter>
#include <QVector2D>

#include "stroke.h"

void StrokePainter::render(const Stroke &stroke, QPainter *painter)
{
    const auto samples = stroke.samples();
    if (samples.size() < 2)
        return;

    auto it = std::cbegin(samples);
    auto lastSample = *it;
    ++it;

    auto forwardPoints = QVector<QVector2D>{};
    auto backwardPoints = QVector<QVector2D>{};

    while (it != std::cend(samples)) {
        const auto currentSample = *it;

        const auto penWidth = currentSample.width;

        const auto currentPos = currentSample.position;
        const auto lastPos = lastSample.position;

        const auto direction = currentPos - lastPos;
        const auto ortho = QVector2D(direction.y(), -direction.x()).normalized();

        const auto p1 = (lastPos - penWidth * ortho / 2.0f);
        const auto p2 = (lastPos + penWidth * ortho / 2.0f);
        const auto p3 = (currentPos - penWidth * ortho / 2.0f);
        const auto p4 = (currentPos + penWidth * ortho / 2.0f);

        forwardPoints << p1 << p3;
        backwardPoints << p2 << p4;

        lastSample = currentSample;
        ++it;
    }

    auto path = QPainterPath();
    path.moveTo(static_cast<qreal>(forwardPoints[0].x()),
                static_cast<qreal>(forwardPoints[0].y()));
    for (auto it = forwardPoints.cbegin() + 1; it != forwardPoints.cend(); ++it) {
        path.lineTo(static_cast<qreal>(it->x()),
                    static_cast<qreal>(it->y()));
    }
    for (auto it = backwardPoints.crbegin(); it != backwardPoints.crend(); ++it) {
        path.lineTo(static_cast<qreal>(it->x()),
                    static_cast<qreal>(it->y()));
    }
    path.closeSubpath();
    path.setFillRule(Qt::WindingFill);

    painter->setPen(Qt::NoPen);
    painter->setBrush(stroke.color());
    painter->drawPath(path);
}
