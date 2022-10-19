//
// Created by Maxim on 15.07.2022.
//

#ifndef SHOP_ASSISTANT_MAINWINDOW_H
#define SHOP_ASSISTANT_MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include "product_form.h"
#include "implementations/implForms.h"
#include "implementations/implData.h"
#include "ui_UI.h"

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

    static void logger(const QString& message, QTextBrowser*);

private:
    Ui::MainWindow* ui;
    implData* p_ImplData;
    implForms* p_implForms;
};

#endif //SHOP_ASSISTANT_MAINWINDOW_H
