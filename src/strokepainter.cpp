// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "strokepainter.h"

#include <QPainter>
#include <QPainterPath>
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
    for (auto backwardIt = backwardPoints.crbegin(); backwardIt != backwardPoints.crend(); ++backwardIt) {
        path.lineTo(static_cast<qreal>(backwardIt->x()),
                    static_cast<qreal>(backwardIt->y()));
    }
    path.closeSubpath();
    path.setFillRule(Qt::WindingFill);

    painter->setPen(Qt::NoPen);
    painter->setBrush(stroke.color());
    painter->drawPath(path);
}
