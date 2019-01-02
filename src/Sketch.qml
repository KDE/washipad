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

Item {
    id: root
    readonly property size size: Qt.size(sketchContent.width, sketchContent.height)
    readonly property point cursorPos: Qt.point(handler.point.x, handler.point.y)

    property SketchModel model
    property color penColor: "black"
    readonly property var penType: (penColor === Qt.rgba(0, 0, 0, 1)) ? Stroke.Outline : Stroke.Fill

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: sketchContent.width
        contentHeight: sketchContent.height

        topMargin: contentHeight * 0.1
        bottomMargin: topMargin
        rightMargin: contentWidth * 0.1
        leftMargin: rightMargin

        Page {
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
            return Qt.point(point.x + flickable.contentX,
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

        onPressedChanged: {
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
