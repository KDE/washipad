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

#include "strokeitem.h"

#include <QPainter>

#include "strokepainter.h"

StrokeItem::StrokeItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
    setRenderTarget(FramebufferObject);
}

Stroke StrokeItem::stroke() const
{
    return m_stroke;
}

void StrokeItem::setStroke(const Stroke &stroke)
{
    m_stroke = stroke;
    emit strokeChanged(stroke);
    update();
}

void StrokeItem::addSample(const StrokeSample &sample)
{
    m_stroke.addSample(sample);
    emit strokeChanged(m_stroke);
    update();
}

void StrokeItem::paint(QPainter *painter)
{
    StrokePainter::render(m_stroke, painter);
}
