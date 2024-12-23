// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls as Controls
import Qt.labs.platform
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.components as Addons

import WashiPad

Kirigami.ApplicationWindow {
    id: root

    property bool isMouseSupportEnabled: false
    property color currentColor: "black"

    pageStack.initialPage: Kirigami.Page {
        id: page

        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        bottomPadding: 0

        background: Rectangle {
            id: background
            color: Qt.rgba(0.25, 0.25, 0.25, 1.0)
        }

        contentItem: Sketch {
            id: sketch
            penColor: root.currentColor
            model: sketchModel
            isMouseSupportEnabled: root.isMouseSupportEnabled
        }

        actions: [
            Kirigami.Action {
                displayComponent: RowLayout {
                    spacing: 0

                }
            },
            Controls.Action {
                text: root.isMouseSupportEnabled ? i18nc("@action:intoolbar", "Mouse Mode") : i18nc("@action:intoolbar", "Tablet Mode")
                icon.name: root.isMouseSupportEnabled ? 'input-mouse-symbolic' : 'input-tablet-symbolic'
                onTriggered: root.isMouseSupportEnabled = !root.isMouseSupportEnabled
            },
            Controls.Action {
                text: i18nc("@action:intoolbar", "Save")
                icon.name: 'document-save-symbolic'
                onTriggered: saveDialog.open()
            },
            Kirigami.Action {
                text: i18nc("@action:intoolbar", "Close")
                icon.name: 'document-save-symbolic'
                onTriggered: Qt.quit()
                visible: root.visibility === Controls.ApplicationWindow.FullScreen
            }
        ]

        Addons.FloatingToolBar {
            id: toolBar

            readonly property bool cursorNearToolBar: {
                const fivemm = 5 * Screen.pixelDensity
                const rect = Qt.rect(toolBar.x - fivemm, toolBar.y - fivemm,
                                   toolBar.width + 2 * fivemm, toolBar.height + 2 * fivemm)
                return sketch.cursorPos.x > rect.x && sketch.cursorPos.x < (rect.x + rect.width)
                    && sketch.cursorPos.y > rect.y && sketch.cursorPos.y < (rect.y + rect.height)
            }

            opacity: cursorNearToolBar ? 0 : 1
            Behavior on opacity {
                NumberAnimation { duration: Kirigami.Units.shortDuration; easing.type: Easing.InOutQuad }
            }

            parent: page.overlay
            padding: Kirigami.Units.largeSpacing
            z: 600000

            anchors {
                bottom: parent.bottom
                margins: Kirigami.Units.largeSpacing
                bottomMargin: Kirigami.Units.largeSpacing + sketch.horizontalScrollBarHeight
                horizontalCenter: parent.horizontalCenter
            }

            Controls.ButtonGroup {
                id: colorGroup
            }

            contentItem: RowLayout {
                spacing: Kirigami.Units.largeSpacing

                Repeater {
                    model: ["black", "gray", "#ee7700", "#1188cc", "#11aa11", "#cc3377", "#cc1111"]

                    Controls.AbstractButton {
                        id: button

                        required property color modelData

                        Controls.ButtonGroup.group: colorGroup

                        onClicked: root.currentColor = modelData
                        checkable: true
                        checked: root.currentColor === modelData
                        scale: checked ? 1.25 : 1

                        background: Rectangle {
                            color: button.modelData
                            radius: Kirigami.Units.cornerRadius
                            border {
                                width: button.hovered || button.checked ? 1 : 0
                                color: Kirigami.Theme.highlightColor
                            }
                        }

                        implicitHeight: Kirigami.Units.gridUnit * 2
                        implicitWidth: Kirigami.Units.gridUnit * 2
                    }
                }

            }
        }

        SketchModel {
            id: sketchModel
        }

        SketchSerializer {
            id: serializer
        }

        FileDialog {
             id: saveDialog
             title: i18nc("@title:dialog", "Save as...")
             fileMode: FileDialog.SaveFile
             folder: StandardPaths.writableLocation(StandardPaths.HomeLocation)
             nameFilters: [ "SVG Image (*.svg)", "PNG Image (*.png)" ]
             onAccepted: {
                 serializer.serialize(sketchModel, sketch.size, file)
             }
         }
     }
}
