import QtQuick 2.4
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.4
import Machinekit.Application 1.0

Image {
    readonly  property string activeLanguage: getLanguage()

    id: root
    fillMode: Image.PreserveAspectFit
    smooth: true
    source: "qrc:Machinekit/Application/Controls/icons/flag-" + activeLanguage
    height: dummyButton.height

    function getLanguage() {
        // uncomment the languages when supported
        var languageMap = {
          "de": "german",
          "ru": "russian",
          "es": "spanish",
          "en": "english",
          //"uk": "ukranian",
          //"it": "italian",
          //"tu": "turkish",
        }

        var language = ApplicationHelpers.currentLanguage;
        for (var key in languageMap) {
            if (language.indexOf(key)  == 0) {
                return languageMap[key];
            }
        }
        return "english";
    }

    function setLanguage(language) {
        ApplicationHelpers.setLanguage(language);
        restartDialog.open();
    }

    MouseArea {
        anchors.fill: parent
        onClicked: languageMenu.popup()
    }

    Menu {
        id: languageMenu

        MenuItem {
            text: qsTr("English")
            icon.source: "qrc:Machinekit/Application/Controls/icons/flag-english"
            checkable: true
            checked: root.activeLanguage == "english"
            ButtonGroup.group: exclusiveGroup
            onTriggered: root.setLanguage("en")
        }

        MenuItem {
            text: qsTr("German")
            icon.source: "qrc:Machinekit/Application/Controls/icons/flag-german"
            checkable: true
            checked: root.activeLanguage == "german"
            ButtonGroup.group: exclusiveGroup
            onTriggered: root.setLanguage("de")
        }

        MenuItem {
            text: qsTr("Russian")
            icon.source: "qrc:Machinekit/Application/Controls/icons/flag-russian"
            checkable: true
            checked: root.activeLanguage == "russian"
            ButtonGroup.group: exclusiveGroup
            onTriggered: root.setLanguage("ru")
        }

        MenuItem {
            text: qsTr("Spanish")
            icon.source: "qrc:Machinekit/Application/Controls/icons/flag-spanish"
            checkable: true
            checked: root.activeLanguage == "spanish"
            ButtonGroup.group: exclusiveGroup
            onTriggered: root.setLanguage("es")
        }

        ButtonGroup {
            id: exclusiveGroup
        }
    }

    MessageDialog {
        id: restartDialog
        title: qsTr("Restart Application")
        text: qsTr("For the change to take effect, you need to restart the application.\nRestart now?")
        standardButtons: Dialog.Yes | Dialog.No
        icon: StandardIcon.Question
        onYes:  ApplicationHelpers.restartApplication();
    }
}
