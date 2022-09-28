//
// Created by Maxim on 02.09.2022.
//

#ifndef SHOP_ASSISTANT_RECEPTFROMTRANSIT_FORM_H
#define SHOP_ASSISTANT_RECEPTFROMTRANSIT_FORM_H
#include <QObject>
#include <QLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QVector>
#include <QComboBox>
#include <vector>
#include <string>
#include "data/Data.h"
#include "implementations/implData.h"
#include "models/objectTree_model.h"

namespace forms
{
    class receptFromTransit_form : QObject
    {
        Q_OBJECT

    public:
        receptFromTransit_form();

        ~receptFromTransit_form() override;

        void setDataPtr(implData*);

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
        implData* data;
        models::objectTree_model* oT_model;

        std::wstring docNum;
        std::wstring nameOfCreator;
    };
}
#endif //SHOP_ASSISTANT_RECEPTFROMTRANSIT_FORM_H
