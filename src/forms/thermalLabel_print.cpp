//
// Created by Maxim on 01.12.2022.
//
#include "thermalLabel_print.h"

form::thermalLabel_print::thermalLabel_print(implData* data_, QTextBrowser* log_, QWidget* parent) : QWidget(parent),
menu(new QMenuBar(this)), d_noPricePrint(new QLineEdit(this)), no_price_print(new QCheckBox(this)),
d_product(new QLineEdit(this)), product(new QLineEdit(this)), templateTicket(new QTableWidget(this)),
main_l(new QVBoxLayout(this)), menu_l(new QHBoxLayout(this)), chooseProd_l(new QHBoxLayout(this)),
tempTable_l(new QHBoxLayout(this)), data(data_), log(log_), printers(new QComboBox(this)), d_printers(new QLineEdit(this)),
printerList_l(new QHBoxLayout(this)), checkBox_l(new QHBoxLayout(this))
{}

void form::thermalLabel_print::setupUI()
{
    printer_list = utl::getAvailablePrinters();

    menu->setFixedHeight(30);

    d_product->setFrame(false);
    d_product->setReadOnly(true);
    d_product->setText("Введите код/штрих-код товара:");
    d_product->setFixedSize(240, 20);
    d_product->setStyleSheet("background-color:#fffdf4;");

    //product

    d_noPricePrint->setFrame(false);
    d_noPricePrint->setReadOnly(true);
    d_noPricePrint->setText("Код товара вместо цены:");
    d_noPricePrint->setFixedSize(240, 20);
    d_noPricePrint->setStyleSheet("background-color:#fffdf4;");
    no_price_print->setToolTip("Код вместо цены");

    d_printers->setFrame(false);
    d_printers->setReadOnly(true);
    d_printers->setText("Выберите принтер:");
    d_printers->setFixedSize(240, 20);
    d_printers->setStyleSheet("background-color:#fffdf4;");
    printers->addItems(printer_list);


    templateTicket->setFixedSize(320, 210);
    templateTicket->setFrameStyle(QFrame::NoFrame);
    templateTicket->setRowCount(4);
    templateTicket->setColumnCount(1);
    templateTicket->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    templateTicket->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    templateTicket->verticalHeader()->setVisible(false);
    templateTicket->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    templateTicket->horizontalHeader()->setVisible(false);
    templateTicket->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    templateTicket->setShowGrid(false);

    templateTicket->setItem(0, 0, new QTableWidgetItem);
    templateTicket->item(0, 0)->setFlags(templateTicket->item(0, 0)->flags() ^ Qt::ItemIsEditable);
    templateTicket->item(0, 0)->setTextAlignment(Qt::AlignCenter);
    templateTicket->item(0, 0)->setFont(QFont("Times New Roman", 14, QFont::Bold));

    templateTicket->setItem(1, 0, new QTableWidgetItem);
    templateTicket->item(1, 0)->setFlags(templateTicket->item(1, 0)->flags() ^ Qt::ItemIsEditable);
    templateTicket->item(1, 0)->setTextAlignment(Qt::AlignCenter);
    templateTicket->item(1, 0)->setFont(QFont("Times New Roman", 24, QFont::Bold));

    templateTicket->setItem(2, 0, new QTableWidgetItem);
    templateTicket->item(2, 0)->setFlags(templateTicket->item(2, 0)->flags() ^ Qt::ItemIsEditable);
    templateTicket->item(2, 0)->setTextAlignment(Qt::AlignCenter);

    templateTicket->setItem(3, 0, new QTableWidgetItem);
    templateTicket->item(3, 0)->setFlags(templateTicket->item(2, 0)->flags() ^ Qt::ItemIsEditable);
    templateTicket->item(3, 0)->setTextAlignment(Qt::AlignRight);
    templateTicket->item(3, 0)->setFont(QFont("Times New Roman", 12, QFont::Bold));

    menu_l->addWidget(menu);
    chooseProd_l->addWidget(d_product);
    chooseProd_l->addWidget(product);
    checkBox_l->addWidget(d_noPricePrint);
    checkBox_l->addWidget(no_price_print);
    printerList_l->addWidget(d_printers);
    printerList_l->addWidget(printers);
    tempTable_l->addWidget(templateTicket);

    main_l->addLayout(menu_l);
    main_l->addLayout(chooseProd_l);
    main_l->addLayout(checkBox_l);
    main_l->addLayout(printerList_l);
    main_l->addLayout(tempTable_l);

    this->setObjectName("thermalLabel_print");
    this->setLayout(main_l);
    this->setWindowTitle("Печать ценников на термопринтере");
    this->setStyleSheet("QWidget#thermalLabel_print{background-color:#fffdf4;}");

    QObject::connect(product, &QLineEdit::returnPressed, this, &form::thermalLabel_print::print);
}

void form::thermalLabel_print::print()
{
    time_t time = std::time(nullptr);
    tm* timeInfo(std::localtime(&time));
    std::wstring time_str = utl::w_asctime(timeInfo).substr(0, 10);

#if 1
    data::productTableFields s{data->prodData.getProduct(product->text().toInt())};

    if(s.id != 0) {
        templateTicket->item(0, 0)->setText(s.print_name);

        if (no_price_print->isChecked()) {
            templateTicket->item(1, 0)->setText(s.code);
            templateTicket->item(3, 0)->setText(QString::fromStdWString(time_str));
        } else {
            templateTicket->item(1, 0)->setText(QString::number(s.trade_price) + " ₽");
            templateTicket->item(3, 0)->setText(s.code + "   " + QString::fromStdWString(time_str));
        }

        std::string barcodePath{"../../data/barcodes/"};
        barcodePath += s.code.toStdString();
        barcodePath += ".png";

        if (access(barcodePath.c_str(), 0)) {
            utl::barcodeGen(s.code.toStdWString());
        }

        templateTicket->item(2, 0)->setData(Qt::DecorationRole, QPixmap(QString::fromStdString(barcodePath)));

        QPrinter printer(QPrinter::HighResolution);
        printer.setPrinterName(printers->currentText());

        QPainter painter(&printer);
        templateTicket->render(&painter);
    }
    else{
        utl::logger("Товара с кодом " + product->text() + " не существует.", log);
    }
    product->clear();
#endif
#if 0
    auto prod = data->prodContain.products.search(product->text().toStdWString());

    if(prod != data->prodContain.products.end())
    {
        templateTicket->item(0, 0)->setText(QString::fromStdWString(prod->second->map()[L"print_name"]->wstring()));

        if(no_price_print->isEnabled())
        {
            templateTicket->item(1, 0)->setText(QString::fromStdWString(prod->second->map()[L"code"]->wstring()));
        }
        else
        {
            templateTicket->item(1, 0)->setText(QString::fromStdWString(prod->second->map()[L"trade_price"]->wstring()));
        }

        std::string barcodePath{"../../data/barcodes/"};
        barcodePath += std::string(prod->second->map()[L"code"]->wstring().begin(), prod->second->map()[L"code"]->wstring().end());
        barcodePath += ".png";

        if (access(barcodePath.c_str(), 0))
        {
            utl::barcodeGen(prod->second->map()[L"code"]->wstring());
        }

        templateTicket->item(2, 0)->setData(Qt::DecorationRole, QPixmap(QString::fromStdString(barcodePath)));
        templateTicket->item(3, 0)->setText(QString::fromStdWString(prod->second->map()[L"code"]->wstring())
        + "   " + QString::fromStdWString(time_str));

        QPrinter printer(QPrinter::HighResolution);
        printer.setPrinterName(printers->currentText());

        QPainter painter(&printer);
        templateTicket->render(&painter);
    }
    else
    {
        utl::logger("Товара с кодом " + product->text() + " не существует.", log);
    }
#endif
}

void form::thermalLabel_print::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void form::make_thermalLabelPrint(QMdiArea* mdiArea, implData* data, QTextBrowser* log)
{
    auto subWindow = new QMdiSubWindow(mdiArea);
    auto tL_f = new form::thermalLabel_print(data, log, subWindow);

    subWindow->setWidget(tL_f);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(subWindow);

    tL_f->setupUI();

    subWindow->show();

    QObject::connect(tL_f, &QWidget::destroyed, [subWindow, mdiArea]()
    {
        //mdiArea->closeActiveSubWindow();
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });
}
