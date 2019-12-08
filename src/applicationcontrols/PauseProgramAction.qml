/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
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
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
** Benjamin Balga <balga DOT benjamin AT gmail DOT com>
**
****************************************************************************/

import QtQuick 2.4
import Machinekit.Application 1.0

ApplicationAction {
    property bool _ready: status.synced && command.connected
    property bool _paused: status.synced && (status.motion.paused || _pauseGuard)
    property bool _motionPaused: status.synced && status.motion.paused //status.task.taskPaused
    property bool _pauseGuard: false

    text: qsTr("Pause")
    icon.source: "qrc:Machinekit/Application/Controls/icons/light/pause"
    shortcut: "P"
    tooltip: qsTr("Pause execution [%1]").arg(shortcut)
    onTriggered: {
        _pauseGuard = true // force paused state and reset when motion is paused
        command.pauseProgram('execute');
    }
    on_MotionPausedChanged: {
        _pauseGuard = false
    }
    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && status.running
             && !_paused
}
