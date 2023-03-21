//
// Created by Maxim on 12.01.2023.
//

#include <LabelThermalPrinter.h>
#include "defs.h"

form::LabelThermalPrinter::LabelThermalPrinter(implData* data, QTextBrowser* log,QWidget* parent) : QWidget(parent),
ui_(new Ui::thermal_printer()), data_(data), log_(log), templateLabel_(new QTableWidget(this))
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    printer_.setResolution(QPrinter::HighResolution);

    ui_->setupUi(this);
    ui_->buttons_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui_->template_label_l->addWidget(templateLabel_);
    ui_->printer_list->addItems(utl::getAvailablePrinters());
    printer_.setPrinterName(ui_->printer_list->itemText(0));

    templateLabel_->setFixedSize(330, 220);
    templateLabel_->setFrameStyle(QFrame::NoFrame);
    templateLabel_->setRowCount(4);
    templateLabel_->setColumnCount(1);
    templateLabel_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    templateLabel_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    templateLabel_->verticalHeader()->setVisible(false);
    templateLabel_->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    templateLabel_->horizontalHeader()->setVisible(false);
    templateLabel_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    templateLabel_->setShowGrid(false);

    templateLabel_->setItem(0, 0, new QTableWidgetItem);
    templateLabel_->item(0, 0)->setFlags(templateLabel_->item(0, 0)->flags() ^ Qt::ItemIsEditable);
    templateLabel_->item(0, 0)->setTextAlignment(Qt::AlignCenter);
    templateLabel_->item(0, 0)->setFont(QFont("Times New Roman", 14, QFont::Bold));

    templateLabel_->setItem(1, 0, new QTableWidgetItem);
    templateLabel_->item(1, 0)->setFlags(templateLabel_->item(1, 0)->flags() ^ Qt::ItemIsEditable);
    templateLabel_->item(1, 0)->setTextAlignment(Qt::AlignCenter);
    templateLabel_->item(1, 0)->setFont(QFont("Times New Roman", 24, QFont::Bold));

    templateLabel_->setItem(2, 0, new QTableWidgetItem);
    templateLabel_->item(2, 0)->setFlags(templateLabel_->item(2, 0)->flags() ^ Qt::ItemIsEditable);
    templateLabel_->item(2, 0)->setTextAlignment(Qt::AlignCenter);

    templateLabel_->setItem(3, 0, new QTableWidgetItem);
    templateLabel_->item(3, 0)->setFlags(templateLabel_->item(2, 0)->flags() ^ Qt::ItemIsEditable);
    templateLabel_->item(3, 0)->setTextAlignment(Qt::AlignRight);
    templateLabel_->item(3, 0)->setFont(QFont("Times New Roman", 14, QFont::Bold));

    ui_->code_line->setFocus();

    QObject::connect(ui_->print_btn, &QPushButton::clicked, this, &LabelThermalPrinter::print);
    QObject::connect(ui_->code_line, &QLineEdit::returnPressed, this, &LabelThermalPrinter::makeTemplateLabel);
    QObject::connect(ui_->printDialog_btn, & QPushButton::clicked, this, &LabelThermalPrinter::printDialog);
}

form::LabelThermalPrinter::~LabelThermalPrinter()
{
    delete ui_;
    delete templateLabel_;
    this->disconnect();
}

void form::LabelThermalPrinter::print()
{
    printer_.setPrinterName(ui_->printer_list->currentText());
    QPageSize pageSize(QSizeF(43.0, 25.0), QPageSize::Millimeter, "", QPageSize::FuzzyMatch);
    printer_.setPageSize(pageSize);

    QImage img(TERM_LABEL);
    QPainter painter(&printer_);
    QRect rect = painter.viewport();
    QSize size = img.size();
    size.scale(rect.size(), Qt::AspectRatioMode::IgnoreAspectRatio);
    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow(img.rect());
    painter.drawImage(0, 0, img);
    painter.end();
#if 0
    QPageSize pageSize(QSizeF(43.0, 25.0), QPageSize::Millimeter, "", QPageSize::ExactMatch);
    printer_.setPageSize(pageSize);
    QPainter painter(&printer_);
    QPixmap pixmap(templateLabel_->size());
    templateLabel_->render(&pixmap);
    QRect targetRect(0, 0, 192, 120);
    painter.drawPixmap(targetRect, pixmap, pixmap.rect());
#endif
}

void form::LabelThermalPrinter::printDialog()
{
    QPrintDialog dialog(&printer_, this);

    if(dialog.exec() == QPrintDialog::Accepted)
    {
        print();
    }
}

void form::LabelThermalPrinter::makeTemplateLabel()
{
    time_t time = std::time(nullptr);
    tm* timeInfo(std::localtime(&time));
    std::wstring time_str = utl::w_asctime(timeInfo).substr(0, 10);

    if(ui_->code_line->text().size() <= 7)
    {
        product_ = data_->productData.getProduct(ui_->code_line->text().toInt());
    }
    else
    {
        product_ = data_->productData.getProduct(ui_->code_line->text());
    }


    if(product_.code != 0) {
        templateLabel_->item(0, 0)->setText(product_.print_name);

        if (ui_->code_or_price_checkbox->isChecked()) {
            templateLabel_->item(1, 0)->setText(utl::toStrCode(product_.code));
            templateLabel_->item(3, 0)->setText(QString::fromStdWString(time_str));
        } else {
            templateLabel_->item(1, 0)->setText(QString::number(product_.trade_price) + "₽");
            templateLabel_->item(3, 0)->setText(utl::toStrCode(product_.code) + "   " + QString::fromStdWString(time_str) + "  ");
        }

        utl::barcodeGen(utl::toStrCode(product_.code).toStdWString());

        templateLabel_->item(2, 0)->setData(Qt::DecorationRole, QPixmap(BAR_GEN));

        QPixmap pixmap = QPixmap(templateLabel_->size());
        templateLabel_->render(&pixmap);
        pixmap.save(TERM_LABEL, nullptr, 100);

        print();
    }
    else{
        utl::logger("Товара с кодом " + ui_->code_line->text() + " не существует.", log_);
    }
    ui_->code_line->clear();
}

void form::makeThermalPrinter(implData* data, QTextBrowser* log, QMdiArea* mdiArea)
{
    auto subWindow = new QMdiSubWindow(mdiArea);
    auto lblTermPrint = new form::LabelThermalPrinter(data, log, subWindow);

    subWindow->setWidget(lblTermPrint);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(subWindow);

    subWindow->show();

    QObject::connect(lblTermPrint, &QWidget::destroyed, [subWindow, mdiArea]()
    {
        subWindow->disconnect();
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });
}

