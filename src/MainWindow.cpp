//
// Created by Maxim on 20.07.2022.
//
#include "UI/MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow()), p_ImplData(new implData),
p_implForms(new implForms)
{
    ui->setupUi(this);

    p_ImplData->prodContain.upload("C:/Users/Maxim/CLionProjects/shop_assistant/bin/products.json");
    p_ImplData->prodInfo.upload("C:/Users/Maxim/CLionProjects/shop_assistant/bin/prodInfo.json");
    p_ImplData->docInfo.upload("C:/Users/Maxim/CLionProjects/shop_assistant/docs/docInfo.json");

    p_ImplData->addProdDoc.upload("C:/Users/Maxim/CLionProjects/shop_assistant/docs/addProd_doc.json", p_ImplData->docsPtr);
}

MainWindow::~MainWindow()
{
    p_ImplData->prodContain.save("C:/Users/Maxim/CLionProjects/shop_assistant/bin/products.json");
    p_ImplData->prodInfo.save("C:/Users/Maxim/CLionProjects/shop_assistant/bin/prodInfo.json");
    p_ImplData->addProdDoc.save("C:/Users/Maxim/CLionProjects/shop_assistant/docs/addProd_doc.json");
    p_ImplData->docInfo.save("C:/Users/Maxim/CLionProjects/shop_assistant/docs/docInfo.json");

    delete ui;
    delete p_implForms;
    delete p_ImplData;
}

void MainWindow::create_productTable()
{
    auto* newPT_f = new forms::prodTable_form();
    newPT_f->setDataPtr(p_ImplData);
    newPT_f->setupUI();
    newPT_f->mainWgt->setParent(ui->mdiArea);
    ui->mdiArea->addSubWindow(newPT_f->mainWgt);
    newPT_f->mainWgt->showMaximized();
}

void MainWindow::show_hide_servMes()
{
    if(ui->serviceMessages->isHidden()) ui->serviceMessages->show();
    else ui->serviceMessages->hide();
}

void MainWindow::create_editProdGroup()
{
    auto* newPG_e = new forms::prodGroup_editor();
    newPG_e->setDataPtr(p_ImplData);
    newPG_e->setupUI();
    newPG_e->mainWgt->setParent(ui->mdiArea);
    ui->mdiArea->addSubWindow(newPG_e->mainWgt);
    newPG_e->mainWgt->showMaximized();
}

void MainWindow::create_receptFromTransit()
{
    auto newRFT_f = new forms::receptFromTransit_form();
    newRFT_f->setDataPtr(p_ImplData);
    newRFT_f->setupUi();
    newRFT_f->mainWgt->setParent(ui->mdiArea);
    ui->mdiArea->addSubWindow(newRFT_f->mainWgt);
    newRFT_f->mainWgt->showMaximized();
}

void MainWindow::create_addProd()
{
    auto newAP_f = new forms::addProduct_form;
    auto subWindow = new QMdiSubWindow(ui->mdiArea);

    subWindow->setWidget(newAP_f->mainWgt);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    newAP_f->setDataPtr(p_ImplData);
    newAP_f->setLog_ptr(ui->serviceMessages);
    newAP_f->setupUI();

    ui->mdiArea->addSubWindow(subWindow);

    subWindow->show();

    QObject::connect(newAP_f->mainWgt, &QWidget::destroyed, [newAP_f, subWindow, this]()
    {
        delete newAP_f;
        ui->mdiArea->removeSubWindow(subWindow);
    });
}

void MainWindow::logger(const QString &message, QTextBrowser *log)
{
    char buffer[25];
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    const char* format = "%d.%m.%Y %HH:%MM:%SS";
    strftime(buffer, 25, format, now);

    log->setMarkdown(log->toMarkdown() + "[" + buffer + "]" + " " + message);
}

void MainWindow::create_docJournal()
{
    auto newDJ_f = new forms::docJournal_form();
    newDJ_f->setLog_ptr(ui->serviceMessages);
    newDJ_f->setData_ptr(p_ImplData);
    newDJ_f->setMainWindow_ptr(ui->mdiArea);
    newDJ_f->setupUI();
    newDJ_f->mainWgt->setParent(ui->mdiArea);
    ui->mdiArea->addSubWindow(newDJ_f->mainWgt);
    newDJ_f->mainWgt->showMaximized();
}


