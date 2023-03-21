//
// Created by Maxim on 09.01.2023.
//

#ifndef SHOP_ASSISTANT_PRODUCTCARD_H
#define SHOP_ASSISTANT_PRODUCTCARD_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QMdiArea>
#include <QMdiSubWindow>
#include "ui_productCard.h"
#include "impls/implData.h"
#include "utilities.h"
#include "LabelThermalPrinter.h"
#include "DataMatrixList.h"

namespace form
{
    class ProductCard final : public QWidget
    {
        Q_OBJECT
    public:
        ProductCard(implData* data, QTextBrowser* log, QMdiArea* mdi, QWidget* parent = nullptr, const int& code = 0);
        ~ProductCard() override;

    signals:
        void changed(const QString& group);
        void deleted();

    private slots:
        void blockEdit();
        void saveProduct();
        void deleteProduct();
        void editProduct();
        void savedProductStatus();
        void editedProductStatus();
        void openThermalPrintForm();
        void openDataMatrixList();

    protected:
        void closeEvent(QCloseEvent* event);

    private:
        Ui::product_card* ui_;
        implData* data_;
        QTextBrowser* log_;
        QMdiArea* mdiArea_;
        int status_{3}; // 0-edit, 1-save, 2-delete, 3-new
    };

    form::ProductCard* makeProductCard(implData* data, QTextBrowser* log, QMdiArea* mdiArea, const int& code = 0);
}


#endif //SHOP_ASSISTANT_PRODUCTCARD_H
