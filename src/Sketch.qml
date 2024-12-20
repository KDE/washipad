// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Window
import QtQuick.Controls


import WashiPad

Item {
    id: root
    readonly property size size: Qt.size(sketchContent.width, sketchContent.height)
    readonly property point cursorPos: Qt.point(handler.point.x, handler.point.y)

    property SketchModel model
    property color penColor: "black"
    readonly property var penType: (penColor === Qt.rgba(0, 0, 0, 1)) ? Stroke.Outline : Stroke.Fill
    property alias isMouseSupportEnabled: mouseArea.enabled

    Flickable {
        id: flickable

        anchors.fill: parent

        clip: true
        topMargin: contentHeight * 0.1
        bottomMargin: topMargin
        rightMargin: contentWidth * 0.1
        leftMargin: rightMargin

        interactive: !mouseArea.isPress

        contentWidth: sketchContent.width
        contentHeight: sketchContent.height

        PaintingCanvas {
            id: sketchContent

            StrokeListItem {
                id: fillStrokes
                anchors.fill: parent
                z: 0
                type: Stroke.Fill
                model: root.model
            }

            StrokeListItem {
                id: outlineStrokes
                anchors.fill: parent
                z: 1
                type: Stroke.Outline
                model: root.model
            }

            StrokeItem {
                id: currentStroke
                anchors.fill: parent
                z: stroke.type === Stroke.Outline ? 1 : 0
            }
        }

        ScrollBar.vertical: ScrollBar {anchors.right: flickable.right}
        ScrollBar.horizontal: ScrollBar {anchors.bottom: flickable.bottom}

        MouseArea {
            id: mouseArea

            property bool isPress: false
            property var lastButton

            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onPressed: (mouse) => {
                if (!isPress) {
                    isPress = true
                    lastButton = mouse.button

                    handler.pressed = isPress
                }
            }

            onReleased: (mouse) => {
                if (mouse.button === lastButton) {
                    isPress = false

                    handler.pressed = isPress
                }
            }

            onPositionChanged: (mouse) => {
                handler.mouseMoved(mouse.x - flickable.contentX, mouse.y - flickable.contentY, lastButton)
            }
        }
    }

    SketchViewHandler {
        id: handler

        function isEraser() {
            return point.pointer === TabletEvent.Eraser
        }

        function isEmpty(rect) {
            return rect.left === rect.right
                && rect.top === rect.bottom
        }

        function createPoint() {
            return Qt.vector2d(point.x + flickable.contentX,
                               point.y + flickable.contentY)
        }

        function addSample() {
            var sample = createSample(createPoint(), pressureEquation.width)
            currentStroke.addSample(sample)
        }

        function eraseSamples() {
            var point = createPoint()
            var radius = cursor.height / 2

            outlineStrokes.eraseArea(point, radius)
            fillStrokes.eraseArea(point, radius)
        }

        onPressedChanged: (pressed) => {
            if (isEraser())
                return

            if (!pressed && !isEmpty(currentStroke.stroke.boundingRect())) {
                addSample()
                if (currentStroke.stroke.type === Stroke.Outline)
                    outlineStrokes.addStroke(currentStroke.stroke)
                else
                    fillStrokes.addStroke(currentStroke.stroke)
                currentStroke.stroke = createStroke(penType, root.penColor)
                return
            }

            currentStroke.stroke = createStroke(penType, root.penColor)
        }

        onPointChanged: {
            if (!pressed)
                return

            if (!isEraser())
                addSample()
            else
                eraseSamples()
        }
    }

    PressureEquation {
        id: pressureEquation
        readonly property real minOutlineWidth: 2
        readonly property real maxOutlineWidth: 10

        readonly property real minFillWidth: minOutlineWidth * 2
        readonly property real maxFillWidth: maxOutlineWidth * 2

        readonly property real minEraserWidth: minOutlineWidth * 4
        readonly property real maxEraserWidth: maxOutlineWidth * 8

        pressure: handler.point.pressure
        minWidth: handler.isEraser() ? minEraserWidth
                : root.penType === Stroke.Fill ? minFillWidth
                : minOutlineWidth
        maxWidth: handler.isEraser() ? maxEraserWidth
                : root.penType === Stroke.Fill ? maxFillWidth
                : maxOutlineWidth
    }

    Rectangle {
        id: cursor

        color: "transparent"
        border.color: handler.isEraser() ? Qt.rgba(1, 0, 0, 0.75) : Qt.rgba(0, 0, 1, 0.75)
        border.width: 2
        x: handler.point.x - width / 2
        y: handler.point.y - height / 2
        width: height
        height: pressureEquation.width * 1.5
        radius: height / 2
    }
}
