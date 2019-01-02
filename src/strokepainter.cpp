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

    auto forwardPoints = QVector<QPointF>{};
    auto backwardPoints = QVector<QPointF>{};

    while (it != std::cend(samples)) {
        const auto currentSample = *it;

        const auto penWidth = static_cast<float>(currentSample.width);

        const auto currentPos = QVector2D(currentSample.position);
        const auto lastPos = QVector2D(lastSample.position);

        const auto direction = currentPos - lastPos;
        const auto ortho = QVector2D(direction.y(), -direction.x()).normalized();

        const auto p1 = (lastPos - penWidth * ortho / 2.0f);
        const auto p2 = (lastPos + penWidth * ortho / 2.0f);
        const auto p3 = (currentPos - penWidth * ortho / 2.0f);
        const auto p4 = (currentPos + penWidth * ortho / 2.0f);

        forwardPoints << p1.toPointF() << p3.toPointF();
        backwardPoints << p2.toPointF() << p4.toPointF();

        lastSample = currentSample;
        ++it;
    }

    auto path = QPainterPath();
    path.moveTo(forwardPoints[0]);
    for (auto it = forwardPoints.cbegin() + 1; it != forwardPoints.cend(); ++it) {
        path.lineTo(*it);
    }
    for (auto it = backwardPoints.crbegin(); it != backwardPoints.crend(); ++it) {
        path.lineTo(*it);
    }
    path.closeSubpath();
    path.setFillRule(Qt::WindingFill);

    painter->setPen(Qt::NoPen);
    painter->setBrush(stroke.color());
    painter->drawPath(path);
}
