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

#include "stroke.h"

#include <QVector2D>

qreal operator ""_r(long double value)
{
    return static_cast<qreal>(value);
}

bool StrokeSample::operator==(const StrokeSample &other) const
{
    return position == other.position
        && qFuzzyCompare(width, other.width);
}

Stroke::Type Stroke::type() const
{
    return m_type;
}

void Stroke::setType(Stroke::Type type)
{
    m_type = type;
}

QColor Stroke::color() const
{
    return m_color;
}

void Stroke::setColor(const QColor &color)
{
    m_color = color;
}

QVector<StrokeSample> Stroke::samples() const
{
    return m_samples;
}

void Stroke::addSample(const StrokeSample &sample)
{
    m_samples << sample;
    m_boundingRectCache = QRectF{};
}

void Stroke::addSamples(const QVector<StrokeSample> &samples)
{
    m_samples += samples;
    m_boundingRectCache = QRectF{};
}

QVector<Stroke> Stroke::eraseArea(const QPointF &center, qreal radius)
{
    const auto areaBoundingRect = QRectF{center - QPointF{radius, radius},
                                         center + QPointF{radius, radius}};
    auto result = QVector<Stroke>{};

    // For sure won't get any hits
    if (!boundingRect().intersects(areaBoundingRect)) {
        result.append(*this);
        return result;
    }

    const auto isHit = [=](const StrokeSample &sample) {
        return QVector2D{sample.position - center}.length() <= static_cast<float>(radius);
    };

    auto it = std::cbegin(m_samples);
    const auto end = std::cend(m_samples);

    while (it != end) {
        auto previous = it;
        // Find first hit
        it = std::find_if(it, end, isHit);

        // Copy [previous, it) in a new stroke if there's more than one sample
        const auto distance = static_cast<int>(std::distance(previous, it));
        if (distance >= 2) {
            auto stroke = *this;
            stroke.m_samples.clear();
            stroke.m_samples.reserve(distance);
            std::copy(previous, it, std::back_inserter(stroke.m_samples));

            result.append(stroke);
        }

        // Jump to next non-hit
        it = std::find_if_not(it, end, isHit);
    }

    return result;
}

QRectF Stroke::boundingRect() const
{
    if (!m_boundingRectCache.isValid()) {
        auto minX = 0.0_r;
        auto minY = 0.0_r;
        auto maxX = 0.0_r;
        auto maxY = 0.0_r;

        for (const auto &sample : qAsConst(m_samples)) {
            const auto position = sample.position;
            if (position.x() < minX)
                minX = position.x();
            else if (position.x() > maxX)
                maxX = position.x();

            if (position.y() < minY)
                minY = position.y();
            else if (position.y() > maxY)
                maxY = position.y();
        }

        m_boundingRectCache = {QPointF{minX, minY}, QPointF{maxX, maxY}};
    }

    return m_boundingRectCache;
}
