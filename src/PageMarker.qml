// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick 2.0
import QtQuick.Window 2.0

Rectangle {
    property real markerRadius: 5

    color: "transparent"
    border.color: Qt.rgba(0.75, 0.75, 0, 0.5)
    border.width: 2
    width: height
    height: markerRadius * Screen.pixelDensity
    radius: height / 2
}
