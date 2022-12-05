//
// Created by Maxim on 31.10.2022.
//

#include "prodCard.h"

form::prodCard::prodCard(implData *data_, QTextBrowser * log_, QWidget *parent, std::wstring  prodCode, std::wstring  prodGroup) :
                            productGroup{std::move(prodGroup)},
                            QWidget(parent), productCode{std::move(prodCode)}, hl(new QHBoxLayout(this)),
                            data(data_), log(log_), d_group(new QLineEdit(this)), group(new QComboBox(this)),
                            d_code(new QLineEdit(this)), code(new QLineEdit(this)), d_name(new QLineEdit(this)),
                            name(new QLineEdit(this)), d_printName(new QLineEdit(this)), printName(new QLineEdit(this)),
                            d_barCode(new QLineEdit(this)), barCode(new QLineEdit(this)), d_oPrice(new QLineEdit(this)),
                            oPrice(new QDoubleSpinBox(this)), d_rPrice(new QLineEdit(this)), rPrice(new QDoubleSpinBox(this)),
                            d_expDate(new QLineEdit(this)), expDate(new QSpinBox(this)), actions_btn(new QToolButton(this)),
                            actions(new QMenu(actions_btn)), saveProduct(new QAction("Сохранить", actions)),
                            deleteProduct(new QAction("Удалить", actions)), editProduct(new QAction("Редактировать")),
                            vl(new QVBoxLayout(this)), hl0(new QHBoxLayout(this)), hl1(new QHBoxLayout(this)),
                            hl2(new QHBoxLayout(this)), hl3(new QHBoxLayout(this)), hl4(new QHBoxLayout(this)),
                            hl5(new QHBoxLayout(this)), hl6(new QHBoxLayout(this)), hl7(new QHBoxLayout(this)),
                            oT_model(new models::objectTree_model(group)), rootModelObj(new QObject(this)){}

void form::prodCard::setupUI()
{
    std::function<void(std::unordered_map<std::wstring, variant*>&)> extractor =
            [&extractor, this](std::unordered_map<std::wstring, variant*>& map)
            {
                for(auto&& it : map)
                {
                    auto obj = new QObject(rootModelObj);
                    obj->setObjectName(QString::fromStdWString(it.first));

                    oT_model->addItem(obj, QModelIndex());

                    if(it.second != nullptr && it.second->is_map() && !it.second->is_empty_map())
                    {
                        extractor(it.second->get_map());
                    }
                }
            };

    QStringList cols;
    cols << "objectName";
    oT_model->setColumns(cols);

    extractor(data->prodInfo.prodInfo[L"Группы"]->get_map());

    group->setModel(oT_model);

    actions->addAction(editProduct);
    actions->addAction(saveProduct);
    actions->addAction(deleteProduct);

    actions_btn->setMenu(actions);
    actions_btn->setPopupMode(QToolButton::InstantPopup);
    actions_btn->setText("Действия");
    actions_btn->setFixedSize(100, 30);
    actions_btn->setArrowType(Qt::ArrowType::RightArrow);
    actions_btn->setToolButtonStyle(Qt::ToolButtonTextOnly);

    d_group->setFrame(false);
    d_group->setReadOnly(true);
    d_group->setFixedSize(270, 20);
    d_group->setText("Группа:");
    d_group->setReadOnly(true);
    group->setFixedSize(150, 20);

    d_code->setFrame(false);
    d_code->setReadOnly(true);
    d_code->setFixedSize(270, 20);
    d_code->setText("Код:");
    d_code->setReadOnly(true);
    code->setFixedSize(150, 20);
    code->setReadOnly(true);

    d_name->setFrame(false);
    d_name->setReadOnly(true);
    d_name->setFixedSize(270, 20);
    d_name->setText("Наименование:");
    d_name->setReadOnly(true);
    name->setFixedSize(150, 20);

    d_printName->setFrame(false);
    d_printName->setReadOnly(true);
    d_printName->setFixedSize(270, 20);
    d_printName->setText("Наименование(для печати):");
    d_printName->setReadOnly(true);
    printName->setFixedSize(150, 20);

    d_barCode->setFrame(false);
    d_barCode->setReadOnly(true);
    d_barCode->setFixedSize(270, 20);
    d_barCode->setText("Штрих-код:");
    d_barCode->setReadOnly(true);
    barCode->setFixedSize(150, 20);

    d_oPrice->setFrame(false);
    d_oPrice->setReadOnly(true);
    d_oPrice->setFixedSize(270, 20);
    d_oPrice->setText("Оптовая цена:");
    d_oPrice->setReadOnly(true);
    oPrice->setFixedSize(150, 20);
    oPrice->setMaximum(std::numeric_limits<double>::max());

    d_rPrice->setFrame(false);
    d_rPrice->setReadOnly(true);
    d_rPrice->setFixedSize(270, 20);
    d_rPrice->setText("Розничная цена:");
    d_rPrice->setReadOnly(true);
    rPrice->setFixedSize(150, 20);
    rPrice->setMaximum(std::numeric_limits<double>::max());

    d_expDate->setFrame(false);
    d_expDate->setReadOnly(true);
    d_expDate->setFixedSize(270, 20);
    d_expDate->setText("Срок годности:");
    d_expDate->setReadOnly(true);
    expDate->setFixedSize(150, 20);
    expDate->setMaximum(std::numeric_limits<int>::max());

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

    hl3->addWidget(d_printName);
    hl3->addWidget(printName);
    hl3->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hl4->addWidget(d_barCode);
    hl4->addWidget(barCode);
    hl4->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hl5->addWidget(d_oPrice);
    hl5->addWidget(oPrice);
    hl5->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hl6->addWidget(d_rPrice);
    hl6->addWidget(rPrice);
    hl6->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hl7->addWidget(d_expDate);
    hl7->addWidget(expDate);
    hl7->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    vl->addLayout(hl);
    vl->addLayout(hl0);
    vl->addLayout(hl1);
    vl->addLayout(hl2);
    vl->addLayout(hl3);
    vl->addLayout(hl4);
    vl->addLayout(hl5);
    vl->addLayout(hl6);
    vl->addLayout(hl7);

    this->setLayout(vl);

    fillFields();

    oldGroup = group->currentText().toStdWString();

    QObject::connect(saveProduct, &QAction::triggered, [this](){saveProduct_s();});
    QObject::connect(deleteProduct, &QAction::triggered, [this](){deleteProduct_s();});
    QObject::connect(editProduct, &QAction::triggered, [this](){editProduct_s();});

    QObject::connect(group, &QComboBox::editTextChanged, this, &form::prodCard::formEdited_s);
    QObject::connect(code, &QLineEdit::editingFinished, this, &form::prodCard::formEdited_s);
    QObject::connect(name, &QLineEdit::editingFinished, this, &form::prodCard::formEdited_s);
    QObject::connect(printName, &QLineEdit::editingFinished, this, &form::prodCard::formEdited_s);
    QObject::connect(barCode, &QLineEdit::editingFinished, this, &form::prodCard::formEdited_s);
    QObject::connect(oPrice, &QDoubleSpinBox::editingFinished, this, &form::prodCard::formEdited_s);
    QObject::connect(rPrice, &QDoubleSpinBox::editingFinished, this, &form::prodCard::formEdited_s);
    QObject::connect(expDate, &QSpinBox::editingFinished, this, &form::prodCard::formEdited_s);
}

void form::prodCard::fillFields()
{
    if(productCode.empty())
    {
        formStatus = 3;
        if(data->prodInfo.prodInfo[L"РезервныйКод"]->get_vector().empty())
        {
            code->setText(QString::fromStdWString(data->prodInfo.prodInfo[L"Код"]->get_wstring()));
            data->prodInfo.prodInfo[L"Код"]->get_wstring();
        }
        else
        {
            code->setText(QString::fromStdWString(data->prodInfo.prodInfo[L"РезервныйКод"]->get_vector().back()));
            data->prodInfo.prodInfo[L"РезервныйКод"]->get_vector().pop_back();
        }

        this->setWindowTitle("Карточка товара(новый) ");
    }
    else
    {
        formStatus = 1;

        auto& prodMap = data->prodContain.products[productGroup]->get_map()[productCode]->get_map();

        this->setWindowTitle("Карточка товара ");

        group->setCurrentText(QString::fromStdWString(prodMap[L"группа"]->get_wstring()));
        code->setText(QString::fromStdWString(prodMap[L"код"]->get_wstring()));
        name->setText(QString::fromStdWString(prodMap[L"имя"]->get_wstring()));
        printName->setText(QString::fromStdWString(prodMap[L"имя_печать"]->get_wstring()));
        barCode->setText(QString::fromStdWString(prodMap[L"шк"]->get_wstring()));
        oPrice->setValue(std::stod(prodMap[L"о_цена"]->get_wstring()));
        rPrice->setValue(std::stod(prodMap[L"р_цена"]->get_wstring()));
        expDate->setValue(std::stoi(prodMap[L"сг"]->get_wstring()));
    }

    blockEdit();
}

void form::prodCard::blockEdit()
{
    switch(formStatus)
    {
        case 0:
        {
            saveProduct->setEnabled(true);
            deleteProduct->setEnabled(true);
            editProduct->setEnabled(false);
            group->setEnabled(true);
            barCode->setReadOnly(false);
            name->setReadOnly(false);
            printName->setReadOnly(false);
            oPrice->setReadOnly(false);
            rPrice->setReadOnly(false);
            expDate->setReadOnly(false);
            break;
        }
        case 1:
        {
            saveProduct->setEnabled(false);
            deleteProduct->setEnabled(false);
            editProduct->setEnabled(true);
            group->setEnabled(false);
            barCode->setReadOnly(true);
            name->setReadOnly(true);
            printName->setReadOnly(true);
            oPrice->setReadOnly(true);
            rPrice->setReadOnly(true);
            expDate->setReadOnly(true);
            break;
        }
        case 2:
        {
            saveProduct->setEnabled(true);
            deleteProduct->setEnabled(false);
            editProduct->setEnabled(false);
            group->setEnabled(false);
            barCode->setReadOnly(true);
            name->setReadOnly(true);
            printName->setReadOnly(true);
            oPrice->setReadOnly(true);
            rPrice->setReadOnly(true);
            expDate->setReadOnly(true);
            break;
        }
        case 3:
        {
            editProduct->setEnabled(false);
            deleteProduct->setEnabled(false);
            break;
        }
        default:
        {

        }
    }
}

void form::prodCard::saveProduct_s()
{
    if(formStatus == 3)
    {
        data->prodInfo.prodInfo[L"Код"]->incWStr();
    }
    else if(formStatus == 2)
    {
        code->setText(QString::fromStdWString(data->prodInfo.prodInfo[L"РезервныйКод"]->get_vector().back()));
        data->prodInfo.prodInfo[L"РезервныйКод"]->get_vector().pop_back();
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
        data->prodContain.products[key]->get_map()[code->text().toStdWString()] = new variant(msv);
        auto &map = data->prodContain.products[key]->get_map()[code->text().toStdWString()]->get_map();

        map.insert(std::pair<std::wstring, variant*>(L"группа", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"код", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"имя", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"имя_печать", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"шк", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"р_цена", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"о_цена", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"сг", new variant(ws)));
        map.insert(std::pair<std::wstring, variant*>(L"количество", new variant(ws)));
    }

    auto &map = data->prodContain.products[key]->get_map()[code->text().toStdWString()]->get_map();

    map[L"группа"]->set_wstring(group->currentText().toStdWString());
    map[L"код"]->set_wstring(code->text().toStdWString());
    map[L"имя"]->set_wstring(name->text().toStdWString());
    map[L"имя_печать"]->set_wstring(printName->text().toStdWString());
    map[L"шк"]->set_wstring(barCode->text().toStdWString());
    map[L"о_цена"]->set_wstring(std::to_wstring(oPrice->value()));
    map[L"р_цена"]->set_wstring(std::to_wstring(rPrice->value()));
    map[L"сг"]->set_wstring(std::to_wstring(expDate->value()));
    map[L"количество"]->set_wstring(L"0");

    formStatus = 1;

    blockEdit();

    saveChanges("");

    utl::logger("Товар " + code->text() + " " + name->text() + " сохранён", log);

    emit productsChanged(group->currentText().toStdWString());

    if(oldGroup != group->currentText().toStdWString())
    {
        emit productsChanged(oldGroup);
        oldGroup = group->currentText().toStdWString();
    }

    formSaved_s();
}

void form::prodCard::deleteProduct_s()
{
    if(data->prodContain.products.search(code->text().toStdWString()) != data->prodContain.products.end())
    {
        data->prodContain.products[oldGroup]->get_map().erase(code->text().toStdWString());
    }

    data->prodInfo.prodInfo[L"РезервныйКод"]->get_vector().push_back(code->text().toStdWString());

    formStatus = 2;

    blockEdit();

    utl::logger("Товар " + code->text() + " " + name->text() + " удален", log);

    emit productsChanged(oldGroup);

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
        //delete pC_f;
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });

    return pC_f;
}
