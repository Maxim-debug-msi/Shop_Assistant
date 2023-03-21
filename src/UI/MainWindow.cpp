//
// Created by Maxim on 20.07.2022.
//
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow()), p_ImplData(new implData),
docJournal(new form::docJournal(ui->mdiArea))
{
    ui->setupUi(this);
    ui->mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete p_ImplData;
    delete docJournal;
}

void MainWindow::show_hide_servMes()
{
    if(ui->log->isHidden()) ui->log->show();
    else ui->log->hide();
}

void MainWindow::create_editProdGroup()
{
//    form::create_editProdGroup(ui->mdiArea, p_ImplData, ui->log);
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

}

void MainWindow::openProductTables() {
    form::makeProductTables(p_ImplData, ui->log, ui->mdiArea);
}




