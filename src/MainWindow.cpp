//
// Created by Maxim on 20.07.2022.
//
#include "UI/MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow()), p_ImplData(new implData),
docJournal(new forms::docJournal_form(ui->mdiArea)), dJSubWindow(new QMdiSubWindow(ui->mdiArea))
{
    ui->setupUi(this);

    p_ImplData->prodContain.upload("../../bin/products.json");
    p_ImplData->prodInfo.upload("../../bin/prodInfo.json");
    p_ImplData->docInfo.upload("../../docs/docInfo.json");
    p_ImplData->addProdDoc.upload("../../docs/addProd_doc.json", p_ImplData->docsPtr);

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
    p_ImplData->prodContain.save("../../bin/products.json");
    p_ImplData->prodInfo.save("../../bin/prodInfo.json");
    p_ImplData->addProdDoc.save("../../docs/addProd_doc.json");
    p_ImplData->docInfo.save("../../docs/docInfo.json");

    delete ui;
    delete p_ImplData;
    delete docJournal;
}

void MainWindow::create_productTable()
{
    forms::make_prodTable(ui->mdiArea, p_ImplData, ui->serviceMessages);
}

void MainWindow::show_hide_servMes()
{
    if(ui->serviceMessages->isHidden()) ui->serviceMessages->show();
    else ui->serviceMessages->hide();
}

void MainWindow::create_editProdGroup()
{
    forms::create_editProdGroupForm(ui->mdiArea, p_ImplData, ui->serviceMessages);
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
    forms::create_addProdDoc(ui->mdiArea, p_ImplData, ui->serviceMessages);
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




