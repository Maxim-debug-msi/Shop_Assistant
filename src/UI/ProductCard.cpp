//
// Created by Maxim on 09.01.2023.
//

#include <ProductCard.h>
#include "defs.h"

form::ProductCard::ProductCard(implData* data, QTextBrowser* log, QMdiArea* mdi, QWidget *parent, const int &code) :
QWidget(parent), data_(data), log_(log), mdiArea_(mdi), ui_(new Ui::product_card())
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(this->windowTitle() + " ");

    ui_->setupUi(this);

    ui_->buttons_l->setAlignment(Qt::AlignLeft | Qt:: AlignTop);
    ui_->trade_price->setMaximum(std::numeric_limits<double>::max());
    ui_->retail_price->setMaximum(std::numeric_limits<double>::max());
    ui_->count->setMaximum(std::numeric_limits<double>::max());

    ui_->product_groups->addItems(utl::getProductGroupsList(data_));
    if(code > 0)
    {
        database::productProperties pp{data_->productData.getProduct(code)};

        ui_->product_groups->findText(pp.group);
        ui_->product_code->setText(utl::toStrCode(pp.code));
        ui_->product_name->setText(pp.name);
        ui_->print_name->setText(pp.print_name);
        ui_->count->setValue(pp.count);
        ui_->trade_price->setValue(pp.trade_price);
        ui_->retail_price->setValue(pp.retail_price);
        ui_->barcode->setText(pp.barcode);
        ui_->qr_code->setText(pp.qrcode);
        ui_->provider->setText(pp.provider);
        ui_->exp_date->setValue(pp.exp_date);
        ui_->dm_check->setChecked(pp.mark);

        status_ = 1;
        blockEdit();
    }
    else
    {
        if(data_->jsonData.prodInfo[L"backup_codes"]->vector(tag::vecInt()).empty())
        {
            ui_->product_code->setText(utl::toStrCode(data_->jsonData.prodInfo[L"current_code"]->integer()));
        }
        else
        {
            ui_->product_code->setText(utl::toStrCode(data_->jsonData.prodInfo[L"backup_codes"]->vector(tag::vecInt()).back()));
        }
        blockEdit();
    }

    QObject::connect(ui_->save_btn, &QPushButton::clicked, this, &ProductCard::saveProduct);
    QObject::connect(ui_->delete_btn, &QPushButton::clicked, this, &ProductCard::deleteProduct);
    QObject::connect(ui_->edit_btn, &QPushButton::clicked, this, &ProductCard::editProduct);
    QObject::connect(ui_->print_label_btn, &QPushButton::clicked, this, &ProductCard::openThermalPrintForm);
    QObject::connect(ui_->addDataMatrix_btn, &QPushButton::clicked, this, &ProductCard::openDataMatrixList);

    QObject::connect(ui_->product_groups, &QComboBox::editTextChanged, this, &form::ProductCard::editedProductStatus);
    QObject::connect(ui_->product_code, &QLineEdit::editingFinished, this, &form::ProductCard::editedProductStatus);
    QObject::connect(ui_->product_name, &QLineEdit::editingFinished, this, &form::ProductCard::editedProductStatus);
    QObject::connect(ui_->print_name, &QLineEdit::editingFinished, this, &form::ProductCard::editedProductStatus);
    QObject::connect(ui_->barcode, &QLineEdit::editingFinished, this, &form::ProductCard::editedProductStatus);
    QObject::connect(ui_->retail_price, &QDoubleSpinBox::editingFinished, this, &form::ProductCard::editedProductStatus);
    QObject::connect(ui_->trade_price, &QDoubleSpinBox::editingFinished, this, &form::ProductCard::editedProductStatus);
    QObject::connect(ui_->exp_date, &QSpinBox::editingFinished, this, &form::ProductCard::editedProductStatus);
    QObject::connect(ui_->provider, &QLineEdit::editingFinished, this, &form::ProductCard::editedProductStatus);
    QObject::connect(ui_->dm_check, &QCheckBox::clicked, this, &form::ProductCard::editedProductStatus);
}

form::ProductCard::~ProductCard()
{
    delete ui_;
    this->disconnect();
}

void form::ProductCard::blockEdit()
{
    switch(status_)
    {
        case 0:
        {
            ui_->save_btn->setEnabled(true);
            ui_->delete_btn->setEnabled(true);
            ui_->edit_btn->setEnabled(false);
            ui_->print_label_btn->setEnabled(false);
            ui_->product_groups->setEnabled(true);
            ui_->barcode->setReadOnly(false);
            ui_->product_name->setReadOnly(false);
            ui_->print_name->setReadOnly(false);
            ui_->retail_price->setReadOnly(false);
            ui_->trade_price->setReadOnly(false);
            ui_->count->setReadOnly(false);
            ui_->exp_date->setReadOnly(false);
            ui_->provider->setReadOnly(false);
            ui_->qr_code->setReadOnly(false);
            ui_->dm_check->setEnabled(true);
            break;
        }
        case 1:
        {
            ui_->save_btn->setEnabled(false);
            ui_->delete_btn->setEnabled(false);
            ui_->edit_btn->setEnabled(true);
            ui_->print_label_btn->setEnabled(true);
            ui_->product_groups->setEnabled(false);
            ui_->barcode->setReadOnly(true);
            ui_->product_name->setReadOnly(true);
            ui_->print_name->setReadOnly(true);
            ui_->retail_price->setReadOnly(true);
            ui_->trade_price->setReadOnly(true);
            ui_->count->setReadOnly(true);
            ui_->exp_date->setReadOnly(true);
            ui_->provider->setReadOnly(true);
            ui_->qr_code->setReadOnly(true);
            ui_->dm_check->setEnabled(false);
            break;
        }
        case 2:
        {
            ui_->save_btn->setEnabled(true);
            ui_->delete_btn->setEnabled(false);
            ui_->edit_btn->setEnabled(false);
            ui_->print_label_btn->setEnabled(false);
            ui_->product_groups->setEnabled(false);
            ui_->barcode->setReadOnly(true);
            ui_->product_name->setReadOnly(true);
            ui_->print_name->setReadOnly(true);
            ui_->retail_price->setReadOnly(true);
            ui_->trade_price->setReadOnly(true);
            ui_->exp_date->setReadOnly(true);
            ui_->count->setReadOnly(true);
            ui_->provider->setReadOnly(true);
            ui_->qr_code->setReadOnly(true);
            ui_->dm_check->setEnabled(false);
            break;
        }
        case 3:
        {
            ui_->delete_btn->setEnabled(false);
            ui_->edit_btn->setEnabled(false);
            ui_->print_label_btn->setEnabled(false);
            ui_->dm_check->setEnabled(true);
            break;
        }
        default:
        {

        }
    }
}

void form::ProductCard::saveProduct()
{
    data_->productData.insertProduct(
            {.code = ui_->product_code->text().toInt(),
            .group{ui_->product_groups->currentText()},
            .provider{ui_->provider->text()},
            .name{ui_->product_name->text()},
            .print_name{ui_->print_name->text()},
            .barcode{ui_->barcode->text()},
            .qrcode{ui_->qr_code->text()},
            .count = ui_->count->value(),
            .exp_date = ui_->exp_date->value(),
            .retail_price = ui_->retail_price->value(),
            .trade_price = ui_->trade_price->value(),
            .mark = ui_->dm_check->isChecked()
            });

    status_ = 1;

    blockEdit();

    utl::logger("Товар " + ui_->product_code->text() + " " + ui_->product_name->text() + " сохранён.", log_);

    savedProductStatus();

    if(data_->jsonData.prodInfo[L"backup_codes"]->vector(tag::vecInt()).empty())
    {
        ++data_->jsonData.prodInfo[L"current_code"]->integer();
    }
    else
    {
        data_->jsonData.prodInfo[L"backup_codes"]->vector(tag::vecInt()).pop_back();
    }

    data_->jsonData.prodInfo.fwrite(PROD_JSON);

    emit changed(ui_->product_groups->currentText());
}

void form::ProductCard::deleteProduct()
{
    data_->productData.deleteProduct(ui_->product_code->text().toInt());

    data_->jsonData.prodInfo[L"backup_codes"]->vector(tag::vecInt()).push_back(ui_->product_code->text().toInt());

    status_ = 2;

    blockEdit();

    utl::logger("Товар " + ui_->product_code->text() + " " + ui_->product_name->text() + " удален", log_);

    savedProductStatus();

    data_->jsonData.prodInfo.fwrite(PROD_JSON);

    emit changed(ui_->product_groups->currentText());
}

void form::ProductCard::editProduct()
{
    status_ = 0;

    blockEdit();
}

void form::ProductCard::savedProductStatus()
{
    QString title{this->windowTitle()};
    title.back() = ' ';
    this->setWindowTitle(title);
}

void form::ProductCard::editedProductStatus()
{
    QString title{this->windowTitle()};
    title.back() = '*';
    this->setWindowTitle(title);
}

void form::ProductCard::openThermalPrintForm()
{
    form::makeThermalPrinter(data_, log_, mdiArea_);
}

void form::ProductCard::openDataMatrixList()
{
    if(status_ != 1)
    {
        utl::logger("Для внесения кодов DataMatrix товар должен быть сохранен.", log_);
        return;
    }
    else if(!ui_->dm_check->isChecked())
    {
        utl::logger("Для данного товара не устaновлен флаг \"Маркировка\".", log_);
        return;
    }

   form::makeDataMatrixList(data_, log_, mdiArea_, ui_->product_code->text().toInt());
}

void form::ProductCard::closeEvent(QCloseEvent *event)
{
    emit deleted();
    event->accept();
}

form::ProductCard* form::makeProductCard(implData* data, QTextBrowser* log, QMdiArea* mdiArea, const int& code)
{
    auto* subWindow= new QMdiSubWindow(mdiArea);
    auto* prodCard = new form::ProductCard(data, log, mdiArea, subWindow, code);

    subWindow->setWidget(prodCard);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(subWindow);

    subWindow->show();

    QObject::connect(prodCard, &form::ProductCard::deleted, [subWindow, mdiArea]()
    {
        subWindow->disconnect();
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });

    return prodCard;
}

