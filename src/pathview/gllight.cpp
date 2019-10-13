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

#include "gllight.h"

namespace qtquickvcp {

GLLight::GLLight(QObject *parent) :
    QObject(parent),
    m_position(QVector3D(1.0, 1.0, 5.0)),
    m_intensities(QVector3D(1.0, 1.0, 1.0)),
    m_attenuation(0.01f),
    m_enabled(true),
    m_ambientCoefficient(0.1f)
{
    connect(this, &GLLight::positionChanged,
            this, &GLLight::propertyChanged);
    connect(this, &GLLight::intensitiesChanged,
            this, &GLLight::propertyChanged);
    connect(this, &GLLight::attenuationChanged,
            this, &GLLight::propertyChanged);
    connect(this, &GLLight::ambientCoefficientChanged,
            this, &GLLight::propertyChanged);
    connect(this, &GLLight::enabledChanged,
            this, &GLLight::propertyChanged);
}


QVector3D GLLight::position() const
{
    return m_position;
}

QVector3D GLLight::intensities() const
{
    return m_intensities;
}

float GLLight::attenuation() const
{
    return m_attenuation;
}

bool GLLight::enabled() const
{
    return m_enabled;
}

float GLLight::ambientCoefficient() const
{
    return m_ambientCoefficient;
}

void GLLight::setPosition(QVector3D arg)
{
    if (m_position != arg) {
        m_position = arg;
        emit positionChanged(arg);
    }
}

void GLLight::setIntensities(QVector3D arg)
{
    if (m_intensities != arg) {
        m_intensities = arg;
        emit intensitiesChanged(arg);
    }
}

void GLLight::setAttenuation(float arg)
{
    if (!qFuzzyCompare(m_attenuation, arg)) {
        m_attenuation = arg;
        emit attenuationChanged(arg);
    }
}

void GLLight::setEnabled(bool arg)
{
    if (m_enabled != arg) {
        m_enabled = arg;
        emit enabledChanged(arg);
    }
}

void GLLight::setAmbientCoefficient(float arg)
{
    if (!qFuzzyCompare(m_ambientCoefficient, arg)) {
        m_ambientCoefficient = arg;
        emit ambientCoefficientChanged(arg);
    }
}
} // namespace qtquickvcp
