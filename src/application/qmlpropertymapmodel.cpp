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
#include "qmlpropertymapmodel.h"

/*! \qmlproperty qtquickvcp::QmlPropertyMapModel::count

    The number of data entries in the model.
*/

namespace qtquickvcp {

QmlPropertyMapModel::QmlPropertyMapModel(QObject* parent) :
    QAbstractListModel(parent)
{
}

int QmlPropertyMapModel::count() const
{
    return m_model.size();
}

QQmlPropertyMap* QmlPropertyMapModel::append()
{
    return insert(count());
}

QQmlPropertyMap* QmlPropertyMapModel::appendWithProperties(QStringList const& properties)
{
    return insertWithProperties(count(), properties);
}

QQmlPropertyMap* QmlPropertyMapModel::insert(int index)
{
    beginInsertRows(QModelIndex(), index, index);
    auto map = new QQmlPropertyMap(this);
    m_model.insert(index, map);
    endInsertRows();
    emit countChanged(count());
    return map;
}

QQmlPropertyMap* QmlPropertyMapModel::insertWithProperties(int index, QStringList const& properties)
{
    beginInsertRows(QModelIndex(), index, index);
    auto map = new QQmlPropertyMap(this);
    for (auto const& p : properties) {
        map->insert(p, QVariant());
    }
    m_model.insert(index, map);
    endInsertRows();
    emit countChanged(count());
    return map;
}

QQmlPropertyMap* QmlPropertyMapModel::beginAppend()
{
    return beginInsert(count());
}

void QmlPropertyMapModel::endAppend()
{
    endInsert();
}

QQmlPropertyMap* QmlPropertyMapModel::beginInsert(int index)
{
    beginInsertRows(QModelIndex(), index, index);
    auto map = new QQmlPropertyMap(this);
    m_model.insert(index, map);
    return map;
}

void QmlPropertyMapModel::endInsert()
{
    endInsertRows();
    emit countChanged(count());
}

QQmlPropertyMap* QmlPropertyMapModel::at(int index)
{
    return m_model.at(index);
}

QQmlPropertyMap* QmlPropertyMapModel::get(int index)
{
    return at(index);
}

QQmlPropertyMap* QmlPropertyMapModel::first()
{
    return m_model.front();
}

QQmlPropertyMap* QmlPropertyMapModel::last()
{
    return m_model.back();
}

void QmlPropertyMapModel::remove(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    delete m_model.takeAt(index);
    endRemoveRows();
    emit countChanged(count());
}

void QmlPropertyMapModel::removeRange(int from, int to)
{
    beginRemoveRows(QModelIndex(), from, to);
    auto toDelete = m_model.mid(from, (to-from)+1);
    m_model.erase(m_model.begin()+from, m_model.begin()+to);
    endRemoveRows();
    emit countChanged(count());
    qDeleteAll(toDelete);
}

void QmlPropertyMapModel::clear()
{
    beginResetModel();
    auto copy = m_model;
    m_model.clear();
    endResetModel();
    emit countChanged(count());
    qDeleteAll(copy);
}

int QmlPropertyMapModel::rowCount(QModelIndex const& parent) const
{
    Q_UNUSED(parent);
    return count();
}

QVariant QmlPropertyMapModel::data(QModelIndex const& index, int role) const
{
    if (index.row() < 0 || index.row() >= m_model.size())
        return {};

    switch (role) {
    case ModelDataRole:
        return QVariant::fromValue(m_model[index.row()]);
    default:
        return {};
    }
}

QHash<int, QByteArray> QmlPropertyMapModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ModelDataRole] = "modelData";
    return roles;
}

} // namespace qtquickvcp
