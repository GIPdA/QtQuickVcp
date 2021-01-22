/*
 * Part of QtQuickVcp.
 *
 * Control to add menus into the global Menu Bar of the application.
 */
import QtQuick 2.0
import Qt.labs.platform 1.1 as Platform

// Note: beware that Menu items in QtQuick.Controls 2
// will overwrite Qt.labs.platform if imported afterwards
// and no error or warning will be raised, but menus will not be visible anymore.

Item {
    id: root
    default property list<Platform.Menu> menus

    QtObject {
        id: d

        // /!\ Sync this property with the apps
        property string globalMenuBarObjectName: "_MK_ApplicationMenuBar"

        function findRootMenuBar() {
            var p = root.parent
            while (p) {
                for (var i in p.data) {
                    var it = p.data[i]
                    if (it.objectName && it.objectName === globalMenuBarObjectName)
                        return it
                }
                p = p.parent
            }
            return null
        }

        // Insert menu list to the ApplicationWindow's MenuBar
        function insertMenus() {
            var mb = d.findRootMenuBar()
            if (!mb)
                return

            for (var m in menus) {
                var menu = menus[m]
                var title = menu.title
                mb.addMenu(menu)

                // Bug workaround, reset title otherwise it goes as "Untitled"
                menu.title = title + "_"
                menu.title = title
            }
        }
    }

    onMenusChanged: {
        d.insertMenus()
    }

    Component.onCompleted: {
        d.insertMenus()
    }
}
