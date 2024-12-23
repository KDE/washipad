// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#ifndef SKETCHSERIALIZER_H
#define SKETCHSERIALIZER_H

#include <QObject>
#include <QUrl>
#include <QSize>
#include <qqmlregistration.h>

#include "sketchmodel.h"

class SketchSerializer : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    using QObject::QObject;

    Q_INVOKABLE static void serialize(SketchModel *model, const QSize &size, const QUrl &fileUrl);
};

#endif // SKETCHSERIALIZER_H
