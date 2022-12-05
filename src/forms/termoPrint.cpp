//
// Created by Maxim on 01.12.2022.
//
#include "termoPrint.h"

form::termoPrint::termoPrint(implData* data_, QTextBrowser* log_, QWidget* parent) : QWidget(parent), menu(new QMenuBar(this)),
d_product(new QLineEdit(this)), product(new QLineEdit(this)), templateTicket(new QTableWidget(this)),
main_l(new QVBoxLayout(this)), menu_l(new QHBoxLayout(this)), chooseProd_l(new QHBoxLayout(this)),
tempTable_l(new QHBoxLayout(this)), data(data_), log(log_), printers(new QComboBox(this)), d_printers(new QLineEdit(this)),
printerList_l(new QHBoxLayout(this))
{}

void form::termoPrint::setupUI()
{
    printerList = utl::getAvailablePrinters();

    menu->setFixedHeight(30);

    d_product->setFrame(false);
    d_product->setReadOnly(true);
    d_product->setText("Введите код/штрих-код товара:");
    d_product->setFixedSize(270, 20);

    //product

    d_printers->setFrame(false);
    d_printers->setReadOnly(true);
    d_printers->setText("Выберите принтер:");
    d_printers->setFixedSize(270, 20);
    printers->addItems(printerList);


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
    QFont font("Times New Roman", 12);
    font.setBold(true);

    templateTicket->setItem(0, 0, new QTableWidgetItem);
    templateTicket->item(0, 0)->setFlags(templateTicket->item(0, 0)->flags() ^ Qt::ItemIsEditable);
    templateTicket->item(0, 0)->setTextAlignment(Qt::AlignCenter);
    templateTicket->item(0, 0)->setFont(font);

    templateTicket->setItem(1, 0, new QTableWidgetItem);
    templateTicket->item(1, 0)->setFlags(templateTicket->item(1, 0)->flags() ^ Qt::ItemIsEditable);
    templateTicket->item(1, 0)->setTextAlignment(Qt::AlignCenter);
    templateTicket->item(1, 0)->setFont(QFont("Times New Roman", 32, QFont::Bold));

    templateTicket->setItem(2, 0, new QTableWidgetItem);
    templateTicket->item(2, 0)->setFlags(templateTicket->item(2, 0)->flags() ^ Qt::ItemIsEditable);
    templateTicket->item(2, 0)->setTextAlignment(Qt::AlignCenter);

    templateTicket->setItem(3, 0, new QTableWidgetItem);
    templateTicket->item(3, 0)->setFlags(templateTicket->item(2, 0)->flags() ^ Qt::ItemIsEditable);
    templateTicket->item(3, 0)->setTextAlignment(Qt::AlignRight);
    templateTicket->item(3, 0)->setFont(font);

    menu_l->addWidget(menu);
    chooseProd_l->addWidget(d_product);
    chooseProd_l->addWidget(product);
    printerList_l->addWidget(d_printers);
    printerList_l->addWidget(printers);
    tempTable_l->addWidget(templateTicket);

    main_l->addLayout(menu_l);
    main_l->addLayout(chooseProd_l);
    main_l->addLayout(printerList_l);
    main_l->addLayout(tempTable_l);

    this->setLayout(main_l);
    this->setWindowTitle("Печать ценников на термопринтере");

    QObject::connect(product, &QLineEdit::returnPressed, this, &form::termoPrint::findProduct);
}

void form::termoPrint::findProduct()
{
    time_t time = std::time(nullptr);
    tm* timeInfo(std::localtime(&time));
    std::wstring time_str = utl::w_asctime(timeInfo).substr(0, 10);

    auto prod = data->prodContain.products.search(product->text().toStdWString());

    if(prod != data->prodContain.products.end())
    {
        templateTicket->item(0, 0)->setText(QString::fromStdWString(prod->second->get_map()[L"имя_печать"]->get_wstring()));
        templateTicket->item(1, 0)->setText(QString::fromStdWString(prod->second->get_map()[L"р_цена"]->get_wstring()));

        std::string barcodePath{"../../data/barcodes/"};
        barcodePath += std::string(prod->second->get_map()[L"код"]->get_wstring().begin(), prod->second->get_map()[L"код"]->get_wstring().end());
        barcodePath += ".png";

        if (access(barcodePath.c_str(), 0))
        {
            utl::barcodeGen(prod->second->get_map()[L"код"]->get_wstring());
        }

        templateTicket->item(2, 0)->setData(Qt::DecorationRole, QPixmap(QString::fromStdString(barcodePath)));
        templateTicket->item(3, 0)->setText(QString::fromStdWString(time_str));

        print();
    }
    else
    {
        utl::logger("Товара с кодом " + product->text() + " не существует.", log);
    }
}

void form::termoPrint::print()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPrinterName(printers->currentText());
    //printer.setPageSize(QPageSize(QPageSize::Letter));
    //printer.setPageOrientation(QPageLayout::Landscape);
    //printer.setPageMargins(QMarginsF(-1, -0.1, 0.1, 0.1), QPageLayout::Millimeter);
    //printer.setPageMargins(0.1, 0.1, 0.1, 0.1, QPrinter::Millimeter);
    //printer.setFullPage(true);
    //printer.setResolution(200);
    //QPrintDialog dialog(&printer, this);

    QPainter painter(&printer);
    templateTicket->render(&painter);

    //auto preview = new QPrintPreviewDialog(&printer, this);
    //preview->setFont(QFont("Arial", 18, QFont::Bold));
    //preview->show();
}

void form::make_termoPrint(QMdiArea* mdiArea, implData* data, QTextBrowser* log, QWidget *parent)
{
    auto subWindow = new QMdiSubWindow(mdiArea);
    auto tP_f = new form::termoPrint(data, log, subWindow);

    subWindow->setWidget(tP_f);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    tP_f->setupUI();

    mdiArea->addSubWindow(subWindow);

    subWindow->show();

    QObject::connect(tP_f, &QWidget::destroyed, [subWindow, mdiArea]()
    {
        //delete pC_f;
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });
    QObject::connect(parent, &QWidget::destroyed, [subWindow, mdiArea]()
    {
        //delete pC_f;
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });
}
