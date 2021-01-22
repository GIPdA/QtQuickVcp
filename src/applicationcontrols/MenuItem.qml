/*
 * Part of QtQuickVcp.
 *
 * MenuItem from Qt.Labs (supports native menu) with Action support.
 */
import QtQuick 2.12
import QtQuick.Controls 2.12
import Qt.labs.platform 1.1 as QLP

QLP.MenuItem {
    property Action action
    text: action ? action.text : ""
    //icon: action.icon // FIXME Should be in 5.15? (error is "MenuItem.icon" is not available in Qt.labs.platform 1.1.)
    checkable: action ? action.checkable : false
    checked: action ? action.checked : false
    enabled: action ? action.enabled : true
    shortcut: action ? action.shortcut : undefined
}
