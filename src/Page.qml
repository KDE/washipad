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
