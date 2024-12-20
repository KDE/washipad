// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick

ToolButton {
    id: root
    property bool active: false
    property color color: "black"

    Rectangle {
        color: root.color

        anchors.centerIn: parent
        width: height
        height: root.height * 0.6
        radius: height / 2
    }

    Rectangle {
        visible: root.active
        color: "transparent"
        border.color: root.color
        border.width: 2

        anchors.centerIn: parent
        width: height
        height: root.height * 0.8
        radius: height / 2
    }
}
