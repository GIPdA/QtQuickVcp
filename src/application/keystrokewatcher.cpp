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

#include "keystrokewatcher.h"

#include <QKeyEvent>
#include <QDebug>


/*!
    \qmlproperty bool qtquickvcp::KeystrokeWatcher::enabled

    This property holds whether the shortcut is enabled.

    The default value is \c true.
*/

/*! \qmlsignal qtquickvcp::KeystrokeWatcher::pressed()

    This signal is emitted when the key sequence is pressed.

    The corresponding handler is \c onPressed.
*/

/*! \qmlsignal qtquickvcp::KeystrokeWatcher::released()

    This signal is emitted when the key is released.

    The corresponding handler is \c onReleased.
*/

namespace qtquickvcp {

static QKeySequence valueToKeySequence(const QVariant &value)
{
    if (value.type() == QVariant::Int)
        return QKeySequence(static_cast<QKeySequence::StandardKey>(value.toInt()));
    return QKeySequence::fromString(value.toString());
}

KeystrokeWatcher::KeystrokeWatcher(QQuickItem* parent) :
    QQuickItem(parent)
{
    // Install an event filter to receive key events without focus
    connect(this, &QQuickItem::windowChanged, this, [&](QQuickWindow* window) {

        if (m_window)
            m_window->removeEventFilter(this);

        m_window = window;
        if (m_window)
            m_window->installEventFilter(this);
    });
}

KeystrokeWatcher::~KeystrokeWatcher()
{
    if (m_window)
        m_window->removeEventFilter(this);
}

/*!
    \qmlproperty keysequence qtquickvcp::KeystrokeWatcher::sequence

    This property holds the key sequence. The key sequence can be set
    to one of the \l{QKeySequence::StandardKey}{standard keyboard shortcuts}, or
    it can be described with a string containing a shortcut.
    Only one key (optionally combined with modifiers) is supported.

    The default value is an empty key sequence.

    \qml
    KeystrokeWatcher {
        sequence: "Up"
        onPressed: print("Up key pressed")
        onReleased: print("Up key released")
    }
    \endqml

    \sa sequences
*/
QVariant KeystrokeWatcher::sequence() const
{
    return  m_sequence;
}

void KeystrokeWatcher::setSequence(const QVariant& sequence)
{
    if (m_sequence == sequence)
        return;

    QKeySequence keySequence = valueToKeySequence(sequence);
    m_keySequence = keySequence;
    m_key = m_keySequence[0] & 0x1ffffff; // Mask modifiers
    emit sequenceChanged();
}

/*!
    \qmlproperty bool qtquickvcp::KeystrokeWatcher::autoRepeat

    This property holds whether the shortcut can auto repeat
    (onPressed will be called multiple times).

    The default value is \c true.
*/
bool KeystrokeWatcher::autoRepeat() const
{
    return m_autoRepeat;
}

void KeystrokeWatcher::setAutoRepeat(bool autoRepeat)
{
    if (m_autoRepeat == autoRepeat)
        return;

    m_autoRepeat = autoRepeat;
    emit autoRepeatChanged(m_autoRepeat);
}

/*!
    \qmlproperty bool QtQuick::Shortcut::enabled

    This property holds whether the shortcut is enabled.

    The default value is \c true.
*/
bool KeystrokeWatcher::isEnabled() const
{
    return m_enabled;
}

void KeystrokeWatcher::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;
    emit enabledChanged(m_enabled);
}


void KeystrokeWatcher::keyPressEvent(QKeyEvent* event)
{
    if (isEnabled())
        handleKeyPressEvent(event);
}

void KeystrokeWatcher::keyReleaseEvent(QKeyEvent* event)
{
    if (isEnabled())
        handleKeyReleaseEvent(event);
}

bool KeystrokeWatcher::eventFilter(QObject* , QEvent* event)
{
    if (!m_enabled)
        return false;

    switch (event->type())
    {
    case QEvent::ShortcutOverride:
        // Override shortcuts if they match
        if (matchKeyEvent(static_cast<QKeyEvent*>(event)))
            event->accept();
        // Filter event if it matched
        return event->isAccepted();

    case QEvent::KeyPress:
        return handleKeyPressEvent(static_cast<QKeyEvent*>(event));
    case QEvent::KeyRelease:
        return handleKeyReleaseEvent(static_cast<QKeyEvent*>(event));

    default:
        break;
    }

    return false;
}

bool KeystrokeWatcher::matchKeyEvent(QKeyEvent* event) const
{
    return ((event->key()) | static_cast<int>(event->modifiers() & m_keyModifiersMask)) == m_keySequence[0];
}

bool KeystrokeWatcher::handleKeyPressEvent(QKeyEvent* event)
{
    if (matchKeyEvent(event)) {
        if (event->isAutoRepeat() && !m_autoRepeat)
            return true; // Consume but do nothing

        emit pressed();
        return true;
    }
    return false;
}

bool KeystrokeWatcher::handleKeyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == m_key) {
        emit released();
        return true;
    }
    return false;
}

} // namespace qtquickvcp
