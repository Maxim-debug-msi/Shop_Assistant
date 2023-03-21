//
// Created by Maxim on 30.01.2023.
//

#ifndef SHOP_ASSISTANT_DATAMATRIXLIST_H
#define SHOP_ASSISTANT_DATAMATRIXLIST_H

#include <QWidget>
#include <QTextBrowser>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSet>
#include "impls/implData.h"
#include "ui_DataMatrixList.h"
#include "ProductCard.h"

namespace form
{
    class DataMatrixList final : public QWidget
    {
        Q_OBJECT

    public:
        DataMatrixList(implData* data, QTextBrowser* log, QWidget *parent, const int& code);
        ~DataMatrixList() override;

    signals:
        void deleted();

    private slots:
        void addRow();
        void deleteRow();
        void save();

    protected:
        void closeEvent(QCloseEvent* event);

    private:
        Ui::dataMatrixList* ui_;
        implData* data_;
        QTextBrowser* log_;
        int code_;
        QSet<QString> checkedDM_;
    };

    void makeDataMatrixList(implData* data, QTextBrowser* log, QMdiArea* mdi, const int& code);
}

#endif //SHOP_ASSISTANT_DATAMATRIXLIST_H
