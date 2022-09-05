//
// Created by Maxim on 02.09.2022.
//

#ifndef SHOP_ASSISTANT_ADDPROD_FORM_H
#define SHOP_ASSISTANT_ADDPROD_FORM_H
#include <QObject>
#include <QLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QVector>
#include <QComboBox>
#include <vector>
#include <string>
#include "data/Data.h"
#include "models/objectTree_model.h"

struct prodProperties
{

};

namespace forms
{
    class addProd : QObject
    {
        Q_OBJECT

    public:
        addProd();

        ~addProd() override;

        void setDataPtr(dataContain::prod_data*);

        void setModelPtr(dataContain::prodTree_model*);

        void setupUi();

    public:
        QWidget* mainWgt;

    private: //widgets
        QTableWidget* table;
        QPushButton* addProduct;
        QPushButton* delProduct;
        QComboBox* groupList;

        QPushButton* write;
        QPushButton* enter;

    private: //layouts
        QVBoxLayout* vl;
        QHBoxLayout* hl;
        QGridLayout* gl;
        QHBoxLayout* hl2;

    private: //data
        QVector<QObject*> obj_list;
        dataContain::prod_data* data;
        dataContain::prodTree_model* model;
        models::objectTree_model* oT_model;

        std::wstring docNum;
        std::wstring nameOfCreator;
    };
}
#endif //SHOP_ASSISTANT_ADDPROD_FORM_H
