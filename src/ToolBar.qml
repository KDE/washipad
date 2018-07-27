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

import QtQuick 2.0
import QtQuick.Window 2.0

import WashiPad 1.0

Row {
    id: root
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
