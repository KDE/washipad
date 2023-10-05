// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick 2.0
import QtQuick.Window 2.0
import Qt.labs.platform 1.1

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
         title: "Save as..."
         fileMode: FileDialog.SaveFile
         folder: StandardPaths.writableLocation(StandardPaths.HomeLocation)
         nameFilters: [ "SVG Image (*.svg)", "PNG Image (*.png)" ]
         onAccepted: {
             serializer.serialize(sketchModel, sketch.size, file)
         }
     }
}
