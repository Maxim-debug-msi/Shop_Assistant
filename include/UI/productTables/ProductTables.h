//
// Created by Maxim on 22.12.2022.
//

#ifndef SHOP_ASSISTANT_PRODUCTTABLES_H
#define SHOP_ASSISTANT_PRODUCTTABLES_H

#include <QWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTableWidget>
#include "ui_ProductTables.h"
#include "impls/implData.h"
#include "models/objectTree_model.h"
#include "ProductCard.h"
#include "utilities.h"
#include "LabelThermalPrinter.h"
#include "addGroup_dlg.h"
#include "deleteGroup_dlg.h"
#include "searchProduct_dlg.h"

namespace form {
    class ProductTables final : public QWidget {
    Q_OBJECT

    public:
        explicit ProductTables(implData* data, QTextBrowser* log, QMdiArea* mdi, QWidget *parent = nullptr);

        ~ProductTables() override;

    private slots:
        void changeVisibleTable();
        void openProdCard(const int&, const int&);
        void addProduct();
        void openThermalPrintForm();
        void showHideGroupTree();
        void addProductGroup();
        void deleteProductGroup();
        void fillTable(const QString& group);
        void fillTables();
        void fillModel();
        void findProductTableRow();
        void keyPressEvent(QKeyEvent* event) override;
        void findProductDlg();

    protected:
        bool eventFilter(QObject* watcher, QEvent* event) override;

    private:
        QMap<QString, QTableWidget*> tables_;
        QString searchWord_;
        Ui::ProductTables *ui_;
        implData* data_;
        QTextBrowser* log_;
        QMdiArea* mdiArea_;

        models::objectTree_model* model_;
        QObject* rootModelObj_;
    };

    void makeProductTables(implData* data, QTextBrowser* log, QMdiArea* mdiArea);
}

#endif //SHOP_ASSISTANT_PRODUCTTABLES_H
