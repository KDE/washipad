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

#include "sketchmodel.h"

void SketchModel::addStroke(const Stroke &stroke)
{
    strokesRef(stroke.type()).append(stroke);
}

QVector<Stroke> SketchModel::strokes(Stroke::Type type) const
{
    return strokesRef(type);
}

QVector<Stroke> SketchModel::strokes() const
{
    return m_fillStrokes + m_outlineStrokes;
}

void SketchModel::eraseArea(Stroke::Type type, const QPointF &center, qreal radius)
{
    auto &strokes = strokesRef(type);

    auto it = std::begin(strokes);
    auto end = std::end(strokes);

    while (it != end) {
        const auto newStrokes = it->eraseArea(center, radius);

        if (newStrokes.size() == 0) {
            // Remove
            it = strokes.erase(it);
            end = std::end(strokes);

        } else if (newStrokes.size() == 1) {
            const auto newStroke = newStrokes.first();
            if (newStroke.samples() != it->samples()) {
                // Change
                *it = newStroke;
            }
            it++;

        } else {
            // Change + Inserts
            *it = newStrokes.first();

            auto newIt = std::cbegin(newStrokes);
            const auto newEnd = std::cend(newStrokes);

            newIt++; // Already in the model due to dataChanged above

            it++;

            while (newIt != newEnd) {
                it = strokes.insert(it, *newIt);
                end = std::end(strokes);

                newIt++;
                it++;
            }
        }
    }
}

const QVector<Stroke> &SketchModel::strokesRef(Stroke::Type type) const
{
    switch (type) {
    case Stroke::Type::Fill:
        return m_fillStrokes;
    case Stroke::Type::Outline:
        return m_outlineStrokes;
    }
    Q_UNREACHABLE();
}

QVector<Stroke> &SketchModel::strokesRef(Stroke::Type type)
{
    switch (type) {
    case Stroke::Type::Fill:
        return m_fillStrokes;
    case Stroke::Type::Outline:
        return m_outlineStrokes;
    }
    Q_UNREACHABLE();
}
