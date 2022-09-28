//
// Created by Maxim on 05.09.2022.
//

#include "addProd_form.h"
#include "ioWCharTDocs.hpp"
#include "UI/MainWindow.h"

forms::addProduct_form::addProduct_form() : QObject(nullptr), mainWgt(new QWidget(nullptr)),
                           groupList(new QComboBox(mainWgt)), summaryDate(new QLineEdit(mainWgt)),
                           code(new QLineEdit(mainWgt)), name(new QLineEdit(mainWgt)), count(new QDoubleSpinBox(mainWgt)),
                           price(new QDoubleSpinBox(mainWgt)), expirationDate(new QSpinBox(mainWgt)),
                           vl(new QVBoxLayout(mainWgt)), hl1(new QHBoxLayout), hl2(new QHBoxLayout), hl3(new QHBoxLayout),
                           hl4(new QHBoxLayout), hl5(new QHBoxLayout), hl6(new QHBoxLayout), enterProd_btn(new QPushButton(mainWgt)),
                           data(nullptr), oT_model(new models::objectTree_model(groupList)), deleteProd_btn(new QPushButton(mainWgt)),
                           summaryGL(new QLineEdit(mainWgt)), summaryCode(new QLineEdit(mainWgt)), isHasEdit(true),
                           summaryCount(new QLineEdit(mainWgt)), summaryName(new QLineEdit(mainWgt)),
                           summaryPrice(new QLineEdit(mainWgt)), summaryRPrice(new QLineEdit(mainWgt)),
                           rPrice(new QDoubleSpinBox(mainWgt)), EANCode(new QLineEdit(mainWgt)), log(nullptr),
                           summaryEANCode(new QLineEdit(mainWgt)), hl7(new QHBoxLayout), summaryDocNum(new QLineEdit(mainWgt)),
                           docNum(new QLineEdit(mainWgt)), summaryDocDate(new QLineEdit(mainWgt)),
                           docDate(new QLineEdit(mainWgt)), hl13(new QHBoxLayout),
                           status(new QLineEdit(mainWgt)), creator(new QLineEdit(mainWgt)), hl9(new QHBoxLayout),
                           hl8(new QHBoxLayout), hl10(new QHBoxLayout), hl11(new QHBoxLayout), menu(new QMenu()),
                           summaryEndDate(new QLineEdit(mainWgt)), endDate(new QDateEdit(mainWgt)),
                           hl12(new QHBoxLayout), writeProd_btn(new QPushButton(mainWgt)), actions_btn(new QToolButton(mainWgt)),
                           remEnter_act(new QAction("Отмена проведения", mainWgt)), nameBuf(new std::wstring),
                           remDel_act(new QAction("Снять пометку удаления", mainWgt)){}

forms::addProduct_form::~addProduct_form()
{
    delete pTInfo;
    delete mainWgt;
    delete nameBuf;
}

void forms::addProduct_form::setDataPtr(implData* data_)
{
    data = data_;
}

void forms::addProduct_form::setupUI()
{
    std::function<void(std::unordered_map<std::wstring, std::unique_ptr<variant>>&)> extractor =
            [&extractor, this](std::unordered_map<std::wstring, std::unique_ptr<variant>>& map)
            {
                for(auto&& it : map)
                {
                    obj_list.push_back(new QObject);
                    obj_list.last()->setObjectName(QString::fromStdWString(it.first));

                    oT_model->addItem(obj_list[obj_list.size() - 1], QModelIndex());

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

    if(data->prodInfo.prodInfo[L"РезервныйКод"]->get_vector().empty())
    {
        code->setText(QString::fromStdWString(data->prodInfo.prodInfo[L"Код"]->get_wstring()));
        data->prodInfo.prodInfo[L"Код"]->get_wstring();
        data->prodInfo.prodInfo[L"Код"]->incWStr();
    }
    else
    {
        code->setText(QString::fromStdWString(data->prodInfo.prodInfo[L"РезервныйКод"]->get_vector().back()));
        data->prodInfo.prodInfo[L"РезервныйКод"]->get_vector().pop_back();
    }

    enterProd_btn->setText("Провести");

    writeProd_btn->setText("Записать");

    deleteProd_btn->setText("Удалить");
    deleteProd_btn->setEnabled(false);

    actions_btn->setText("Действия");
    actions_btn->setFixedSize(100, 30);
    actions_btn->setArrowType(Qt::ArrowType::RightArrow);
    actions_btn->setToolButtonStyle(Qt::ToolButtonTextOnly);

    remEnter_act->setEnabled(false);
    remDel_act->setEnabled(false);

    menu->addAction(remEnter_act);
    menu->addAction(remDel_act);

    actions_btn->setMenu(menu);
    actions_btn->setPopupMode(QToolButton::InstantPopup);

    *nameBuf = L"Заведение товара МТ1-";
    *nameBuf += data->addProdDoc.docs[L"Номер"]->get_wstring();
    *nameBuf += L" от ";
    *nameBuf += dataContain::w_asctime(pTInfo);
    *nameBuf += L"*";

    mainWgt->setWindowTitle(QString::fromStdWString(*nameBuf));

    summaryDocNum->setFrame(false);
    summaryDocNum->setReadOnly(true);
    summaryDocNum->setFixedSize(270, 20);
    summaryDocNum->setText("Номер документа:");
    docNum->setReadOnly(true);
    docNum->setFixedSize(150, 20);
    docNum->setText(QString::fromStdWString(L"МТ1-" + data->addProdDoc.docs[L"Номер"]->get_wstring()));

    summaryDocDate->setFrame(false);
    summaryDocDate->setReadOnly(true);
    summaryDocDate->setFixedSize(150, 20);
    summaryDocDate->setText("Дата документа:");
    docDate->setReadOnly(true);
    docDate->setFixedSize(150, 20);
    docDate->setText(QString::fromStdWString(dataContain::w_asctime(pTInfo)));

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
    price->setFixedSize(80, 20);
    price->setMaximum(std::numeric_limits<double>::max());

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

    summaryEndDate->setText("Дата окончания срока годности:");
    summaryEndDate->setFixedSize(250, 20);
    summaryEndDate->setReadOnly(true);
    summaryEndDate->setFrame(false);
    endDate->setFixedSize(100, 20);
    endDate->setCalendarPopup(true);
    endDate->setDate(QDate(pTInfo->tm_year + 1900, pTInfo->tm_mon + 1, pTInfo->tm_mday));

    status->setFrame(false);
    status->setReadOnly(true);
    status->setFixedSize(60, 20);
    status->setText("Новый");

    creator->setFrame(false);
    creator->setReadOnly(true);
    creator->setFixedSize(270, 20);

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
    hl5->addWidget(price);
    hl5->addWidget(summaryRPrice);
    hl5->addWidget(rPrice);

    hl6->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl6->addWidget(summaryDate);
    hl6->addWidget(expirationDate);
    hl6->addWidget(summaryEndDate);
    hl6->addWidget(endDate);

    hl12->addWidget(deleteProd_btn);
    hl12->addWidget(writeProd_btn);
    hl12->addWidget(enterProd_btn);
    hl12->addWidget(actions_btn);

    vl->addLayout(hl8);
    vl->addLayout(hl1);
    vl->addLayout(hl2);
    vl->addLayout(hl7);
    vl->addLayout(hl3);
    vl->addLayout(hl4);
    vl->addLayout(hl5);
    vl->addLayout(hl6);
    vl->addLayout(hl12);

    QObject::connect(EANCode, SIGNAL(editingFinished()), this, SLOT(editedForm()));
    QObject::connect(groupList, SIGNAL(currentTextChanged(QString)), this, SLOT(editedForm()));
    QObject::connect(name, SIGNAL(editingFinished()), this, SLOT(editedForm()));
    QObject::connect(count, SIGNAL(editingFinished()), this, SLOT(editedForm()));
    QObject::connect(rPrice, SIGNAL(editingFinished()), this, SLOT(editedForm()));
    QObject::connect(price, SIGNAL(editingFinished()), this, SLOT(editedForm()));
    QObject::connect(expirationDate, SIGNAL(editingFinished()), this, SLOT(editedForm()));
    QObject::connect(endDate, SIGNAL(editingFinished()), this, SLOT(editedForm()));

    QObject::connect(expirationDate, &QSpinBox::editingFinished, [this]()
    {
        int y{pTInfo->tm_year + 1900};
        int m{pTInfo->tm_mon + 1};
        int d{1};

        m += expirationDate->value();

        while(m > 12)
        {
            ++y;
            m -= 12;
        }

        endDate->setDate(QDate(y, m, d));
    });

    connect(enterProd_btn, SIGNAL(clicked()), this, SLOT(addProduct()));

    QObject::connect(writeProd_btn, &QPushButton::clicked, [this]()
    {
        if(status->text() != "Проведён" && isHasEdit)
        {
            if(status->text() == "Новый")
            {
                data->docInfo.docInfo[L"Номер"]->incWStr();
            }

            saveDoc();

            deleteProd_btn->setEnabled(true);

            uneditedForm();

            status->setText("Записан");

            MainWindow::logger("Документ " + QString::fromStdWString(*nameBuf) + " сохранен.", log);
        }
        else if(status->text() == "Проведён")
        {
            MainWindow::logger("Документ проведён! Распроведите для редактирования.", log);
        }
        else if(status->text() == "Удалён")
        {
            MainWindow::logger("Документ удалён! Снимите пометку на удаление для редактирования.", log);
        }
        else
        {
            MainWindow::logger("Документ не модифицирован, нет смысла в повторном сохранении.", log);
        }
    });

    QObject::connect(deleteProd_btn, &QPushButton::clicked, [this]()
    {
        if(status->text() == "Удалён")
        {
            data->addProdDoc.docs[L"Документы"]->get_map()[std::to_wstring(pTInfo->tm_year + 1900)]->get_map().erase
            (docNum->text().toStdWString());

            status->setText("Удалён");
            blockEdit(true);
            remEnter_act->setEnabled(true);

            uneditedForm();
        }
        else
        {
            MainWindow::logger("Документ удалён! Для редактирования снемите пометку удаления.",log);
        }
    });

    QObject::connect(remEnter_act, &QAction::triggered, this, [this](){
        blockEdit(false);

        data->prodContain.products.get()[groupList->currentText().toStdWString()]->get_map().erase(code->text().toStdWString());

        status->setText("Записан");
        remEnter_act->setEnabled(false);
        enterProd_btn->setEnabled(true);
    });

    QObject::connect(remDel_act, &QAction::triggered, this, [this](){
        blockEdit(false);

        addProduct();
        saveDoc();

        status->setText("Записан");
        remDel_act->setEnabled(false);
    });
}

void forms::addProduct_form::saveDoc()
{
    tag::mapWStrVar msv;
    tag::wString ws;

    if(data->addProdDoc.docs.find(std::to_wstring(pTInfo->tm_year + 1900)) == data->addProdDoc.docs.end())
    {
        data->addProdDoc.docs[L"Документы"]->get_map().
        insert(std::pair<std::wstring, std::unique_ptr<variant>>(std::to_wstring(pTInfo->tm_year + 1900), nullptr));

        data->addProdDoc.docs[L"Документы"]->get_map()[std::to_wstring(pTInfo->tm_year + 1900)] = std::make_unique<variant>(msv);
    }

    if(data->addProdDoc.docs.find(docNum->text().toStdWString()) == data->addProdDoc.docs.get().end())
    {
        data->addProdDoc.docs[L"Документы"]->get_map()[std::to_wstring(pTInfo->tm_year + 1900)]->
        get_map().insert(std::pair<std::wstring, std::unique_ptr<variant>>
        (L"МТ1-" + docNum->text().toStdWString().substr(4, 6), nullptr));

        data->addProdDoc.docs[L"Документы"]->get_map()[std::to_wstring(pTInfo->tm_year + 1900)]->
        get_map()[docNum->text().toStdWString()] = std::make_unique<variant>(msv);

        auto& map = data->addProdDoc.docs[L"Документы"]->get_map()[std::to_wstring(pTInfo->tm_year + 1900)]->
                get_map()[docNum->text().toStdWString()]->get_map();

        map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Номер", nullptr));
        map[L"Номер"] = std::make_unique<variant>(ws);

        map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Дата", nullptr));
        map[L"Дата"] = std::make_unique<variant>(ws);

        map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Группа", nullptr));
        map[L"Группа"] = std::make_unique<variant>(ws);

        map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"EAN", nullptr));
        map[L"EAN"] = std::make_unique<variant>(ws);

        map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Количество", nullptr));
        map[L"Количество"] = std::make_unique<variant>(ws);

        map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"р_цена", nullptr));
        map[L"р_цена"] = std::make_unique<variant>(ws);

        map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"о_цена", nullptr));
        map[L"о_цена"] = std::make_unique<variant>(ws);

        map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"СГ", nullptr));
        map[L"СГ"] = std::make_unique<variant>(ws);

        map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Окончание СГ", nullptr));
        map[L"Окончание СГ"] = std::make_unique<variant>(ws);

        map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Статус", nullptr));
        map[L"Статус"] = std::make_unique<variant>(ws);

        map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Автор", nullptr));
        map[L"Автор"] = std::make_unique<variant>(ws);
    }

    auto& map = data->addProdDoc.docs[L"Документы"]->get_map()[std::to_wstring(pTInfo->tm_year + 1900)]->
            get_map()[L"МТ1-" + docNum->text().toStdWString().substr(4, 6)]->get_map();

    map[L"Номер"]->set_wstring(docNum->text().toStdWString());

    map[L"Дата"]->set_wstring(docDate->text().toStdWString());

    map[L"Группа"]->set_wstring(groupList->currentText().toStdWString());

    map[L"EAN"]->set_wstring(EANCode->text().toStdWString());

    map[L"Количество"]->set_wstring(count->text().toStdWString());

    map[L"р_цена"]->set_wstring(rPrice->text().toStdWString());

    map[L"о_цена"]->set_wstring(price->text().toStdWString());

    map[L"СГ"]->set_wstring(expirationDate->text().toStdWString());

    map[L"Окончание СГ"]->set_wstring(endDate->text().toStdWString());

    map[L"Статус"]->set_wstring(status->text().toStdWString());

    map[L"Автор"]->set_wstring(status->text().toStdWString());
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
        mainWgt->update();
    }
}

void forms::addProduct_form::blockEdit(const bool& flag)
{
    groupList->setEditable(flag);
    EANCode->setReadOnly(flag);
    name->setReadOnly(flag);
    count->setReadOnly(flag);
    rPrice->setReadOnly(flag);
    price->setReadOnly(flag);
    expirationDate->setReadOnly(flag);
    endDate->setReadOnly(flag);
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
        data->docInfo.docInfo[L"Номер"]->incWStr();
    }

    if(status->text() == "Удалён")
    {
        MainWindow::logger("Документ удалён! Для редактирования снимите пометку на удаление.", log);
    }
    else
    {
        std::wstring key = groupList->currentText().toStdWString();

        tag::mapWStrVar msv;
        tag::wString ws;

        if (data->prodContain.products.find(key) == data->prodContain.products.end()) {
            data->prodContain.products.get()[key] = std::make_unique<variant>(msv);
        }

        if (data->prodContain.products.find(code->text().toStdWString()) == data->prodContain.products.end()) {
            data->prodContain.products.get()[key]->get_map()[code->text().toStdWString()] = std::make_unique<variant>(
                    msv);
            auto &map = data->prodContain.products.get()[key]->get_map()[code->text().toStdWString()]->get_map();

            map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Группа", nullptr));
            map[L"Группа"] = std::make_unique<variant>(ws);

            map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"EAN", nullptr));
            map[L"EAN"] = std::make_unique<variant>(ws);

            map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Код", nullptr));
            map[L"Код"] = std::make_unique<variant>(ws);

            map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Наименование", nullptr));
            map[L"Наименование"] = std::make_unique<variant>(ws);

            map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Количество", nullptr));
            map[L"Количество"] = std::make_unique<variant>(ws);

            map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"о_цена", nullptr));
            map[L"о_цена"] = std::make_unique<variant>(ws);

            map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"р_цена", nullptr));
            map[L"р_цена"] = std::make_unique<variant>(ws);

            map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"СГ", nullptr));
            map[L"СГ"] = std::make_unique<variant>(ws);

            map.insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Окончание СГ", nullptr));
            map[L"Окончание СГ"] = std::make_unique<variant>(ws);
        }

        auto &map = data->prodContain.products.get()[key]->get_map()[code->text().toStdWString()]->get_map();

        map[L"Группа"]->set_wstring(key);

        map[L"EAN"]->set_wstring(EANCode->text().toStdWString());

        map[L"Код"]->set_wstring(code->text().toStdWString());

        map[L"Наименование"]->set_wstring(name->text().toStdWString());

        map[L"Количество"]->set_wstring(count->text().toStdWString());

        map[L"о_цена"]->set_wstring(rPrice->text().toStdWString());

        map[L"р_цена"]->set_wstring(price->text().toStdWString());

        map[L"СГ"]->set_wstring(expirationDate->text().toStdWString());

        map[L"Окончание СГ"]->set_wstring(endDate->text().toStdWString());

        status->setText("Проведён");

        enterProd_btn->setEnabled(false);
        deleteProd_btn->setEnabled(true);
        remEnter_act->setEnabled(true);

        saveDoc();
        blockEdit(true);

        uneditedForm();
    }
}


