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

#ifndef PRESSUREEQUATION_H
#define PRESSUREEQUATION_H

#include <QObject>

#include "stroke.h"

class PressureEquation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal minWidth READ minWidth WRITE setMinWidth NOTIFY minWidthChanged)
    Q_PROPERTY(qreal maxWidth READ maxWidth WRITE setMaxWidth NOTIFY maxWidthChanged)

    Q_PROPERTY(qreal pressure READ pressure WRITE setPressure NOTIFY pressureChanged)
    Q_PROPERTY(qreal width READ width NOTIFY widthChanged)
public:
    using QObject::QObject;

    qreal minWidth() const;
    qreal maxWidth() const;

    qreal pressure() const;
    qreal width() const;

public slots:
    void setMinWidth(qreal minWidth);
    void setMaxWidth(qreal maxWidth);

    void setPressure(qreal pressure);

signals:
    void minWidthChanged(qreal minWidth);
    void maxWidthChanged(qreal maxWidth);

    void pressureChanged(qreal pressure);
    void widthChanged(qreal width);

private:
    void updateWidth();

    qreal m_minWidth = 1.0_r;
    qreal m_maxWidth = 1.0_r;

    qreal m_pressure = 0.0_r;
    qreal m_width = m_maxWidth;
};

#endif // PRESSUREEQUATION_H
