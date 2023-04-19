//
// Created by Maxim on 15.07.2022.
//

#ifndef SHOP_ASSISTANT_MAINWINDOW_H
#define SHOP_ASSISTANT_MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include "ui_MainWindow.h"
#include "ProductTables.h"
#include "implDevices.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget*);

    ~MainWindow() override;

public slots:

    void serviceMessages();

    void create_editProdGroup();

    void create_receptFromTransit();

    void create_addProd();

    void create_docJournal();

    void openProductTables();

    void openScannerSettings();

private:
    Ui::MainWindow* ui_;
    implData*       data_;
    implDevices*    devices_;

};

#endif //SHOP_ASSISTANT_MAINWINDOW_H
