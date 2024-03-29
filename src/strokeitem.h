// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#ifndef STROKEITEM_H
#define STROKEITEM_H

#include <QQuickPaintedItem>

#include "stroke.h"

class StrokeItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(Stroke stroke READ stroke WRITE setStroke NOTIFY strokeChanged)
public:
    explicit StrokeItem(QQuickItem *parent = nullptr);

    Stroke stroke() const;
    void setStroke(const Stroke &stroke);

    Q_INVOKABLE void addSample(const StrokeSample &sample);

    void paint(QPainter *painter) override;

signals:
    void strokeChanged(const Stroke &stroke);

private:
    Stroke m_stroke;
};

#endif // STROKEITEM_H
