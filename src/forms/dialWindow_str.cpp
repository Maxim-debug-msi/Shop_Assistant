//
// Created by Maxim on 17.11.2022.
//
#include "forms/dialWindow_str.h"

form::dialWindow_str::dialWindow_str(QWidget* parent) : QDialog(parent), text(new QLineEdit(this)),
accept_btn(new QPushButton(this)), decline_btn(new QPushButton(this)), main_l(new QHBoxLayout(this))
{
    this->setWindowTitle("Добавление новой группы");

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

form::dialWindow_str::~dialWindow_str() {}

QString form::dialWindow_str::getInput() const
{
    return text->text();
}

