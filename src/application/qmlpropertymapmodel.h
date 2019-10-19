/****************************************************************************
**
** Copyright (C) 2019 Benjamin Balga
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
** Benjamin Balga <balga DOT benjamin AT gmail DOT com>
**
****************************************************************************/
/*!
 * \brief Model of QQmlPropertyMap
 * This model allows to create a model with dynamic & bindable roles in QML,
 * to replace Javascript arrays or ListModels that can't have bound properties.
 *
 * Usage:
 * PropertyMapModel {
 *     id: myModel
 *     Component.onCompleted: {
 *         var t = append() // Create properties "on the fly" (doesn't work with ListView?)
 *         t.foo = Qt.binding(function() { return someProperty })
 *         t.bar = Qt.binding(function() { return someOtherProperty })
 *
 *         var e = appendWithProperties(["foo", "bar"]) // Pre-create some properties
 *         e.foo = Qt.binding(function() { return someProperty })
 *         e.bar = Qt.binding(function() { return someOtherProperty })
 *         e.foo2 = 42
 *
 *         var f = beginAppend() // Create all properties and bindings before adding the row to the model
 *         // Doesn't support nested beginAppend/Insert()
 *         f.foo = Qt.binding(function() { return someProperty })
 *         f.bar = Qt.binding(function() { return someOtherProperty })
 *         endAppend() // Don't forget to call it!
 *     }
 * }
 *
 * Repeater {
 *     model: myModel
 *     Text { text: modelData.foo + " " + modelData.bar }
 * }
 *
 */
#ifndef QMLPROPERTYMAPMODEL_H
#define QMLPROPERTYMAPMODEL_H

#include <QAbstractListModel>
#include <QQmlPropertyMap>

namespace qtquickvcp {

class QmlPropertyMapModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum ModelRoles {
        ModelDataRole = Qt::UserRole + 1
    };

    QmlPropertyMapModel(QObject *parent = nullptr);

    int count() const;

    Q_INVOKABLE QQmlPropertyMap* append();
    Q_INVOKABLE QQmlPropertyMap* appendWithProperties(QStringList const& properties);
    Q_INVOKABLE QQmlPropertyMap* insert(int index);
    Q_INVOKABLE QQmlPropertyMap* insertWithProperties(int index, QStringList const& properties);

    Q_INVOKABLE QQmlPropertyMap* beginAppend();
    Q_INVOKABLE void endAppend();
    Q_INVOKABLE QQmlPropertyMap* beginInsert(int index);
    Q_INVOKABLE void endInsert();


    Q_INVOKABLE QQmlPropertyMap* at(int index);
    Q_INVOKABLE QQmlPropertyMap* get(int index);
    Q_INVOKABLE QQmlPropertyMap* first();
    Q_INVOKABLE QQmlPropertyMap* last();

    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void removeRange(int from, int to);

    Q_INVOKABLE void clear();

    int rowCount(QModelIndex const& parent = QModelIndex()) const;
    QVariant data(QModelIndex const& index, int role = ModelDataRole) const;

signals:
    void countChanged(int count);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QVector<QQmlPropertyMap*> m_model;
};
} // namespace qtquickvcp

#endif // QMLPROPERTYMAPMODEL_H
