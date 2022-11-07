//
// Created by Maxim on 21.07.2022.
//

#ifndef SHOP_ASSISTANT_PRODUCT_FORM_H
#define SHOP_ASSISTANT_PRODUCT_FORM_H

#include <QTableWidget>
#include <QTreeView>
#include <QLayout>
#include <QTreeView>
#include <QPushButton>
#include <QHeaderView>
#include <QMdiArea>
#include <vector>
#include "models/objectTree_model.h"
#include "implementations/implData.h"
#include "data/Data.h"
#include "forms/prod_card.h"

namespace forms
{
    class prodTable_form : public QObject{
        Q_OBJECT

    public:
        prodTable_form(implData*, QTextBrowser*, QWidget* parent = nullptr);

        ~prodTable_form() override;

        void setupUI();

        void setMainWindow_ptr(QMdiArea*);

    private slots:
        void showTable();
        void openProdCard(const std::wstring&, std::map<std::wstring, QTableWidget*>* tables = nullptr);

    public:
        QWidget* mainWgt;

    private:
        QPushButton* openProdCard_btn;
        QWidget* tableBoard;
        QTableWidget* table;

    private:
        QVBoxLayout* main_l;
        QHBoxLayout* topMenu_l;
        QHBoxLayout* middle_l;
    private:
        std::map<std::wstring, std::pair<QTableWidget*, int>> prodSearch_code; //словарь для быстрого поиска товара в tables
        std::map<std::wstring, QTableWidget*> tables;

        QObject* rootModelObj;

        QTreeView* viewModel;
        models::objectTree_model* oT_model;

        implData* data;
        QMdiArea* mainWindow;
        QTextBrowser* log;
    };

    void make_prodTable(QMdiArea*, implData*, QTextBrowser*);
}

#endif //SHOP_ASSISTANT_PRODUCT_FORM_H
