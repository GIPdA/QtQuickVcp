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
#ifndef KEYSTROKEWATCHER_H
#define KEYSTROKEWATCHER_H

#include <QEvent>
#include <QPointer>
#include <QQuickItem>
#include <QQuickWindow>

namespace qtquickvcp {

class KeystrokeWatcher : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QVariant sequence READ sequence WRITE setSequence NOTIFY sequenceChanged FINAL)
    Q_PROPERTY(bool autoRepeat READ autoRepeat WRITE setAutoRepeat NOTIFY autoRepeatChanged FINAL)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged FINAL)

public:
    explicit KeystrokeWatcher(QQuickItem *parent = nullptr);
    ~KeystrokeWatcher() override;

    QVariant sequence() const;
    void setSequence(const QVariant &sequence);

    bool autoRepeat() const;
    void setAutoRepeat(bool autoRepeat);

    bool isEnabled() const;
    void setEnabled(bool enabled);

signals:
    void sequenceChanged();
    void autoRepeatChanged(bool autoRepeat);
    void enabledChanged(bool enabled);

    void pressed();
    void released();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    //bool event(QEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    bool matchKeyEvent(QKeyEvent* event) const;
    bool handleKeyPressEvent(QKeyEvent *event);
    bool handleKeyReleaseEvent(QKeyEvent *event);

    QVariant m_sequence;
    QKeySequence m_keySequence;
    int m_key {0};
    Qt::KeyboardModifiers m_keyModifiersMask {Qt::CTRL|Qt::SHIFT|Qt::ALT|Qt::META};
    bool m_autoRepeat {false};
    bool m_enabled {true};

    QPointer<QQuickWindow> m_window;
}; // class KeyShortcut
} // namespace qtquickvcp

#endif // KEYSTROKEWATCHER_H
