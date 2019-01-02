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

#include "pressureequation.h"

#include <cmath>
#include <QDebug>

namespace {
    float sigmoid(float x)
    {
        const auto lambda = 5.0f;
        return 1.0f / (1.0f + std::exp(-lambda * x));
    }

    float computeWidth(float minWidth, float maxWidth, float pressure)
    {
        const auto alpha = sigmoid(2.0f * pressure - 1.0f);
        return (1.0f - alpha) * minWidth + alpha * maxWidth;
    }
}

float PressureEquation::minWidth() const
{
    return m_minWidth;
}

float PressureEquation::maxWidth() const
{
    return m_maxWidth;
}

float PressureEquation::pressure() const
{
    return m_pressure;
}

float PressureEquation::width() const
{
    return m_width;
}

void PressureEquation::setMinWidth(float minWidth)
{
    if (qFuzzyCompare(m_minWidth, minWidth))
        return;

    m_minWidth = minWidth;
    emit minWidthChanged(minWidth);
    updateWidth();
}

void PressureEquation::setMaxWidth(float maxWidth)
{
    if (qFuzzyCompare(m_maxWidth, maxWidth))
        return;

    m_maxWidth = maxWidth;
    emit maxWidthChanged(maxWidth);
    updateWidth();
}

void PressureEquation::setPressure(float pressure)
{
    if (qFuzzyCompare(m_pressure, pressure))
        return;

    m_pressure = pressure;
    emit pressureChanged(pressure);
    updateWidth();
}

void PressureEquation::updateWidth()
{
    m_width = computeWidth(m_minWidth, m_maxWidth, m_pressure);
    emit widthChanged(m_width);
}
