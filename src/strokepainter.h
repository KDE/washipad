// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#ifndef STROKEPAINTER_H
#define STROKEPAINTER_H

class QPainter;
class Stroke;

namespace StrokePainter
{
    void render(const Stroke &stroke, QPainter *painter);
};

#endif // STROKEPAINTER_H
