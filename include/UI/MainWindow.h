//
// Created by Maxim on 15.07.2022.
//

#ifndef SHOP_ASSISTANT_MAINWINDOW_H
#define SHOP_ASSISTANT_MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include "implementations/implData.h"
#include "ui_UI.h"
#include "forms/docJournal.h"
#include "forms/prodTable.h"
#include "forms/editProdGroup.h"
#include "forms/receptFromTransit.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget*);

    ~MainWindow();

public slots:
    void create_productTable();

    void show_hide_servMes();

    void create_editProdGroup();

    void create_receptFromTransit();

    void create_addProd();

    void create_docJournal();

private:
    Ui::MainWindow* ui;
    implData* p_ImplData;
    form::docJournal* docJournal;
    QMdiSubWindow* dJSubWindow;
};

#endif //SHOP_ASSISTANT_MAINWINDOW_H
