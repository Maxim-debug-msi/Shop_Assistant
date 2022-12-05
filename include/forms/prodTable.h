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
#include "implementations/implData.h"
#include "data/Data.h"
#include "forms/prodCard.h"
#include "forms/termoPrint.h"


namespace form
{
    class prodTable : public QWidget
    {
        Q_OBJECT

    public:
        prodTable(implData*, QTextBrowser*, QWidget* parent = nullptr);

        ~prodTable() override = default;

        void setupUI();

        void setMainWindow_ptr(QMdiArea*);

    public slots:
        void refillingTable(const std::wstring&);

    private slots:
        void showTable();
        void openProdCard(const std::wstring&, const std::wstring&);
        void printer();

    private:
        QPushButton* openProdCard_btn;
        QPushButton* showHideModel_btn;
        QPushButton* printer_btn;
        QWidget* tableBoard;

    private:
        QVBoxLayout* main_l;
        QHBoxLayout* topMenu_l;
        QHBoxLayout* topMenu2_l;
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


#endif //SHOP_ASSISTANT_PRODTABLE_H
