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

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.1 as Platform
import Qt.labs.settings 1.0

import Machinekit.Application 1.0
import Machinekit.Application.Controls 1.0

Item {
    id: root

    property string title: qsTr("QtQuickVcp Live Coding") + (loader.item ? " - " + loader.item.title : "")
    property alias item: loader.item
    readonly property var statusBar: (((item !== null) && (item.statusBar !== undefined)) ? item.statusBar : null)
    readonly property var toolBar: (((item !== null) && (item.toolBar !== undefined)) && !hideToolbar.checked ? item.toolBar : null)
    readonly property var services: (((item !== null) && (item.services !== undefined)) ? loader.item.services : [])

    // Disconnects this page
    signal disconnect()


    QtObject {
        id: d

        property string loadedFile
        property string loadedFolder: !loadedFile ? "" : loadedFile.substr(0, loadedFile.lastIndexOf('/'))

        function loadQmlFile(file) {
            loadedFile = file;
            settings.lastFileName = file;
            loader.source = "";
            ApplicationHelpers.clearQmlComponentCache();
            loader.source = file;
        }

        function reload() {
            loadQmlFile(loadedFile);
        }

        function openWithSystemEditor() {
            ApplicationHelpers.openUrlWithDefaultApplication(loadedFile);
        }
    }

    Settings {
        id: settings
        category: "liveCoding"
        property string lastFileName: Platform.StandardPaths.writableLocation(Platform.StandardPaths.HomeLocation)
    }

    Label {
        id: dummyText
        visible: false
    }


    ApplicationMenuBar {
       Platform.Menu {
            title: qsTr("&Live Coding")
            /*MenuItem {
                text: qsTr("CheckTest")
                checkable: true
            }//*/

            Platform.MenuItem {
                text: qsTr("&Reload")
                enabled: loader.source != ""
                onTriggered: d.reload()
            }
            Platform.MenuItem {
                text: qsTr("&Open...")
                onTriggered: fileDialog.open()
            }
            Platform.Menu {
                title: qsTr("Recent files")
                // TODO

                Platform.MenuSeparator {}
                Platform.MenuItem {
                    text: qsTr("Clear menu")
                    //onTriggered:
                }
            }
            Platform.MenuItem {
                text: qsTr("&Edit...")
                enabled: fileDialog.file != ""
                onTriggered: d.openWithSystemEditor()
            }
            Platform.MenuSeparator {}
            Platform.MenuItem {
                text: qsTr("&Exit Live Coding")
                onTriggered: root.disconnect()
            }
            Platform.MenuItem {
                id: hideToolbar
                text: qsTr("&Hide Tool Bar")
                checkable: true
                checked: false
            }
        }
    }


    ColumnLayout {
        anchors.fill: parent

        /*RowLayout {
            Button {
                text: qsTr("Open")
                onClicked: fileDialog.open()
            }

            Button {
                text: qsTr("Edit")
                onClicked: d.openWithSystemEditor()
            }

            Button {
                text: qsTr("Reload")
                onClicked: d.reload()
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Disconnect")
                onClicked: root.disconnect()
            }
        }//*/

        Loader {
            id: loader
            Layout.fillWidth: true
            Layout.fillHeight: true

            onStatusChanged: {
                if (status == Loader.Ready) {
                    item.disconnect.connect(root.disconnect)
                    item.shutdown.connect(root.disconnect)
                }

                if (status == Loader.Error) {
                    var msg = loader.sourceComponent.errorString();
                    errorLabel.text = qsTr("QML Error: Loading QML file failed:\n") + msg;
                }
            }

            Label {
                id: errorLabel
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                font.pointSize: dummyText.font.pointSize * 1.1
                visible: loader.status === Loader.Error
            }

            Column {
                anchors.centerIn: parent
                visible: loader.status === Loader.Null

                Button {
                    anchors.horizontalCenter: parent.horizontalCenter
                    padding: 30
                    text: qsTr("Open File...")
                    onClicked: fileDialog.open()
                }

                Button {
                    anchors.horizontalCenter: parent.horizontalCenter
                    padding: 30
                    text: qsTr("Open last file:\n"+settings.lastFileName)
                    visible: !!(settings.lastFileName)
                    onClicked: {
                        d.loadQmlFile(settings.lastFileName)
                    }
                }
            }

        }
    }

    Platform.FileDialog {
        property bool selected: false
        id: fileDialog
        folder: settings.lastFileName
        nameFilters: [qsTr("QML files (*.qml)")]
        onFileChanged: d.reload()
        onAccepted: d.loadQmlFile(file)
    }

    FileWatcher {
        id: fileWatcher
        fileUrl: d.loadedFolder
        recursive: true
        onFileChanged: d.reload()
        nameFilters: [
            "*.qml"
        ]
    }
}
