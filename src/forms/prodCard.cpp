//
// Created by Maxim on 31.10.2022.
//

#include "prodCard.h"

form::prodCard::prodCard(implData *data_, QTextBrowser * log_, QWidget *parent, std::wstring  prodCode, std::wstring  prodGroup) :
                            product_group{std::move(prodGroup)},
                            QWidget(parent), product_code{std::move(prodCode)}, hl(new QHBoxLayout(this)),
                            data(data_), log(log_), d_group(new QLineEdit(this)), group(new QComboBox(this)),
                            d_code(new QLineEdit(this)), code(new QLineEdit(this)), d_name(new QLineEdit(this)),
                            name(new QLineEdit(this)), d_print_name(new QLineEdit(this)), print_name(new QLineEdit(this)),
                            d_barcode(new QLineEdit(this)), barcode(new QLineEdit(this)), d_retail_price(new QLineEdit(this)),
                            retail_price(new QDoubleSpinBox(this)), d_trade_price(new QLineEdit(this)), trade_price(new QDoubleSpinBox(this)),
                            d_exp_date(new QLineEdit(this)), exp_date(new QSpinBox(this)), actions_btn(new QToolButton(this)),
                            actions(new QMenu(actions_btn)), save_product(new QAction("Сохранить", actions)),
                            delete_product(new QAction("Удалить", actions)), edit_product(new QAction("Редактировать")),
                            vl(new QVBoxLayout(this)), hl0(new QHBoxLayout(this)), hl1(new QHBoxLayout(this)),
                            hl2(new QHBoxLayout(this)), hl3(new QHBoxLayout(this)), hl4(new QHBoxLayout(this)),
                            hl5(new QHBoxLayout(this)), hl6(new QHBoxLayout(this)), hl7(new QHBoxLayout(this)),
                            oT_model(new models::objectTree_model(group)), root_model_obj(new QObject(this)),
                            d_provider(new QLineEdit(this)), provider(new QLineEdit(this)), d_qrcode(new QLineEdit(this)),
                            qrcode(new QLineEdit(this)), provider_l(new QHBoxLayout(this)), qrcode_l(new QHBoxLayout(this)),
                            d_count(new QLineEdit(this)), count(new QDoubleSpinBox(this)), count_l(new QHBoxLayout(this)){}

void form::prodCard::setupUI()
{
    std::function<void(std::unordered_map<std::wstring, variant*>&)> extractor =
            [&extractor, this](std::unordered_map<std::wstring, variant*>& map)
            {
                for(auto&& it : map)
                {
                    auto obj = new QObject(root_model_obj);
                    obj->setObjectName(QString::fromStdWString(it.first));

                    oT_model->addItem(obj, QModelIndex());

                    if(it.second != nullptr && it.second->is_map() && !it.second->map().empty())
                    {
                        extractor(it.second->map());
                    }
                }
            };

    QStringList cols;
    cols << "objectName";
    oT_model->setColumns(cols);

    extractor(data->prodInfo.prodInfo[L"groups"]->map());

    group->setModel(oT_model);

    actions->addAction(edit_product);
    actions->addAction(save_product);
    actions->addAction(delete_product);

    actions_btn->setMenu(actions);
    actions_btn->setPopupMode(QToolButton::InstantPopup);
    actions_btn->setText("Действия");
    actions_btn->setFixedSize(100, 30);
    actions_btn->setArrowType(Qt::ArrowType::RightArrow);
    actions_btn->setToolButtonStyle(Qt::ToolButtonTextOnly);

    d_group->setFrame(false);
    d_group->setReadOnly(true);
    d_group->setFixedSize(200, 20);
    d_group->setText("Группа:");
    d_group->setStyleSheet("background-color:#fffdf4;");
    group->setFixedSize(150, 20);

    d_code->setFrame(false);
    d_code->setReadOnly(true);
    d_code->setFixedSize(200, 20);
    d_code->setText("Код:");
    d_code->setStyleSheet("background-color:#fffdf4;");
    code->setFixedSize(150, 20);
    code->setReadOnly(true);

    d_name->setFrame(false);
    d_name->setReadOnly(true);
    d_name->setFixedSize(200, 20);
    d_name->setText("Наименование:");
    d_name->setStyleSheet("background-color:#fffdf4;");
    name->setFixedSize(150, 20);

    d_print_name->setFrame(false);
    d_print_name->setReadOnly(true);
    d_print_name->setFixedSize(200, 20);
    d_print_name->setText("Наименование(для печати):");
    d_print_name->setStyleSheet("background-color:#fffdf4;");
    print_name->setFixedSize(150, 20);

    d_barcode->setFrame(false);
    d_barcode->setReadOnly(true);
    d_barcode->setFixedSize(200, 20);
    d_barcode->setText("Штрих-код:");
    d_barcode->setStyleSheet("background-color:#fffdf4;");
    barcode->setFixedSize(150, 20);

    d_qrcode->setFrame(false);
    d_qrcode->setReadOnly(true);
    d_qrcode->setFixedSize(200, 20);
    d_qrcode->setText("Честный знак:");
    d_qrcode->setStyleSheet("background-color:#fffdf4;");
    qrcode->setFixedSize(150, 20);

    d_provider->setFrame(false);
    d_provider->setReadOnly(true);
    d_provider->setFixedSize(200, 20);
    d_provider->setText("Поставщик:");
    d_provider->setStyleSheet("background-color:#fffdf4;");
    provider->setFixedSize(150, 20);

    d_retail_price->setFrame(false);
    d_retail_price->setReadOnly(true);
    d_retail_price->setFixedSize(200, 20);
    d_retail_price->setText("Оптовая цена:");
    d_retail_price->setStyleSheet("background-color:#fffdf4;");
    retail_price->setFixedSize(150, 20);
    retail_price->setMaximum(std::numeric_limits<double>::max());

    d_trade_price->setFrame(false);
    d_trade_price->setReadOnly(true);
    d_trade_price->setFixedSize(200, 20);
    d_trade_price->setText("Розничная цена:");
    d_trade_price->setStyleSheet("background-color:#fffdf4;");
    trade_price->setFixedSize(150, 20);
    trade_price->setMaximum(std::numeric_limits<double>::max());

    d_count->setFrame(false);
    d_count->setReadOnly(true);
    d_count->setFixedSize(200, 20);
    d_count->setText("Количество/общий вес:");
    d_count->setStyleSheet(this->styleSheet());
    count->setFixedSize(150, 20);
    count->setMaximum(std::numeric_limits<double>::max());

    d_exp_date->setFrame(false);
    d_exp_date->setReadOnly(true);
    d_exp_date->setFixedSize(200, 20);
    d_exp_date->setText("Срок годности:");
    d_exp_date->setStyleSheet("background-color:#fffdf4;");
    exp_date->setFixedSize(150, 20);
    exp_date->setMaximum(std::numeric_limits<int>::max());

    hl->addWidget(actions_btn);
    hl->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hl0->addWidget(d_group);
    hl0->addWidget(group);
    hl0->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hl1->addWidget(d_code);
    hl1->addWidget(code);
    hl1->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hl2->addWidget(d_name);
    hl2->addWidget(name);
    hl2->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hl3->addWidget(d_print_name);
    hl3->addWidget(print_name);
    hl3->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hl4->addWidget(d_barcode);
    hl4->addWidget(barcode);
    hl4->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hl5->addWidget(d_retail_price);
    hl5->addWidget(retail_price);
    hl5->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hl6->addWidget(d_trade_price);
    hl6->addWidget(trade_price);
    hl6->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hl7->addWidget(d_exp_date);
    hl7->addWidget(exp_date);
    hl7->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    qrcode_l->addWidget(d_qrcode);
    qrcode_l->addWidget(qrcode);
    qrcode_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    provider_l->addWidget(d_provider);
    provider_l->addWidget(provider);
    provider_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    count_l->addWidget(d_count);
    count_l->addWidget(count);
    count_l->setAlignment((Qt::AlignLeft | Qt::AlignTop));

    vl->addLayout(hl);
    vl->addLayout(hl0);
    vl->addLayout(hl1);
    vl->addLayout(hl2);
    vl->addLayout(hl3);
    vl->addLayout(hl4);
    vl->addLayout(qrcode_l);
    vl->addLayout(provider_l);
    vl->addLayout(count_l);
    vl->addLayout(hl5);
    vl->addLayout(hl6);
    vl->addLayout(hl7);

    this->setLayout(vl);
    this->setObjectName("prodCard");
    //this->setStyleSheet("QWidget#prodCard{background-color:#fffdf4;}");

    fillFields();

    old_group = group->currentText().toStdWString();

    QObject::connect(save_product, &QAction::triggered, [this](){saveProduct_s();});
    QObject::connect(delete_product, &QAction::triggered, [this](){deleteProduct_s();});
    QObject::connect(edit_product, &QAction::triggered, [this](){editProduct_s();});

    QObject::connect(group, &QComboBox::editTextChanged, this, &form::prodCard::formEdited_s);
    QObject::connect(code, &QLineEdit::editingFinished, this, &form::prodCard::formEdited_s);
    QObject::connect(name, &QLineEdit::editingFinished, this, &form::prodCard::formEdited_s);
    QObject::connect(print_name, &QLineEdit::editingFinished, this, &form::prodCard::formEdited_s);
    QObject::connect(barcode, &QLineEdit::editingFinished, this, &form::prodCard::formEdited_s);
    QObject::connect(retail_price, &QDoubleSpinBox::editingFinished, this, &form::prodCard::formEdited_s);
    QObject::connect(trade_price, &QDoubleSpinBox::editingFinished, this, &form::prodCard::formEdited_s);
    QObject::connect(exp_date, &QSpinBox::editingFinished, this, &form::prodCard::formEdited_s);
}

void form::prodCard::fillFields()
{
    if(product_code.empty())
    {
        formStatus = 3;
        if(data->prodInfo.prodInfo[L"backup_codes"]->vector(tag::vecWStr()).empty())
        {
            code->setText(QString::fromStdWString(data->prodInfo.prodInfo[L"current_code"]->wstring()));
            //data->prodInfo.prodInfo[L"код"]->wstring();
        }
        else
        {
            code->setText(QString::fromStdWString(data->prodInfo.prodInfo[L"backup_codes"]->vector(tag::vecWStr()).back()));
            data->prodInfo.prodInfo[L"backup_codes"]->vector(tag::vecWStr()).pop_back();
        }

        this->setWindowTitle("Карточка товара(новый) ");
    }
    else
    {
        formStatus = 1;

        auto& prodMap = data->prodContain.products[product_group]->map()[product_code]->map();

        this->setWindowTitle("Карточка товара ");

        group->setCurrentText(QString::fromStdWString(prodMap[L"group"]->wstring()));
        code->setText(QString::fromStdWString(prodMap[L"code"]->wstring()));
        name->setText(QString::fromStdWString(prodMap[L"name"]->wstring()));
        print_name->setText(QString::fromStdWString(prodMap[L"print_name"]->wstring()));
        barcode->setText(QString::fromStdWString(prodMap[L"barcode"]->wstring()));
        qrcode->setText(QString::fromStdWString(prodMap[L"qrcode"]->wstring()));
        provider->setText(QString::fromStdWString(prodMap[L"provider"]->wstring()));
        qrcode->setText(QString::fromStdWString(prodMap[L"qrcode"]->wstring()));
        count->setValue(std::stod(prodMap[L"count"]->wstring()));
        retail_price->setValue(std::stod(prodMap[L"retail_price"]->wstring()));
        trade_price->setValue(std::stod(prodMap[L"trade_price"]->wstring()));
        exp_date->setValue(std::stoi(prodMap[L"exp_date"]->wstring()));
    }

    blockEdit();
}

void form::prodCard::blockEdit()
{
    switch(formStatus)
    {
        case 0:
        {
            save_product->setEnabled(true);
            delete_product->setEnabled(true);
            edit_product->setEnabled(false);
            group->setEnabled(true);
            barcode->setReadOnly(false);
            name->setReadOnly(false);
            print_name->setReadOnly(false);
            retail_price->setReadOnly(false);
            trade_price->setReadOnly(false);
            count->setReadOnly(false);
            exp_date->setReadOnly(false);
            provider->setReadOnly(false);
            qrcode->setReadOnly(false);
            break;
        }
        case 1:
        {
            save_product->setEnabled(false);
            delete_product->setEnabled(false);
            edit_product->setEnabled(true);
            group->setEnabled(false);
            barcode->setReadOnly(true);
            name->setReadOnly(true);
            print_name->setReadOnly(true);
            retail_price->setReadOnly(true);
            trade_price->setReadOnly(true);
            count->setReadOnly(true);
            exp_date->setReadOnly(true);
            provider->setReadOnly(true);
            qrcode->setReadOnly(true);
            break;
        }
        case 2:
        {
            save_product->setEnabled(true);
            delete_product->setEnabled(false);
            edit_product->setEnabled(false);
            group->setEnabled(false);
            barcode->setReadOnly(true);
            name->setReadOnly(true);
            print_name->setReadOnly(true);
            retail_price->setReadOnly(true);
            trade_price->setReadOnly(true);
            exp_date->setReadOnly(true);
            count->setReadOnly(true);
            provider->setReadOnly(true);
            qrcode->setReadOnly(true);
            break;
        }
        case 3:
        {
            edit_product->setEnabled(false);
            delete_product->setEnabled(false);
            break;
        }
        default:
        {

        }
    }
}

void form::prodCard::saveProduct_s()
{
    //if(group->currentText().isEmpty())
    //{
    //    return;
    //}
    if(formStatus == 3)
    {
        utl::inc_productCode(data->prodInfo.prodInfo[L"current_code"]->wstring());
    }
    else if(formStatus == 2)
    {
        code->setText(QString::fromStdWString(data->prodInfo.prodInfo[L"backup_codes"]->vector(tag::vecWStr()).back()));
        data->prodInfo.prodInfo[L"backup_codes"]->vector(tag::vecWStr()).pop_back();
    }

    std::wstring key = group->currentText().toStdWString();

    tag::mapWStrVar msv;
    tag::wString ws;

    if (data->prodContain.products.search(key) == data->prodContain.products.end())
    {
        data->prodContain.products.insert({key, new variant(msv)});
    }

    if (data->prodContain.products.search(code->text().toStdWString()) == data->prodContain.products.end())
    {
        data->prodContain.products[key]->map()[code->text().toStdWString()] = new variant(msv);
        auto &map = data->prodContain.products[key]->map()[code->text().toStdWString()]->map();

        map.insert(std::pair<std::wstring, variant*>(L"group", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"code", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"name", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"print_name", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"barcode", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"retail_price", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"trade_price", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"exp_date", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"count", new variant(ws)));
        map.insert({L"qrcode", new variant(ws)});
        map.insert({L"provider", new variant(ws)});
    }

    auto &map = data->prodContain.products[key]->map()[code->text().toStdWString()]->map();

    map[L"group"]->wstring() = group->currentText().toStdWString();
    map[L"code"]->wstring() = code->text().toStdWString();
    map[L"name"]->wstring() = name->text().toStdWString();
    map[L"print_name"]->wstring() = print_name->text().toStdWString();
    map[L"barcode"]->wstring() = barcode->text().toStdWString();
    map[L"retail_price"]->wstring() = std::to_wstring(retail_price->value());
    map[L"trade_price"]->wstring() = std::to_wstring(trade_price->value());
    map[L"exp_date"]->wstring() = std::to_wstring(exp_date->value());
    map[L"count"]->wstring() = count->text().toStdWString();
    map[L"qrcode"]->wstring() = qrcode->text().toStdWString();
    map[L"provider"]->wstring() = provider->text().toStdWString();


    data->prodData.insertProduct("products", {.code{code->text()},
                                                          .group{group->currentText()},
                                                          .provider{provider->text()},
                                                          .name{name->text()},
                                                          .print_name{print_name->text()},
                                                          .barcode{barcode->text()},
                                                          .qrcode{qrcode->text()},
                                                          .count = count->value(),
                                                          .exp_date = exp_date->value(),
                                                          .retail_price = retail_price->value(),
                                                          .trade_price = trade_price->value(),
                                                          .id = code->text().toInt()});

    formStatus = 1;

    blockEdit();

    saveChanges("");

    utl::logger("Товар " + code->text() + " " + name->text() + " сохранён.", log);

    emit productsChanged(group->currentText().toStdWString());

    if(old_group != group->currentText().toStdWString())
    {
        emit productsChanged(old_group);
        old_group = group->currentText().toStdWString();
    }

    formSaved_s();
}

void form::prodCard::deleteProduct_s()
{
    if(data->prodContain.products.search(code->text().toStdWString()) != data->prodContain.products.end())
    {
        data->prodContain.products[old_group]->map().erase(code->text().toStdWString());
    }

    data->prodData.deleteProduct(code->text());

    data->prodInfo.prodInfo[L"backup_codes"]->vector(tag::vecWStr()).push_back(code->text().toStdWString());

    formStatus = 2;

    blockEdit();

    utl::logger("Товар " + code->text() + " " + name->text() + " удален", log);

    emit productsChanged(old_group);

    formSaved_s();

    saveChanges("");
}

void form::prodCard::editProduct_s()
{
    formStatus = 0;

    blockEdit();
}

void form::prodCard::formEdited_s()
{
    this->windowTitle().back() = '*';
}

void form::prodCard::formSaved_s()
{
    this->windowTitle().back() = ' ';
}

void form::prodCard::saveChanges(const std::string& filePath)
{
    data->prodContain.save("../../data/products/products.json");
    data->prodInfo.save("../../data/products/prodInfo.json");
}

form::prodCard* form::make_productCard(QMdiArea* mdiArea, implData* data, QTextBrowser *log, const std::wstring& code,
                                       const std::wstring& group_)
{
    auto subWindow = new QMdiSubWindow(mdiArea);
    auto pC_f = new form::prodCard(data, log, subWindow, code, group_);

    subWindow->setWidget(pC_f);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    pC_f->setupUI();

    mdiArea->addSubWindow(subWindow);

    subWindow->show();

    QObject::connect(pC_f, &QWidget::destroyed, [subWindow, mdiArea]()
    {
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });

    return pC_f;
}
