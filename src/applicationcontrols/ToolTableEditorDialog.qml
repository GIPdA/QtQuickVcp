/****************************************************************************
**
** Copyright (C) 2017 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler <alexander AT roessler DOT systems>
**
****************************************************************************/
import QtQuick 2.4
import QtQuick.Layouts 1.4
import QtQuick.Controls 2.4
import QtQuick.Window 2.4

Dialog
{
    property alias core: editor.core
    property alias status: editor.status
    property alias command: editor.command
    property alias helper: editor.helper
    implicitWidth: container.implicitWidth
    implicitHeight: container.implicitHeight

    id: root
    title: qsTr("Tool Table Editor")

    QtObject {
        id: d
        readonly property bool ready: status.synced && command.connected
    }

    onVisibleChanged: {
        if (!visible && editor.modified) {
            editor.resetModifications();
        }
    }

    contentItem: Item {
        id: container

        ColumnLayout {
        anchors.fill: parent
        anchors.margins: Screen.pixelDensity
        enabled: d.ready

        ToolTableEditor {
            id: editor
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.fillWidth: true

            Label {
                text: editor.errored ? qsTr("Error in tool table.") : (editor.modified ? qsTr("Tool table has modifications.") : "")
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Add Row")
                icon.name: "list-add"
                onClicked: editor.addRow()
            }

            Button {
                text: qsTr("Remove Row")
                icon.name: "list-remove"
                enabled: editor.itemSelected
                onClicked: editor.removeRow()
            }

            Button {
                text: qsTr("Reset Modifications")
                icon.name: "view-refresh"
                enabled: editor.modified
                onClicked: editor.resetModifications()
            }

            Button {
                text: qsTr("Update Tool Table")
                icon.name: "dialog-ok-apply"
                enabled: !editor.errored && editor.modified
                onClicked: editor.updateToolTable()
            }

            Button {
                text: qsTr("Close")
                icon.name: "dialog-close"
                onClicked: root.close()
            }
        }
    }
    }
}
