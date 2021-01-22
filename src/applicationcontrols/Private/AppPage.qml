import QtQuick 2.0

Loader {
    id: applicationLoader
    width: 600
    height: 500

    property string applicationSource: ""
    property var applicationConfig: undefined
    property var serviceDiscovery: undefined
    property string title: (active && (item !== null))
                           ? ((item.title !== undefined) ? item.title : "") : ""
    property var services: (((item !== null) && (item.services !== undefined)) ? item.services : [])
    property var toolBar: (((item !== null) && (item.toolBar !== undefined)) ? item.toolBar : null)
    property var statusBar: (((item !== null) && (item.statusBar !== undefined)) ? item.statusBar : null)

    signal goBack(bool shutdown)


    active: ((applicationSource !== "") || (applicationConfig === undefined)) ? true : applicationConfig.selectedConfig.loaded
    source: ((applicationSource !== "") || (applicationConfig === undefined)) ? applicationSource : applicationConfig.selectedConfig.mainFile

    onSourceChanged: {
        console.log("app source changed: " + source + " active: " + active);
    }

    onStatusChanged: {
        if (status === Loader.Error) {
            var msg = applicationLoader.sourceComponent.errorString();
            setError(qsTr("QML Error:"), qsTr("Loading QML file failed:\n" + msg));
        }
    }

    Connections {
        target: applicationLoader.item
        ignoreUnknownSignals: true
        onServicesChanged: serviceDiscovery.updateServices()
        onDisconnect: goBack(false)
        onShutdown: goBack(true)
    }
}

