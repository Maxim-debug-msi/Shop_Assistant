//
// Created by Maxim on 21.07.2022.
//

#ifndef SHOP_ASSISTANT_PRODUCT_FORM_H
#define SHOP_ASSISTANT_PRODUCT_FORM_H

#include <fstream>
#include <QTableWidget>
#include <QTreeView>
#include <vector>
#include "models/objectTree_model.h"
#include "data/Data.h"

namespace forms
{
    class prodTable_form : QObject{
        Q_OBJECT

    public:
        prodTable_form();

        ~prodTable_form() override;

        void build();

        void setModelPtr(dataContain::prodTree_model*);

        void setDataPtr(dataContain::prod_data*);

    public slots:
        void drowTable();

    public:
        QWidget* mainWgt;
    private:
        std::map<std::wstring, std::pair<QTableWidget*, int>> prodSearch_code; //словарь для быстрого поиска товара в tables
        std::map<std::wstring, QTableWidget*> tables;

        QWidget* table;
        QTreeView* viewModel;
        QVector<QObject*> obj_list;
        models::objectTree_model* oT_model;
        dataContain::prod_data* data;
        dataContain::prodTree_model* model;
    };
}

#endif //SHOP_ASSISTANT_PRODUCT_FORM_H
