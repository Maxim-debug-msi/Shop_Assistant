//
// Created by Maxim on 01.12.2022.
//

#ifndef SHOP_ASSISTANT_THERMALLABEL_PRINT_H
#define SHOP_ASSISTANT_THERMALLABEL_PRINT_H

#include <QWidget>
#include <QTableWidget>
#include <QMenuBar>
#include <QLayout>
#include <QPrinter>
#include <QPrintDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QLineEdit>
#include <QHeaderView>
#include <QtPrintSupport>
#include <QComboBox>
#include <QStyleOption>
#include <QDate>
#include <io.h>
#include <ctime>
#include "utilities.h"
#include "impls/implData.h"

namespace form {
    class thermalLabel_print : public QWidget {
    Q_OBJECT

    public:
        explicit thermalLabel_print(implData*, QTextBrowser*, QWidget* parent = nullptr);

        ~thermalLabel_print() override = default;

        void setupUI();

    protected:
        void paintEvent(QPaintEvent*) override;

    private slots:
        void print();

    private:
        QMenuBar *menu;

        QLineEdit* d_product;
        QLineEdit* product;

        QLineEdit* d_printers;
        QComboBox* printers;

        QLineEdit* d_noPricePrint;
        QCheckBox* no_price_print;

        QTableWidget *templateTicket;

    private:
        QVBoxLayout *main_l;
        QHBoxLayout* menu_l;
        QHBoxLayout* chooseProd_l;
        QHBoxLayout* checkBox_l;
        QHBoxLayout* printerList_l;
        QHBoxLayout* tempTable_l;

    private:
        implData* data;
        QTextBrowser* log;
        QStringList printer_list;
    };

    void make_thermalLabelPrint(QMdiArea*, implData*, QTextBrowser*);
}
#endif //SHOP_ASSISTANT_THERMALLABEL_PRINT_H
