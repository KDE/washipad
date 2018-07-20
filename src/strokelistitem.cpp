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

#include "strokelistitem.h"

#include <QPainter>

#include "sketchmodel.h"
#include "strokepainter.h"

StrokeListItem::StrokeListItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
    setRenderTarget(FramebufferObject);
}

void StrokeListItem::addStroke(const Stroke &stroke)
{
    Q_ASSERT(m_model);
    Q_ASSERT(stroke.type() == m_type);
    m_model->addStroke(stroke);
    update();
}

void StrokeListItem::eraseArea(const QPointF &center, qreal radius)
{
    Q_ASSERT(m_model);
    m_model->eraseArea(m_type, center, radius);
    update();
}

void StrokeListItem::paint(QPainter *painter)
{
    const auto strokes = m_model->strokes(m_type);
    for (const auto &stroke : qAsConst(strokes))
        StrokePainter::render(stroke, painter);
}

Stroke::Type StrokeListItem::type() const
{
    return m_type;
}

SketchModel *StrokeListItem::model() const
{
    return m_model;
}

void StrokeListItem::setType(Stroke::Type type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(type);
    update();
}

void StrokeListItem::setModel(SketchModel *model)
{
    if (m_model == model)
        return;

    m_model = model;
    emit modelChanged(model);
    update();
}
