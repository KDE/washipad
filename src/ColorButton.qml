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
