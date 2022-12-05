//
// Created by Maxim on 01.12.2022.
//

#ifndef SHOP_ASSISTANT_TERMOPRINT_H
#define SHOP_ASSISTANT_TERMOPRINT_H

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
#include <io.h>
#include <ctime>
#include "utilities.h"
#include "implementations/implData.h"

namespace form {
    class termoPrint : public QWidget {
    Q_OBJECT

    public:
        explicit termoPrint(implData*, QTextBrowser*, QWidget* parent = nullptr);

        ~termoPrint() override = default;

        void setupUI();

    private slots:
        void findProduct();

        void print();

    private:
        QMenuBar *menu;

        QLineEdit* d_product;
        QLineEdit* product;

        QLineEdit* d_printers;
        QComboBox* printers;

        QTableWidget *templateTicket;

    private:
        QVBoxLayout *main_l;
        QHBoxLayout* menu_l;
        QHBoxLayout* chooseProd_l;
        QHBoxLayout* printerList_l;
        QHBoxLayout* tempTable_l;

    private:
        implData* data;
        QTextBrowser* log;
        QStringList printerList;
    };

    void make_termoPrint(QMdiArea*, implData*, QTextBrowser*, QWidget* parent = nullptr);
}
#endif //SHOP_ASSISTANT_TERMOPRINT_H
