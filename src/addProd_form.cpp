//
// Created by Maxim on 05.09.2022.
//

#include "addProd_form.h"
#include "ioWCharTDocs.hpp"
#include "UI/MainWindow.h"

forms::addProduct_form::addProduct_form(const std::wstring& docNum, QWidget* parent) : QObject(nullptr),
                           mainWgt(new QWidget(parent)), docNumber(new std::wstring(docNum)),
                           groupList(new QComboBox(mainWgt)), summaryDate(new QLineEdit(mainWgt)),
                           code(new QLineEdit(mainWgt)), name(new QLineEdit(mainWgt)), count(new QDoubleSpinBox(mainWgt)),
                           optPrice(new QDoubleSpinBox(mainWgt)), expirationDate(new QSpinBox(mainWgt)),
                           vl(new QVBoxLayout(mainWgt)), hl1(new QHBoxLayout(mainWgt)), hl2(new QHBoxLayout(mainWgt)),
                           hl3(new QHBoxLayout),
                           hl4(new QHBoxLayout(mainWgt)), hl5(new QHBoxLayout(mainWgt)), hl6(new QHBoxLayout(mainWgt)),
                           enterProd_btn(new QPushButton(mainWgt)),
                           data(nullptr), oT_model(new models::objectTree_model(groupList)), deleteProd_btn(new QPushButton(mainWgt)),
                           summaryGL(new QLineEdit(mainWgt)), summaryCode(new QLineEdit(mainWgt)), isHasEdit(true),
                           summaryCount(new QLineEdit(mainWgt)), summaryName(new QLineEdit(mainWgt)),
                           summaryPrice(new QLineEdit(mainWgt)), summaryRPrice(new QLineEdit(mainWgt)),
                           rPrice(new QDoubleSpinBox(mainWgt)), EANCode(new QLineEdit(mainWgt)), log(nullptr),
                           summaryEANCode(new QLineEdit(mainWgt)), hl7(new QHBoxLayout(mainWgt)), summaryDocNum(new QLineEdit(mainWgt)),
                           docNum(new QLineEdit(mainWgt)), summaryDocDate(new QLineEdit(mainWgt)),
                           docDate(new QLineEdit(mainWgt)), hl13(new QHBoxLayout(mainWgt)),
                           status(new QLineEdit(mainWgt)), creator(new QLineEdit(mainWgt)), hl9(new QHBoxLayout(mainWgt)),
                           hl8(new QHBoxLayout(mainWgt)), hl10(new QHBoxLayout(mainWgt)), hl11(new QHBoxLayout(mainWgt)),
                           menu(new QMenu()),
                           hl12(new QHBoxLayout), writeProd_btn(new QPushButton(mainWgt)), actions_btn(new QToolButton(mainWgt)),
                           remEnter_act(new QAction("Отмена проведения", mainWgt)), nameBuf(new std::wstring),
                           remDel_act(new QAction("Снять пометку удаления", mainWgt)), hl14(new QHBoxLayout),
                           summaryComment(new QLineEdit(mainWgt)), comment(new QLineEdit(mainWgt))
                           {
                                mainWgt->setAttribute(Qt::WA_DeleteOnClose);
                           }

forms::addProduct_form::~addProduct_form()
{
    delete time;
    delete nameBuf;
    delete oT_model;
    delete docNumber;
}

void forms::addProduct_form::setDataPtr(implData* data_)
{
    data = data_;
}

void forms::addProduct_form::setupUI()
{
    std::function<void(std::unordered_map<std::wstring, variant*>&)> extractor =
            [&extractor, this](std::unordered_map<std::wstring, variant*>& map)
            {
                for(auto&& it : map)
                {
                    modelWidgets.push_back(new QObject);
                    modelWidgets.last()->setObjectName(QString::fromStdWString(it.first));

                    oT_model->addItem(modelWidgets.last(), QModelIndex());

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

    groupList->setModel(oT_model);

    enterProd_btn->setText("Провести");

    writeProd_btn->setText("Записать");

    deleteProd_btn->setText("Удалить");

    actions_btn->setText("Действия");
    actions_btn->setFixedSize(100, 30);
    actions_btn->setArrowType(Qt::ArrowType::RightArrow);
    actions_btn->setToolButtonStyle(Qt::ToolButtonTextOnly);

    menu->addAction(remEnter_act);
    menu->addAction(remDel_act);

    actions_btn->setMenu(menu);
    actions_btn->setPopupMode(QToolButton::InstantPopup);

    summaryDocNum->setFrame(false);
    summaryDocNum->setReadOnly(true);
    summaryDocNum->setFixedSize(270, 20);
    summaryDocNum->setText("Номер документа:");
    docNum->setReadOnly(true);
    docNum->setFixedSize(150, 20);

    summaryDocDate->setFrame(false);
    summaryDocDate->setReadOnly(true);
    summaryDocDate->setFixedSize(150, 20);
    summaryDocDate->setText("Дата документа:");
    docDate->setReadOnly(true);
    docDate->setFixedSize(150, 20);

    summaryGL->setText("Выберите группу товара:");
    summaryGL->setFixedSize(270, 20);
    summaryGL->setReadOnly(true);
    summaryGL->setFrame(false);
    groupList->setFixedSize(170, 20);

    summaryCode->setText("Код товара(присваивается автоматически):");
    summaryCode->setFixedSize(270, 20);
    summaryCode->setReadOnly(true);
    summaryCode->setFrame(false);
    code->setReadOnly(true);
    code->setFixedSize(80, 20);

    summaryEANCode->setText("Укажите EAN код:");
    summaryEANCode->setFixedSize(270, 20);
    summaryEANCode->setReadOnly(true);
    summaryEANCode->setFrame(false);

    summaryName->setText("Укажите название товара:");
    summaryName->setFixedSize(270, 20);
    summaryName->setReadOnly(true);
    summaryName->setFrame(false);

    summaryCount->setText("Укажите колличество\\вес(необязательно):");
    summaryCount->setFixedSize(270, 20);
    summaryCount->setReadOnly(true);
    summaryCount->setFrame(false);
    count->setFixedSize(80, 20);
    count->setMaximum(std::numeric_limits<double>::max());

    summaryPrice->setText("Укажите закупочную цену:");
    summaryPrice->setFixedSize(270, 20);
    summaryPrice->setReadOnly(true);
    summaryPrice->setFrame(false);
    optPrice->setFixedSize(80, 20);
    optPrice->setMaximum(std::numeric_limits<double>::max());

    summaryRPrice->setText("Укажите розничную цену:");
    summaryRPrice->setFixedSize(270, 20);
    summaryRPrice->setReadOnly(true);
    summaryRPrice->setFrame(false);
    rPrice->setFixedSize(80, 20);
    rPrice->setMaximum(std::numeric_limits<double>::max());

    summaryDate->setText("Укажите срок годности(в мес.):");
    summaryDate->setFixedSize(270, 20);
    summaryDate->setReadOnly(true);
    summaryDate->setFrame(false);
    expirationDate->setFixedSize(80, 20);
    expirationDate->setMaximum(std::numeric_limits<int>::max());

    summaryComment->setText("Комментарий:");
    summaryComment->setFixedSize(270, 20);
    summaryComment->setReadOnly(true);
    summaryComment->setFrame(false);

    status->setFrame(false);
    status->setReadOnly(true);
    status->setFixedSize(60, 20);
    status->setText("Новый");

    creator->setFrame(false);
    creator->setReadOnly(true);
    creator->setFixedSize(270, 20);
    creator->setText("Семичкин М.А.");

    hl9->addWidget(summaryDocNum);
    hl9->addWidget(docNum);

    hl13->addWidget(summaryDocDate);
    hl13->addWidget(docDate);

    hl10->addWidget(status);
    hl11->addWidget(creator);

    hl8->addLayout(hl9);
    hl8->addLayout(hl13);
    hl8->addLayout(hl10);
    hl8->addLayout(hl11);

    hl1->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl1->addWidget(summaryGL);
    hl1->addWidget(groupList);

    hl2->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl2->addWidget(summaryCode);
    hl2->addWidget(code);

    hl7->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl7->addWidget(summaryEANCode);
    hl7->addWidget(EANCode);

    hl3->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl3->addWidget(summaryName);
    hl3->addWidget(name);

    hl4->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl4->addWidget(summaryCount);
    hl4->addWidget(count);

    hl5->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl5->addWidget(summaryPrice);
    hl5->addWidget(optPrice);
    hl5->addWidget(summaryRPrice);
    hl5->addWidget(rPrice);

    hl6->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl6->addWidget(summaryDate);
    hl6->addWidget(expirationDate);

    hl12->addWidget(deleteProd_btn);
    hl12->addWidget(writeProd_btn);
    hl12->addWidget(enterProd_btn);
    hl12->addWidget(actions_btn);

    hl14->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl14->addWidget(summaryComment);
    hl14->addWidget(comment);

    vl->addLayout(hl8);
    vl->addLayout(hl1);
    vl->addLayout(hl2);
    vl->addLayout(hl7);
    vl->addLayout(hl3);
    vl->addLayout(hl4);
    vl->addLayout(hl5);
    vl->addLayout(hl6);
    vl->addLayout(hl14);
    vl->addLayout(hl12);

    fillFields(*docNumber);

    QObject::connect(EANCode, SIGNAL(editingFinished()), this, SLOT(editedForm()));
    QObject::connect(groupList, SIGNAL(currentTextChanged(QString)), this, SLOT(editedForm()));
    QObject::connect(name, SIGNAL(editingFinished()), this, SLOT(editedForm()));
    QObject::connect(count, SIGNAL(editingFinished()), this, SLOT(editedForm()));
    QObject::connect(rPrice, SIGNAL(editingFinished()), this, SLOT(editedForm()));
    QObject::connect(optPrice, SIGNAL(editingFinished()), this, SLOT(editedForm()));
    QObject::connect(expirationDate, SIGNAL(editingFinished()), this, SLOT(editedForm()));
    QObject::connect(comment, SIGNAL(editingFinished()), this, SLOT(editedForm()));

    connect(enterProd_btn, &QPushButton::clicked, [this]()
    {
        addProduct();
        MainWindow::logger("Документ " + QString::fromStdWString(*nameBuf) + " проведён.", log);
        emit docStatusChange(true);
    });

    QObject::connect(writeProd_btn, &QPushButton::clicked, [this]()
    {
        if(isHasEdit)
        {
            if(status->text() == "Новый")
            {
                data->addProdDoc.doc.docs[L"Номер"]->incWStr();
                data->prodInfo.prodInfo[L"Код"]->incWStr();
            }

            status->setText("Записан");

            saveDoc();

            blockEdit();

            uneditedForm();

            emit docStatusChange(true);

            MainWindow::logger("Документ " + QString::fromStdWString(*nameBuf) + " сохранен.", log);
        }
        else
        {
            MainWindow::logger("Документ не модифицирован, нет смысла в повторном сохранении.", log);
        }
    });

    QObject::connect(deleteProd_btn, &QPushButton::clicked, [this]()
    {
        if(data->prodContain.products.find(code->text().toStdWString()) != data->prodContain.products.end())
        {
            data->prodContain.products[groupList->currentText().toStdWString()]->get_map().erase(code->text().toStdWString());
        }
        data->addProdDoc.doc.docs[L"Документы"]->get_map()[creator->text().toStdWString()]->get_map()
        [std::to_wstring(*time)]->get_map()[L"Статус"]->set_wstring(L"Удалён");

        status->setText("Удалён");

        blockEdit();

        uneditedForm();

        MainWindow::logger("Документ " + QString::fromStdWString(*nameBuf) + " удалён.", log);

        emit docStatusChange(true);
    });

    QObject::connect(remEnter_act, &QAction::triggered, this, [this]()
    {
        data->prodContain.products[groupList->currentText().toStdWString()]->get_map().erase(code->text().toStdWString());

        data->addProdDoc.doc.docs[L"Документы"]->get_map()[creator->text().toStdWString()]->get_map()
        [std::to_wstring(*time)]->get_map()[L"Статус"]->set_wstring(L"Записан");
        status->setText("Записан");

        blockEdit();

        emit docStatusChange(true);
    });

    QObject::connect(remDel_act, &QAction::triggered, this, [this]()
    {
        data->addProdDoc.doc.docs[L"Документы"]->get_map()[creator->text().toStdWString()]->get_map()
        [std::to_wstring(*time)]->get_map()[L"Статус"]->set_wstring(L"Записан");
        status->setText("Записан");

        blockEdit();

        emit docStatusChange(true);
    });
}

void forms::addProduct_form::saveDoc()
{
    tag::mapWStrVar msv;
    tag::wString ws;

    if(data->addProdDoc.doc.docs.find(creator->text().toStdWString()) == data->addProdDoc.doc.docs.end())
    {
        data->addProdDoc.doc.docs[L"Документы"]->get_map().insert({creator->text().toStdWString(), new variant(msv)});
    }
    if(data->addProdDoc.doc.docs[L"Документы"]->get_map()[creator->text().toStdWString()]->get_map().
    find(std::to_wstring(*time)) == data->addProdDoc.doc.docs.end())
    {
        data->addProdDoc.doc.docs[L"Документы"]->get_map()[creator->text().toStdWString()]->get_map().insert
        ({std::to_wstring(*time), new variant(msv)});

        auto& map = data->addProdDoc.doc.docs[L"Документы"]->get_map()[creator->text().toStdWString()]->get_map()[std::to_wstring(*time)]->get_map();

        map.insert({L"дата_ид", new variant(ws)});
        map.insert({L"Номер", new variant(ws)});
        map.insert({L"Код", new variant(ws)});
        map.insert({L"Дата", new variant(ws)});
        map.insert({L"Группа", new variant(ws)});
        map.insert({L"EAN", new variant(ws)});
        map.insert({L"Количество", new variant(ws)});
        map.insert({L"р_цена", new variant(ws)});
        map.insert({L"о_цена", new variant(ws)});
        map.insert({L"СГ", new variant(ws)});
        //map.insert({L"Окончание СГ", new variant(ws)});
        map.insert({L"Статус", new variant(ws)});
        map.insert({L"Автор", new variant(ws)});
        map.insert({L"Имя", new variant(ws)});
        map.insert({L"Комментарий", new variant(ws)});

        data->docsPtr[L"Заведение товара"]->docsOfCreator[creator->text().toStdWString()].insert
                (std::begin(data->docsPtr[L"Заведение товара"]->docsOfCreator[creator->text().toStdWString()]), *time);

        data->docsPtr[L"Заведение товара"]->docsOfDate.insert(std::begin(data->docsPtr[L"Заведение товара"]->docsOfDate), *time);
    }

    auto& map = data->addProdDoc.doc.docs[L"Документы"]->get_map()[creator->text().toStdWString()]->get_map()[std::to_wstring(*time)]->get_map();

    map[L"дата_ид"]->set_wstring(std::to_wstring(*time));

    map[L"Номер"]->set_wstring(docNum->text().toStdWString());

    map[L"Код"]->set_wstring(code->text().toStdWString());

    map[L"Дата"]->set_wstring(docDate->text().toStdWString());

    map[L"Группа"]->set_wstring(groupList->currentText().toStdWString());

    map[L"EAN"]->set_wstring(EANCode->text().toStdWString());

    map[L"Количество"]->set_wstring(count->text().toStdWString());

    map[L"р_цена"]->set_wstring(rPrice->text().toStdWString());

    map[L"о_цена"]->set_wstring(optPrice->text().toStdWString());

    map[L"СГ"]->set_wstring(expirationDate->text().toStdWString());

    //map[L"Окончание СГ"]->set_wstring(endDate->text().toStdWString());

    map[L"Статус"]->set_wstring(status->text().toStdWString());

    map[L"Автор"]->set_wstring(creator->text().toStdWString());

    map[L"Имя"]->set_wstring(name->text().toStdWString());

    map[L"Комментарий"]->set_wstring(comment->text().toStdWString());
}

void forms::addProduct_form::setLog_ptr(QTextBrowser* sM)
{
    log = sM;
}

void forms::addProduct_form::editedForm()
{
    if(!isHasEdit)
    {
        isHasEdit = true;
        nameBuf->push_back(L'*');
        mainWgt->setWindowTitle(QString::fromStdWString(*nameBuf));
    }
}

void forms::addProduct_form::blockFieldsEdit(const bool& flag)
{
    groupList->setEditable(!flag);
    EANCode->setReadOnly(flag);
    name->setReadOnly(flag);
    count->setReadOnly(flag);
    rPrice->setReadOnly(flag);
    optPrice->setReadOnly(flag);
    expirationDate->setReadOnly(flag);
    comment->setReadOnly(flag);
}

void forms::addProduct_form::uneditedForm()
{
    if(isHasEdit)
    {
        isHasEdit = false;
        nameBuf->pop_back();
        mainWgt->setWindowTitle(QString::fromStdWString(*nameBuf));
    }
}

void forms::addProduct_form::addProduct()
{
    if(status->text() == "Новый")
    {
        data->addProdDoc.doc.docs[L"Номер"]->incWStr();
        data->prodInfo.prodInfo[L"Код"]->incWStr();
    }

    std::wstring key = groupList->currentText().toStdWString();

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

        map.insert(std::pair<std::wstring, variant*>(L"Группа", new variant(ws)));

        map.insert(std::pair<std::wstring, variant*>(L"EAN", new variant(ws)));

        map.insert(std::pair<std::wstring, variant*>(L"Код", new variant(ws)));

        map.insert(std::pair<std::wstring, variant*>(L"Имя", new variant(ws)));

        map.insert(std::pair<std::wstring, variant*>(L"Количество", new variant(ws)));

        map.insert(std::pair<std::wstring, variant*>(L"о_цена", new variant(ws)));

        map.insert(std::pair<std::wstring, variant*>(L"р_цена", new variant(ws)));

        map.insert(std::pair<std::wstring, variant*>(L"СГ", new variant(ws)));
    }

    auto &map = data->prodContain.products.get()[key]->get_map()[code->text().toStdWString()]->get_map();

    map[L"Группа"]->set_wstring(key);

    map[L"EAN"]->set_wstring(EANCode->text().toStdWString());

    map[L"Код"]->set_wstring(code->text().toStdWString());

    map[L"Имя"]->set_wstring(name->text().toStdWString());

    map[L"Количество"]->set_wstring(count->text().toStdWString());

    map[L"о_цена"]->set_wstring(optPrice->text().toStdWString());

    map[L"р_цена"]->set_wstring(rPrice->text().toStdWString());

    map[L"СГ"]->set_wstring(expirationDate->text().toStdWString());


    status->setText("Проведён");

    blockEdit();

    saveDoc();

    uneditedForm();
}

void forms::addProduct_form::blockEdit()
{
    switch (docStates[status->text().toStdWString()])
    {
        case 0:
        {
            remEnter_act->setEnabled(false);
            remDel_act->setEnabled(false);
            enterProd_btn->setEnabled(true);
            deleteProd_btn->setEnabled(true);
            writeProd_btn->setEnabled(true);
            blockFieldsEdit(false);
            break;
        }
        case 1:
        {
            enterProd_btn->setEnabled(false);
            deleteProd_btn->setEnabled(false);
            writeProd_btn->setEnabled(false);
            remDel_act->setEnabled(false);
            remEnter_act->setEnabled(true);
            blockFieldsEdit(true);
            break;
        }
        case 2:
        {
            remDel_act->setEnabled(true);
            writeProd_btn->setEnabled(false);
            enterProd_btn->setEnabled(false);
            remEnter_act->setEnabled(false);
            deleteProd_btn->setEnabled(false);
            blockFieldsEdit(true);
            break;
        }
        default:
        {
            break;
        }
    }
}

void forms::addProduct_form::fillFields(const std::wstring& docNumber_)
{
    if(docNumber_.empty())
    {
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

        *nameBuf = L"Заведение товара МТ1-";
        *nameBuf += data->addProdDoc.doc.docs[L"Номер"]->get_wstring();
        *nameBuf += L" от ";
        *nameBuf += dataContain::w_asctime(pTInfo);
        *nameBuf += L"*";

        mainWgt->setWindowTitle(QString::fromStdWString(*nameBuf));

        docNum->setText(QString::fromStdWString(L"МТ1-" + data->addProdDoc.doc.docs[L"Номер"]->get_wstring()));

        docDate->setText(QString::fromStdWString(dataContain::w_asctime(pTInfo)));

        deleteProd_btn->setEnabled(false);
        remEnter_act->setEnabled(false);
        remDel_act->setEnabled(false);
    }
    else
    {
        auto map = data->addProdDoc.doc.docs.find(docNumber_);

        *time = std::stoll(map->second->get_map()[L"дата_ид"]->get_wstring());
        pTInfo = std::localtime(time);

        *nameBuf = L"Заведение товара МТ1-";
        *nameBuf += map->second->get_map()[L"Номер"]->get_wstring();
        *nameBuf += L" от ";
        *nameBuf += dataContain::w_asctime(pTInfo);
        *nameBuf += L"*";

        mainWgt->setWindowTitle(QString::fromStdWString(*nameBuf));

        docNum->setText(QString::fromStdWString(map->second->get_map()[L"Номер"]->get_wstring()));
        docDate->setText(QString::fromStdWString(map->second->get_map()[L"Дата"]->get_wstring()));
        groupList->findText(QString::fromStdWString(map->second->get_map()[L"Группа"]->get_wstring()));
        EANCode->setText(QString::fromStdWString(map->second->get_map()[L"EAN"]->get_wstring()));
        name->setText(QString::fromStdWString(map->second->get_map()[L"Имя"]->get_wstring()));
        count->setValue(std::stod(map->second->get_map()[L"Количество"]->get_wstring()));
        optPrice->setValue(std::stod(map->second->get_map()[L"о_цена"]->get_wstring()));
        rPrice->setValue(std::stod(map->second->get_map()[L"р_цена"]->get_wstring()));
        status->setText(QString::fromStdWString(map->second->get_map()[L"Статус"]->get_wstring()));
        code->setText(QString::fromStdWString(map->second->get_map()[L"Код"]->get_wstring()));
        creator->setText(QString::fromStdWString(map->second->get_map()[L"Автор"]->get_wstring()));
        expirationDate->setValue(std::stoi(map->second->get_map()[L"СГ"]->get_wstring()));
        comment->setText(QString::fromStdWString(map->second->get_map()[L"Комментарий"]->get_wstring()));

        blockEdit();
    }
}

void forms::create_addProdDoc(QMdiArea* mdiArea, implData* data, QTextBrowser *log, QTableWidget* table,
                                   const std::wstring& docNum, const int& row)
{
    auto subWindow = new QMdiSubWindow(mdiArea);
    auto newAP_f = new forms::addProduct_form(docNum, subWindow);

    subWindow->setWidget(newAP_f->mainWgt);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    newAP_f->setDataPtr(data);
    newAP_f->setLog_ptr(log);
    newAP_f->setupUI();

    mdiArea->addSubWindow(subWindow);

    subWindow->show();

    if(table != nullptr)
    {
        QObject::connect(newAP_f, &forms::addProduct_form::docStatusChange, [row, docNum, data, table]()
        {
            table->item(row, 0)->setIcon(QIcon(QString::fromStdString
            (data->docStatusIcons[data->docsPtr[L"Заведение товара"]->docs.find(docNum)->second->get_map()[L"Статус"]->get_wstring()])));
        });
    }

    QObject::connect(newAP_f->mainWgt, &QWidget::destroyed, [newAP_f, subWindow, mdiArea]()
    {
        delete newAP_f;
        mdiArea->removeSubWindow(subWindow);
    });
}


