//
// Created by Maxim on 20.07.2022.
//
#include "UI/MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow()), p_ImplData(new implData),
p_implForms(new implForms)
{
    ui->setupUi(this);

    p_ImplData->prodData.upload("C:/Users/Maxim/CLionProjects/shop_assistant/bin/products.json");
    p_ImplData->prodTreeModel.upload("C:/Users/Maxim/CLionProjects/shop_assistant/bin/prod_groups.json");
}

MainWindow::~MainWindow()
{
    p_ImplData->prodData.save("C:/Users/Maxim/CLionProjects/shop_assistant/bin/products1.json");
    p_ImplData->prodTreeModel.save("C:/Users/Maxim/CLionProjects/shop_assistant/bin/prod_groups1.json");
    delete ui;
    delete p_implForms;
    delete p_ImplData;
}

void MainWindow::create_productTable()
{
    auto* newPT_f = new forms::prodTable_form();
    newPT_f->setModelPtr(&p_ImplData->prodTreeModel);
    newPT_f->setDataPtr(&p_ImplData->prodData);
    newPT_f->build();
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
    newPG_e->setModelPtr(&p_ImplData->prodTreeModel);
    newPG_e->setDataPtr(&p_ImplData->prodData);
    newPG_e->build();
    newPG_e->mainWgt->setParent(ui->mdiArea);
    ui->mdiArea->addSubWindow(newPG_e->mainWgt);
    newPG_e->mainWgt->showMaximized();
}

void MainWindow::create_addProducts()
{
    auto newAP_f = new forms::addProd();
    newAP_f->setDataPtr(&p_ImplData->prodData);
    newAP_f->setModelPtr(&p_ImplData->prodTreeModel);
    newAP_f->setupUi();
    newAP_f->mainWgt->setParent(ui->mdiArea);
    ui->mdiArea->addSubWindow(newAP_f->mainWgt);
    newAP_f->mainWgt->showMaximized();
}

