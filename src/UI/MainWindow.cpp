//
// Created by Maxim on 20.07.2022.
//
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui_(new Ui::MainWindow()), data_(new implData),
devices_(new implDevices(nullptr))
{
    ui_->setupUi(this);
    ui_->mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation);
}

MainWindow::~MainWindow()
{
    delete ui_;
    delete data_;
    delete devices_;
}

void MainWindow::serviceMessages()
{
    if(ui_->log->isHidden()) ui_->log->show();
    else ui_->log->hide();
}

void MainWindow::create_editProdGroup()
{

}

void MainWindow::create_receptFromTransit()
{

}

void MainWindow::create_addProd()
{
}

void MainWindow::create_docJournal()
{

}

void MainWindow::openProductTables() {
    form::makeProductTables(data_, ui_->log, ui_->mdiArea);
}

void MainWindow::openScannerSettings()
{
    devices_->scanner.show();
}




