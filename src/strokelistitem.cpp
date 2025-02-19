// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

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

void StrokeListItem::eraseArea(const QVector2D &center, float radius)
{
    Q_ASSERT(m_model);
    m_model->eraseArea(m_type, center, radius);
    update();
}

void StrokeListItem::paint(QPainter *painter)
{
    const auto strokes = m_model->strokes(m_type);
    for (const auto &stroke : std::as_const(strokes))
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
    Q_EMIT typeChanged(type);
    update();
}

void StrokeListItem::setModel(SketchModel *model)
{
    if (m_model == model)
        return;

    m_model = model;
    Q_EMIT modelChanged(model);
    update();
}

#include "moc_strokelistitem.cpp"
