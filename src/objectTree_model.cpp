//
// Created by Maxim on 21.07.2022.
//

#include "models/objectTree_model.h"

#include <utility>

models::objectTree_model::objectTree_model(QObject * parent) : QAbstractItemModel(parent)
{
    rootItem = new QObject(this);
}

void models::objectTree_model::setColumns(QStringList cols)
{
    columns = std::move(cols);
}

QModelIndex models::objectTree_model::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
    {
        return QModelIndex{};
    }
    QObject* parentObj = objByIndex(parent);
    return createIndex(row, column, parentObj->children().at(row));
}

QModelIndex models::objectTree_model::parent(const QModelIndex &child) const
{
    QObject* childObj = objByIndex(child);
    QObject* parentObj = childObj->parent();

    if(parentObj == rootItem)
    {
        return {};
    }

    QObject* grandParentObj = parentObj->parent();
    int row = grandParentObj->children().indexOf(parentObj);
    return createIndex(row, 0, parentObj);
}

int models::objectTree_model::rowCount(const QModelIndex &parent) const
{
    return objByIndex(parent)->children().count();
}

int models::objectTree_model::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return columns.count();
}

QVariant models::objectTree_model::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant{};
    }
    if(role == Qt::DisplayRole)
    {
        return objByIndex(index)->property(columns.at(index.column()).toUtf8());
    }
    return QVariant{};
}

QObject *models::objectTree_model::objByIndex(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return rootItem;
    }
    return static_cast<QObject*>(index.internalPointer());
}

void models::objectTree_model::addItem(QObject *item, const QModelIndex &parentIdx)
{
    beginInsertRows(parentIdx, rowCount(parentIdx), rowCount(parentIdx));

    item->setParent(objByIndex(parentIdx));

    endInsertRows();
}




