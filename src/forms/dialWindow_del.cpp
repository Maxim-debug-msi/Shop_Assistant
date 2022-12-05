//
// Created by Maxim on 21.11.2022.
//

#include "dialWindow_del.h"

form::dialWindow_del::dialWindow_del(implData* data_, QString* groupName, QWidget *parent) : QDialog(parent),
rootModelObj(new QObject(this)), groupForDel(groupName),
d_groupList(new QLineEdit(this)), groupList(new QComboBox(this)), accept_btn(new QPushButton(this)),
delete_btn(new QPushButton(this)), decline_btn(new QPushButton(this)), main_l(new QVBoxLayout(this)),
groupList_l(new QHBoxLayout(this)), buttons_l(new QHBoxLayout(this)),
oT_model(new models::objectTree_model(groupList)), data(data_)
{
    this->setWindowTitle("Перемещение/удаление товара");

    d_groupList->setReadOnly(true);
    d_groupList->setFrame(false);
    d_groupList->setFixedSize(270, 20);
    d_groupList->setText("Выберите группу для перемещения: ");

    extractor(data->prodInfo.prodInfo[L"Группы"]->get_map());

    QStringList cols;
    cols << "objectName";
    oT_model->setColumns(cols);

    groupList->setModel(oT_model);

    accept_btn->setDefault(true);
    accept_btn->setText("Переместить");

    delete_btn->setText("Удалить");

    decline_btn->setText("Отмена");

    groupList_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    groupList_l->addWidget(d_groupList);
    groupList_l->addWidget(groupList);

    buttons_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    buttons_l->addWidget(accept_btn);
    buttons_l->addWidget(delete_btn);
    buttons_l->addWidget(decline_btn);

    main_l->addLayout(groupList_l);
    main_l->addLayout(buttons_l);

    *groupForDel = groupList->currentText();

    this->setLayout(main_l);
    QObject::connect(groupList, &QComboBox::currentTextChanged, this, &form::dialWindow_del::currentGroupName);
    QObject::connect(accept_btn, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(delete_btn, &QPushButton::clicked, this, &form::dialWindow_del::deleting);
    QObject::connect(decline_btn, &QPushButton::clicked, this, &QDialog::reject);
}

form::dialWindow_del::~dialWindow_del()
{
    delete oT_model;
}

void form::dialWindow_del::extractor(std::unordered_map<std::wstring, variant *> &map)
{
    for(auto&& it : map)
    {
        if(it.first != *groupForDel)
        {
            auto obj = new QObject(rootModelObj);
            obj->setObjectName(QString::fromStdWString(it.first));

            oT_model->addItem(obj, QModelIndex());

            if (it.second != nullptr && it.second->is_map() && !it.second->is_empty_map()) {
                extractor(it.second->get_map());
            }
        }
    }
}

void form::dialWindow_del::currentGroupName()
{
    *groupForDel = groupList->currentText();
}

