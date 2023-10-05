// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#ifndef SKETCHMODEL_H
#define SKETCHMODEL_H

#include <QObject>
#include <QHash>

#include "stroke.h"

class SketchModel : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    void addStroke(const Stroke &stroke);
    QVector<Stroke> strokes(Stroke::Type type) const;
    QVector<Stroke> strokes() const;

    void eraseArea(Stroke::Type type, const QVector2D &center, float radius);

private:
    const QVector<Stroke> &strokesRef(Stroke::Type type) const;
    QVector<Stroke> &strokesRef(Stroke::Type type);

    QVector<Stroke> m_fillStrokes;
    QVector<Stroke> m_outlineStrokes;
};

#endif // SKETCHMODEL_H
