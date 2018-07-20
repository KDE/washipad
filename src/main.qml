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
import QtQuick.Dialogs 1.0

import WashiPad 1.0

Rectangle {
    id: background
    color: Qt.rgba(0.25, 0.25, 0.25, 1.0)

    SketchModel {
        id: sketchModel
    }

    Sketch {
        id: sketch
        anchors.fill: parent
        penColor: toolBar.currentColor
        model: sketchModel
    }

    ToolBar {
        id: toolBar
        anchors.top: parent.top
        anchors.right: parent.right
        cursorPos: sketch.cursorPos
        onSaveRequested: saveDialog.open()
    }

    SketchSerializer {
        id: serializer
    }

    FileDialog {
         id: saveDialog
         title: "Save as SVG"
         folder: shortcuts.home
         selectExisting: false
         nameFilters: [ "SVG Files (*.svg)" ]
         onAccepted: serializer.serialize(sketchModel, sketch.size, fileUrl)
     }
}
