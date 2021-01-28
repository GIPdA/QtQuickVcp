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
**
****************************************************************************/

import QtQuick 2.4
import Machinekit.Application 1.0

ApplicationAction {
    id: root

    property int axis: 0
    property double velocity: settings.initialized ? settings.values["axis" + axis]["jogVelocity"] : 0.0
    property double distance: 0.0
    property double safeDistance: 0.0 // Simulates continuous jog with stream of steps. Set to 0 to use the "true" continuous jog
    property int safeJogInterval: 100 // ms

    property bool _ready: status.synced && command.connected

    function stop() {
        continuousJogTimer.stop();
        if (_ready)
            command.jog(ApplicationCommand.StopJog, axis);
    }

    text: ""
    shortcut: ""
    tooltip: qsTr("Jog Axis %1 [%2]").arg(axis).arg(shortcut)
    onTriggered: {
        if (status.task.taskMode !== ApplicationStatus.TaskModeManual) {
            command.setTaskMode('execute', ApplicationCommand.TaskModeManual);
        }
        if (velocity !== 0.0) {
            if (distance === 0.0) {
                if (safeDistance === 0.0)
                    command.jog(ApplicationCommand.ContinuousJog, axis, velocity);
                else
                    continuousJogTimer.restart()
            }
            else {
                command.jog(ApplicationCommand.IncrementJog, axis, velocity, distance);
            }
        } else {
            continuousJogTimer.stop()
            command.jog(ApplicationCommand.StopJog, axis);
        }
    }

    enabled: _ready
             && (status.task.taskState === ApplicationStatus.TaskStateOn)
             && !status.running

    onEnabledChanged: {
        if (!enabled)
            stop()
    }

    Timer {
        id: continuousJogTimer
        running: false
        repeat: true
        triggeredOnStart: true
        interval: root.safeJogInterval // ms
        onTriggered: {
            command.jog(ApplicationCommand.IncrementJog, axis, velocity, safeDistance);
        }
    }
}
