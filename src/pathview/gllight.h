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

#ifndef GLLIGHT_H
#define GLLIGHT_H

#include <QObject>
#include <QVector3D>

namespace qtquickvcp {

class GLLight : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D intensities READ intensities WRITE setIntensities NOTIFY intensitiesChanged)
    Q_PROPERTY(float attenuation READ attenuation WRITE setAttenuation NOTIFY attenuationChanged)
    Q_PROPERTY(float ambientCoefficient READ ambientCoefficient WRITE setAmbientCoefficient NOTIFY ambientCoefficientChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit GLLight(QObject *parent = nullptr);

    QVector3D position() const;
    QVector3D intensities() const;
    float attenuation() const;
    bool enabled() const;
    float ambientCoefficient() const;

signals:
    void positionChanged(QVector3D arg);
    void intensitiesChanged(QVector3D arg);
    void attenuationChanged(float arg);
    void enabledChanged(bool arg);
    void ambientCoefficientChanged(float arg);
    void propertyChanged();

public slots:
    void setPosition(QVector3D arg);
    void setIntensities(QVector3D arg);
    void setAttenuation(float arg);
    void setEnabled(bool arg);
    void setAmbientCoefficient(float arg);

private:
    QVector3D m_position;
    QVector3D m_intensities;
    float m_attenuation;
    bool m_enabled;
    float m_ambientCoefficient;
}; // class GLLight
} // namespace qtquickvcp

#endif // GLLIGHT_H
