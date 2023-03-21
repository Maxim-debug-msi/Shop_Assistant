//
// Created by Maxim on 21.07.2022.
//

#ifndef SHOP_ASSISTANT_PRODTABLE_H
#define SHOP_ASSISTANT_PRODTABLE_H

#include <QTableWidget>
#include <QTreeView>
#include <QLayout>
#include <QTreeView>
#include <QPushButton>
#include <QHeaderView>
#include <QMdiArea>
#include <QPrinter>
#include <QPrintDialog>
#include <vector>
#include "models/objectTree_model.h"
#include "impls/implData.h"
#include "data/data.h"
#include "forms/prodCard.h"
#include "forms/thermalLabel_print.h"


namespace form
{
    class prodTable : public QWidget
    {
        Q_OBJECT

    public:
        prodTable(implData*, QTextBrowser*, QWidget* parent = nullptr);

        ~prodTable() override = default;

        void setupUI();

        void setMdiAreaPtr(QMdiArea*);

    public slots:
        void refillingTable(const std::wstring&);

    private slots:
        void showTable();
        void openProdCard(const std::wstring&, const std::wstring&);
        void printer();

    private:
        QPushButton* open_prod_card_btn;
        QPushButton* show_hide_model_btn;
        QPushButton* printer_btn;
        QWidget* table_board;

    private:
        QVBoxLayout* main_l;
        QHBoxLayout* topMenu_l;
        QHBoxLayout* topMenu2_l;
        QHBoxLayout* middle_l;
    private:
        std::map<std::wstring, std::pair<QTableWidget*, int>> prodSearch_code; //словарь для быстрого поиска товара в tables
        std::map<std::wstring, QTableWidget*> tables;

        QObject* root_model_obj;

        QTreeView* view_model;
        models::objectTree_model* obj_tree_model;

        implData* data;
        QMdiArea* mdiArea;
        QTextBrowser* log;
    };

    void make_prodTable(QMdiArea* mdiArea_, implData* data, QTextBrowser* log);
}


#endif //SHOP_ASSISTANT_PRODTABLE_H
