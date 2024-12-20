// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Window

import WashiPad

Row {
    id: root
    property bool isMouseSupportEnabled: false
    property point cursorPos
    property color currentColor: "black"

    signal saveRequested

    height: childrenRect.height
    opacity: cursorNearToolBar() ? 0 : 1

    Behavior on opacity {
        NumberAnimation { duration: 200; easing.type: Easing.InOutQuad }
    }

    function cursorNearToolBar() {
        var fivemm = 5 * Screen.pixelDensity
        var rect = Qt.rect(root.x - fivemm, root.y - fivemm,
                           root.width + 2 * fivemm, root.height + 2 * fivemm)
        return cursorPos.x > rect.x && cursorPos.x < (rect.x + rect.width)
            && cursorPos.y > rect.y && cursorPos.y < (rect.y + rect.height)
    }

    Repeater {
        model: ["black", "gray",
                "#ee7700", "#1188cc",
                "#11aa11", "#cc3377",
                "#cc1111"]
        delegate: ColorButton {
            color: model.modelData
            active: root.currentColor === color
            onClicked: root.currentColor = color
        }
    }

    ToolButton {
        id: mouseToggler

        onClicked: root.isMouseSupportEnabled = !root.isMouseSupportEnabled

        Image {
            anchors.fill: parent
            source: root.isMouseSupportEnabled ? "qrc:/input-mouse.svg" : "qrc:/input-tablet.svg"
            fillMode: Image.PreserveAspectFit
            sourceSize: Qt.size(width, height)
        }
    }

    ToolButton {
        onClicked: root.saveRequested()

        Image {
            anchors.fill: parent
            source: "qrc:/document-save.svg"
            fillMode: Image.PreserveAspectFit
            sourceSize: Qt.size(width, height)
        }
    }

    ToolButton {
        color: "darkRed"
        onClicked: Qt.quit()

        Image {
            anchors.fill: parent
            source: "qrc:/window-close.svg"
            fillMode: Image.PreserveAspectFit
            sourceSize: Qt.size(width, height)
        }
    }
}
