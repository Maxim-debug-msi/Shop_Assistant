//
// Created by Maxim on 21.07.2022.
//

#ifndef SHOP_ASSISTANT_OBJECTTREE_MODEL_H
#define SHOP_ASSISTANT_OBJECTTREE_MODEL_H

#include <QAbstractItemModel>
#include <QWidget>

namespace models
{
    class objectTree_model : public QAbstractItemModel
    {
    Q_OBJECT

    public:
        objectTree_model(QObject* parent = nullptr);
        void setColumns(QStringList cols);
        void addItem(QObject* item, const QModelIndex& parentIdx);

    public:
        QModelIndex index(int row, int column, const QModelIndex &parent) const override;
        QModelIndex parent(const QModelIndex &child) const override;
        int rowCount(const QModelIndex& parent) const override;
        int columnCount(const QModelIndex& parent) const override;
        QVariant data(const QModelIndex& index, int role) const override;

    protected:
        QStringList columns;
        QObject* rootItem;

        QObject* objByIndex(const QModelIndex& index) const;
    };
}
#endif //SHOP_ASSISTANT_OBJECTTREE_MODEL_H
