//
// Created by Maxim on 02.03.2023.
//
#include "searchProduct_dlg.h"

form_dialog::searchProduct_dlg::searchProduct_dlg(QWidget *parent) : QDialog(parent), accept_btn(new QPushButton(this)),
decline_btn(new QPushButton(this)), main_l(new QHBoxLayout(this)), text(new QLineEdit(this))
{
    this->setWindowTitle("Поиск товара");

    accept_btn->setText("Принять");
    accept_btn->setDefault(true);

    decline_btn->setText("Отмена");

    main_l->addWidget(text);
    main_l->addWidget(accept_btn);
    main_l->addWidget(decline_btn);

    this->setLayout(main_l);

    connect(accept_btn, &QPushButton::clicked, this, &QDialog::accept);
    connect(decline_btn, &QPushButton::clicked, this, &QDialog::reject);
}

form_dialog::searchProduct_dlg::~searchProduct_dlg()
{
    this->disconnect();
}

QString form_dialog::searchProduct_dlg::getInput() const
{
    return text->text();
}
