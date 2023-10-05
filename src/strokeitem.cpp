// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

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
