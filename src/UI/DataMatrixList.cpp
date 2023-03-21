//
// Created by Maxim on 30.01.2023.
//

#include <UI/DataMatrixList/DataMatrixList.h>
#include "defs.h"

form::DataMatrixList::DataMatrixList(implData *data, QTextBrowser *log, QWidget *parent, const int& code) : QWidget(parent),
ui_(new Ui::dataMatrixList), data_(data), log_(log), code_(code)
{
    this->setAttribute(Qt::WA_DeleteOnClose);

    ui_->setupUi(this);

    QVector<QString> dms = data->productData.getDM(code_);

    ui_->topButtons_l->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui_->dm_tbl->setColumnCount(2);
    ui_->dm_tbl->setRowCount(dms.size());
    ui_->dm_tbl->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui_->dm_tbl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui_->dm_tbl->setHorizontalHeaderItem(1, new QTableWidgetItem);
    ui_->dm_tbl->horizontalHeaderItem(1)->setData(Qt::DecorationRole, QPixmap(CB_ICO));
    ui_->dm_tbl->setHorizontalHeaderLabels({"Маркировка", ""});
    ui_->dm_tbl->verticalHeader()->hide();
    ui_->product_le->setText(utl::toStrCode(code) + " " + data->productData.getProductName(code));

    int row{0};

    for(auto&& dm : dms)
    {
        ui_->dm_tbl->setItem(row, 0, new QTableWidgetItem(dm));
        ui_->dm_tbl->item(row, 0)->setFlags(ui_->dm_tbl->item(row, 0)->flags() ^ Qt::ItemIsEditable);

        ui_->dm_tbl->setItem(row, 1, new QTableWidgetItem);
        ui_->dm_tbl->item(row, 1)->setFlags(ui_->dm_tbl->item(row, 1)->flags() ^ Qt::ItemIsEditable);
        ui_->dm_tbl->item(row, 1)->setCheckState(Qt::Unchecked);

        ++row;
    }

    QObject::connect(ui_->addDM_btn, &QPushButton::clicked, this, &form::DataMatrixList::addRow);
    QObject::connect(ui_->deleteDM_btn, &QPushButton::clicked, this, &form::DataMatrixList::deleteRow);
    QObject::connect(ui_->save_btn, &QPushButton::clicked, this, &form::DataMatrixList::save);
}

form::DataMatrixList::~DataMatrixList()
{
    delete ui_;
    this->disconnect();
}

void form::DataMatrixList::closeEvent(QCloseEvent *event)
{
    emit deleted();
    event->accept();
}

void form::DataMatrixList::addRow()
{
    ui_->dm_tbl->setRowCount(ui_->dm_tbl->rowCount() + 1);
    int currentRowIdx{ui_->dm_tbl->rowCount() - 1};

    ui_->dm_tbl->setItem(currentRowIdx, 0, new QTableWidgetItem);
    ui_->dm_tbl->item(currentRowIdx, 0)->setFlags(ui_->dm_tbl->item(currentRowIdx, 0)->flags() ^ Qt::ItemIsEditable);

    ui_->dm_tbl->setItem(currentRowIdx, 1, new QTableWidgetItem());
    ui_->dm_tbl->item(currentRowIdx, 1)->setFlags(ui_->dm_tbl->item(currentRowIdx, 1)->flags() ^ Qt::ItemIsEditable);
    ui_->dm_tbl->item(currentRowIdx, 1)->setCheckState(Qt::Unchecked);
}

void form::DataMatrixList::deleteRow()
{
    if(ui_->dm_tbl->rowCount() > 0)
    {
        for (int row{0}, end{ui_->dm_tbl->rowCount()}; row < end; ++row)
        {
            if(ui_->dm_tbl->item(row, 1)->checkState())
            {
                data_->productData.deleteDM(ui_->dm_tbl->item(row, 0)->text());
                ui_->dm_tbl->removeRow(row);
                --row;
                --end;
            }
        }
    }
}

void form::DataMatrixList::save()
{
    if(ui_->dm_tbl->rowCount() == 0)
    {
        utl::logger("Нечего сохранять.", log_);
        return;
    }
    else
    {
        std::vector<QString> result;
        for(int row{0}, endRow{ui_->dm_tbl->rowCount()}; row < endRow; ++row)
        {
            result.push_back(ui_->dm_tbl->item(row, 0)->text());
        }
        data_->productData.saveDM(result, code_);

        utl::logger("КМ успешно записаны.", log_);
    }
}

void form::makeDataMatrixList(implData* data, QTextBrowser* log, QMdiArea* mdiArea, const int& code)
{
    auto* subWindow = new QMdiSubWindow(mdiArea);
    auto* dml = new form::DataMatrixList(data, log, subWindow, code);

    subWindow->setWidget(dml);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(subWindow);

    subWindow->show();

    QObject::connect(dml, &form::DataMatrixList::deleted, [subWindow, mdiArea]()
    {
        subWindow->disconnect();
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });
}

