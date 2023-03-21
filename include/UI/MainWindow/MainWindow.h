//
// Created by Maxim on 15.07.2022.
//

#ifndef SHOP_ASSISTANT_MAINWINDOW_H
#define SHOP_ASSISTANT_MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include "ui_MainWindow.h"
#include "forms/docJournal.h"
#include "forms/prodTable.h"
#include "forms/editProdGroup.h"
#include "forms/receptFromTransit.h"
#include "ProductTables.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget*);

    ~MainWindow() override;

public slots:

    void show_hide_servMes();

    void create_editProdGroup();

    void create_receptFromTransit();

    void create_addProd();

    void create_docJournal();

    void openProductTables();

private:
    Ui::MainWindow* ui;
    implData* p_ImplData;
    form::docJournal* docJournal;
};

#endif //SHOP_ASSISTANT_MAINWINDOW_H
