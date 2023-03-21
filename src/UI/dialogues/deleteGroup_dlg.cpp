//
// Created by Maxim on 21.11.2022.
//

#include "deleteGroup_dlg.h"

form_dialog::deleteGroup_dlg::deleteGroup_dlg(implData* data, QString* groupName, QWidget *parent) : QDialog(parent),
groupForDel_(groupName),
d_groupList(new QLineEdit(this)), groupList(new QComboBox(this)), accept_btn(new QPushButton(this)),
delete_btn(new QPushButton(this)), decline_btn(new QPushButton(this)), main_l(new QVBoxLayout(this)),
groupList_l(new QHBoxLayout(this)), buttons_l(new QHBoxLayout(this)),
data_(data)
{
    this->setWindowTitle("Перемещение/удаление товара");

    d_groupList->setReadOnly(true);
    d_groupList->setFrame(false);
    d_groupList->setFixedSize(270, 20);
    d_groupList->setText("Выберите группу для перемещения: ");

    groupList->addItems(getProductGroups(data_->jsonData.prodInfo[L"groups"]->map()));

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

    *groupForDel_ = groupList->currentText();

    this->setLayout(main_l);
    QObject::connect(groupList, &QComboBox::currentTextChanged, this, &form_dialog::deleteGroup_dlg::currentGroupName);
    QObject::connect(accept_btn, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(delete_btn, &QPushButton::clicked, this, [this](){ QDialog::done(3);});
    QObject::connect(decline_btn, &QPushButton::clicked, this, &QDialog::reject);
}

form_dialog::deleteGroup_dlg::~deleteGroup_dlg() = default;

QStringList form_dialog::deleteGroup_dlg::getProductGroups(std::unordered_map<std::wstring, variant *> &map)
{
    QStringList result;
    for(auto&& it : map)
    {
        if(it.first != *groupForDel_)
        {
            result << QString::fromStdWString(it.first);

            if (it.second != nullptr && it.second->is_map() && !it.second->map().empty()) {
                getProductGroups(it.second->map());
            }
        }
    }
    return result;
}

void form_dialog::deleteGroup_dlg::currentGroupName()
{
    *groupForDel_ = groupList->currentText();
}

