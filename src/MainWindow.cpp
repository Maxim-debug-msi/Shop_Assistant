//
// Created by Maxim on 20.07.2022.
//
#include "UI/MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow()), p_ImplData(new implData),
docJournal(new form::docJournal(ui->mdiArea)), dJSubWindow(new QMdiSubWindow(ui->mdiArea))
{
    ui->setupUi(this);

    p_ImplData->prodContain.upload("../../data/products/products.json");
    p_ImplData->prodInfo.upload("../../data/products/prodInfo.json");
    p_ImplData->docInfo.upload("../../data/docs/docInfo.json");
    p_ImplData->addProdDoc.upload("../../data/docs/addProd_doc.json", p_ImplData->docsPtr);

    dJSubWindow->setWidget(docJournal->mainWgt);
    docJournal->setLog_ptr(ui->serviceMessages);
    docJournal->setData_ptr(p_ImplData);
    docJournal->setMainWindow_ptr(ui->mdiArea);
    docJournal->setupUI();
    ui->mdiArea->addSubWindow(dJSubWindow);
    dJSubWindow->hide();
}

MainWindow::~MainWindow()
{
    p_ImplData->prodContain.save("../../data/products/products.json");
    p_ImplData->prodInfo.save("../../data/products/prodInfo.json");
    p_ImplData->addProdDoc.save("../../data/docs/addProd_doc.json");
    p_ImplData->docInfo.save("../../data/docs/docInfo.json");

    delete ui;
    delete p_ImplData;
    delete docJournal;
}

void MainWindow::create_productTable()
{
    form::make_prodTable(ui->mdiArea, p_ImplData, ui->serviceMessages);
}

void MainWindow::show_hide_servMes()
{
    if(ui->serviceMessages->isHidden()) ui->serviceMessages->show();
    else ui->serviceMessages->hide();
}

void MainWindow::create_editProdGroup()
{
    form::create_editProdGroup(ui->mdiArea, p_ImplData, ui->serviceMessages);
}

void MainWindow::create_receptFromTransit()
{
    auto newRFT_f = new form::receptFromTransit();
    newRFT_f->setDataPtr(p_ImplData);
    newRFT_f->setupUi();
    newRFT_f->mainWgt->setParent(ui->mdiArea);
    ui->mdiArea->addSubWindow(newRFT_f->mainWgt);
    newRFT_f->mainWgt->showMaximized();
}

void MainWindow::create_addProd()
{
}

void MainWindow::create_docJournal()
{
    if(dJSubWindow->isHidden())
    {
        dJSubWindow->showMaximized();
        docJournal->mainWgt->showMaximized();
    }
    else
    {
        dJSubWindow->hide();
        docJournal->mainWgt->hide();
    }
}




