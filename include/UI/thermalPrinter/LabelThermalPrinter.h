//
// Created by Maxim on 12.01.2023.
//

#ifndef SHOP_ASSISTANT_LABELTHERMALPRINTER_H
#define SHOP_ASSISTANT_LABELTHERMALPRINTER_H

#include <QWidget>
#include <QTextBrowser>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTableWidget>
#include <QPrinter>
#include <QtPrintSupport>
#include <QSize>
#include "ui_thermalPrinter.h"
#include "impls/implData.h"
#include "utilities.h"

namespace form {
    class LabelThermalPrinter final : public QWidget {
    Q_OBJECT
    public:
        LabelThermalPrinter(implData*, QTextBrowser*, QWidget* parent = nullptr);

        ~LabelThermalPrinter() override;

    private slots:
        void makeTemplateLabel();
        void print();
        void printDialog();

    private:
        Ui::thermal_printer* ui_;
        implData* data_;
        QTextBrowser* log_;
        QStringList availablePrinters_;
        QTableWidget* templateLabel_;
        QPrinter printer_;
        database::productProperties product_;
    };

    void makeThermalPrinter(implData* data, QTextBrowser* log, QMdiArea* mdiArea);
}

#endif //SHOP_ASSISTANT_LABELTHERMALPRINTER_H
