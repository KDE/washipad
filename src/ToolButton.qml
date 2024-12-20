// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick

Rectangle {
    id: root

    signal clicked

    implicitHeight: 60
    implicitWidth: implicitHeight * 1.5

    color: "lightGray"

    MouseArea {
        anchors.fill: parent
        z: 1
        onClicked: root.clicked()
    }
}
