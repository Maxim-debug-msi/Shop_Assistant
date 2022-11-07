//
// Created by Maxim on 31.10.2022.
//

#include "prod_card.h"

forms::prod_card::prod_card(implData *data_, QTextBrowser * log_, QWidget *parent, const std::wstring& prodCode) :
                            mainWgt(new QWidget(parent)),
                            QObject(nullptr), productCode(new std::wstring(prodCode)), hl(new QHBoxLayout(mainWgt)),
                            data(data_), log(log_), d_group(new QLineEdit(mainWgt)), group(new QComboBox(mainWgt)),
                            d_code(new QLineEdit(mainWgt)), code(new QLineEdit(mainWgt)), d_name(new QLineEdit(mainWgt)),
                            name(new QLineEdit(mainWgt)), d_printName(new QLineEdit(mainWgt)), printName(new QLineEdit(mainWgt)),
                            d_barCode(new QLineEdit(mainWgt)), barCode(new QLineEdit(mainWgt)), d_oPrice(new QLineEdit(mainWgt)),
                            oPrice(new QDoubleSpinBox(mainWgt)), d_rPrice(new QLineEdit(mainWgt)), rPrice(new QDoubleSpinBox(mainWgt)),
                            d_expDate(new QLineEdit(mainWgt)), expDate(new QSpinBox(mainWgt)), actions_btn(new QToolButton(mainWgt)),
                            actions(new QMenu(actions_btn)), saveProduct(new QAction("Завести товар", actions)),
                            deleteProduct(new QAction("Удалить товар", actions)), editProduct(new QAction("Редактировать")),
                            vl(new QVBoxLayout(mainWgt)), hl0(new QHBoxLayout(mainWgt)), hl1(new QHBoxLayout(mainWgt)),
                            hl2(new QHBoxLayout(mainWgt)), hl3(new QHBoxLayout(mainWgt)), hl4(new QHBoxLayout(mainWgt)),
                            hl5(new QHBoxLayout(mainWgt)), hl6(new QHBoxLayout(mainWgt)), hl7(new QHBoxLayout(mainWgt)),
                            oT_model(new models::objectTree_model(group)), formStatus(nullptr), rootModelObj(new QObject(mainWgt)){}

forms::prod_card::~prod_card()
{
    if(*formStatus == 0)
    {
        data->prodInfo.prodInfo[L"РезервныйКод"]->get_vector().push_back(code->text().toStdWString());
    }
    delete oT_model;
    delete productCode;
    delete formStatus;
}

void forms::prod_card::setupUI()
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

    extractor(data->prodInfo.prodInfo.get()[L"Группы"]->get_map());

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

    mainWgt->setLayout(vl);

    fillFields();

    QObject::connect(saveProduct, &QAction::triggered, [this](){saveProduct_s();});
    QObject::connect(deleteProduct, &QAction::triggered, [this](){deleteProduct_s();});
    QObject::connect(editProduct, &QAction::triggered, [this](){editProduct_s();});
}

void forms::prod_card::fillFields()
{
    if(productCode->empty())
    {
        formStatus = new int{3};
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

        mainWgt->setWindowTitle("Карточка товара(новый) ");
    }
    else
    {
        formStatus = new int{1};
        auto prodMap = data->prodContain.products.find(*productCode)->second->get_map();

        mainWgt->setWindowTitle("Карточка товара ");

        group->findText(QString::fromStdWString(prodMap[L"группа"]->get_wstring()));
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

void forms::prod_card::blockEdit()
{
    switch(*formStatus)
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

void forms::prod_card::saveProduct_s()
{
    if(*formStatus == 3)
    {
        data->prodInfo.prodInfo[L"Код"]->incWStr();
    }

    std::wstring key = group->currentText().toStdWString();

    tag::mapWStrVar msv;
    tag::wString ws;

    if (data->prodContain.products.find(key) == data->prodContain.products.end())
    {
        data->prodContain.products.get()[key] = new variant(msv);
    }

    if (data->prodContain.products.find(code->text().toStdWString()) == data->prodContain.products.end())
    {
        data->prodContain.products.get()[key]->get_map()[code->text().toStdWString()] = new variant(msv);
        auto &map = data->prodContain.products.get()[key]->get_map()[code->text().toStdWString()]->get_map();

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

    auto &map = data->prodContain.products.get()[key]->get_map()[code->text().toStdWString()]->get_map();

    map[L"группа"]->set_wstring(group->currentText().toStdWString());
    map[L"код"]->set_wstring(code->text().toStdWString());
    map[L"имя"]->set_wstring(name->text().toStdWString());
    map[L"имя_печать"]->set_wstring(printName->text().toStdWString());
    map[L"шк"]->set_wstring(barCode->text().toStdWString());
    map[L"о_цена"]->set_wstring(std::to_wstring(oPrice->value()));
    map[L"р_цена"]->set_wstring(std::to_wstring(rPrice->value()));
    map[L"сг"]->set_wstring(std::to_wstring(expDate->value()));
    map[L"количество"]->set_wstring(L"0");

    *formStatus = 1;

    blockEdit();

    data->prodContain.save("../../bin/products.json");

    MainWindow::logger("Товар " + code->text() + " " + name->text() + " заведён", log);

    emit productAdded(group->currentText().toStdWString(), code->text().toStdWString());
}

void forms::prod_card::deleteProduct_s()
{
    if(data->prodContain.products.find(code->text().toStdWString()) != data->prodContain.products.end())
    {
        data->prodContain.products[group->currentText().toStdWString()]->get_map().erase(code->text().toStdWString());
    }

    *formStatus = 2;

    blockEdit();

    MainWindow::logger("Товар " + code->text() + " " + name->text() + " удален", log);
}

void forms::prod_card::editProduct_s()
{
    *formStatus = 0;

    blockEdit();
}

void forms::make_productCard(QMdiArea* mdiArea, implData* data, QTextBrowser *log,
                             std::map<std::wstring, QTableWidget*>* tables, const std::wstring& code)
{
    auto subWindow = new QMdiSubWindow(mdiArea);
    auto pC_f = new forms::prod_card(data, log, subWindow, code);

    subWindow->setWidget(pC_f->mainWgt);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    pC_f->setupUI();

    mdiArea->addSubWindow(subWindow);

    subWindow->show();

    if(tables != nullptr)
    {
        QObject::connect(pC_f, &forms::prod_card::productAdded, [tables, data](const std::wstring& group, const std::wstring& code)
        {
            tables->operator[](group)->setRowCount(tables->operator[](group)->rowCount() + 1);

            int row = tables->operator[](group)->rowCount() - 1;
            auto& product = data->prodContain.products[group]->get_map()[code]->get_map();

            tables->operator[](group)->setItem(row, 0, new QTableWidgetItem(QString::fromStdWString(product[L"код"]->get_wstring())));
            tables->operator[](group)->item(row, 0)->setFlags(tables->operator[](group)->item(row, 0)->flags() ^ Qt::ItemIsEditable);

            tables->operator[](group)->setItem(row, 1, new QTableWidgetItem(QString::fromStdWString(product[L"имя"]->get_wstring())));
            tables->operator[](group)->item(row, 1)->setFlags(tables->operator[](group)->item(row, 1)->flags() ^ Qt::ItemIsEditable);

            tables->operator[](group)->setItem(row, 2, new QTableWidgetItem(QString::fromStdWString(product[L"количество"]->get_wstring())));
            tables->operator[](group)->item(row, 2)->setFlags(tables->operator[](group)->item(row, 2)->flags() ^ Qt::ItemIsEditable);

            tables->operator[](group)->setItem(row, 3, new QTableWidgetItem(QString::fromStdWString(product[L"р_цена"]->get_wstring())));
            tables->operator[](group)->item(row, 3)->setFlags(tables->operator[](group)->item(row, 3)->flags() ^ Qt::ItemIsEditable);
        });

        QObject::connect(pC_f, &forms::prod_card::productDeleted, [tables, data](const std::wstring& group, const std::wstring& code)
        {
            data->prodContain.products[group]->get_map().erase(code);
            tables->operator[](group)->findItems(QString::fromStdWString(code), QFlags<Qt::MatchFlag>()).first();
        });
    }

    QObject::connect(pC_f->mainWgt, &QWidget::destroyed, [pC_f, subWindow, mdiArea]()
    {
        delete pC_f;
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });
}
