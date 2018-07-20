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

#include "stroke.h"

namespace
{
    QPen createPen(const Stroke &stroke, const StrokeSample &sample)
    {
        return QPen(stroke.color(),
                    sample.width,
                    Qt::SolidLine,
                    Qt::RoundCap,
                    Qt::RoundJoin);
    }
}

void StrokePainter::render(const Stroke &stroke, QPainter *painter)
{
    const auto samples = stroke.samples();
    if (samples.size() < 2)
        return;

    auto it = std::cbegin(samples);
    auto lastSample = *it;
    it++;

    while (it != std::cend(samples)) {
        const auto currentSample = *it;

        painter->setPen(createPen(stroke, currentSample));
        painter->drawLine(lastSample.position, currentSample.position);

        lastSample = currentSample;
        it++;
    }
}
