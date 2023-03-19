// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick 2.0
import QtQuick.Window 2.0

Rectangle {
    id: root
    color: "white"
    width: Math.max(Screen.width, 297 * Screen.pixelDensity)
    height: Math.max(Screen.height, 210 * Screen.pixelDensity)

    PageMarker {
        anchors.centerIn: parent
        markerRadius: 1
    }

    PageMarker {
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.top
    }

    PageMarker {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.top
    }

    PageMarker {
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.top
    }

    PageMarker {
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.verticalCenter
    }

    PageMarker {
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.bottom
    }

    PageMarker {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.bottom
    }

    PageMarker {
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.bottom
    }

    PageMarker {
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.verticalCenter
    }
}
