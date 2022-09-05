//
// Created by Maxim on 21.07.2022.
//

#include <iostream>
#include <QLayout>
#include <QTreeView>
#include <QPushButton>
#include <QHeaderView>
#include "product_form.h"

forms::prodTable_form::prodTable_form() : QObject(nullptr), mainWgt(new QWidget), viewModel(new QTreeView(mainWgt)),
oT_model(new models::objectTree_model(viewModel)), model(nullptr), table(new QWidget(mainWgt)), data(nullptr){}

forms::prodTable_form::~prodTable_form()
{
    delete mainWgt;
}

void forms::prodTable_form::drowTable()
{
    std::wstring key{oT_model->data(viewModel->selectionModel()->currentIndex(), Qt::DisplayRole).toString().toStdWString()};

    if(!table->layout()->isEmpty())
    {
        auto widget = table->layout()->takeAt(0)->widget();
        widget->setVisible(false);
        table->layout()->removeWidget(widget);
    }

    if(data->products.find(key) == data->products.end())
    {
        return;
    }

    table->layout()->addWidget(tables[key]);
    tables[key]->setVisible(true);
}

void forms::prodTable_form::setDataPtr(dataContain::prod_data* data_)
{
    data = data_;
}

void forms::prodTable_form::build()
{
    std::function<void(std::unordered_map<std::wstring, std::unique_ptr<variant>>&, QObject*)> extractor =
            [&extractor, this](std::unordered_map<std::wstring, std::unique_ptr<variant>>& map, QObject* parent)
            {
                for(auto&& it : map)
                {
                    obj_list.push_back(new QObject(parent));
                    obj_list[obj_list.size() - 1]->setObjectName(QString::fromStdWString(it.first));

                    if(it.second != nullptr && it.second->is_map() && !it.second->is_empty_map())
                    {
                        extractor(it.second->get_map(), obj_list[obj_list.size() - 1]);
                    }
                }
            };

    extractor(model->groupMap_.get(), nullptr);

    QStringList cols;
    cols << "objectName";
    oT_model->setColumns(cols);

    oT_model->addItem(obj_list[0], QModelIndex());

    table->setLayout(new QGridLayout(table));

    viewModel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    viewModel->setHeaderHidden(true);
    viewModel->setModel(oT_model);
    viewModel->setFixedWidth(250);

    auto* btn = new QPushButton(mainWgt);
    btn->setText("<<");
    btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    btn->setFixedWidth(30);

    auto* l = new QHBoxLayout(mainWgt);
    l->addWidget(viewModel);
    l->addWidget(btn);
    l->addWidget(table);

    mainWgt->setWindowTitle("Товары");
    mainWgt->setLayout(l);

    QObject::connect(viewModel->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
                     this, SLOT(drowTable()));

    QObject::connect(btn, &QPushButton::clicked, [this, btn]()
    {
        if(viewModel->isHidden())
        {
            viewModel->show();
            btn->setText("<<");
        }
        else
        {
            viewModel->hide();
            btn->setText(">>");
        }
    });

    for(auto&& prod_group : data->products)//build tables
    {
        tables[prod_group.first] = new QTableWidget;

        tables[prod_group.first]->setRowCount(static_cast<int>(prod_group.second->map_size()));
        tables[prod_group.first]->setColumnCount(4);
        tables[prod_group.first]->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        tables[prod_group.first]->setHorizontalHeaderLabels({"Код товара", "Наименование", "шт/кг", "Цена"});

        int row{0};
        for(auto&& product : prod_group.second->get_map())
        {
            tables[prod_group.first]->setItem(row, 0, new QTableWidgetItem(QString::fromStdWString(product.second->get_map()[L"Код"]->get_wstring())));
            tables[prod_group.first]->item(row, 0)->setFlags(tables[prod_group.first]->item(row, 0)->flags() ^ Qt::ItemIsEditable);

            tables[prod_group.first]->setItem(row, 1, new QTableWidgetItem(QString::fromStdWString(product.second->get_map()[L"Наименование"]->get_wstring())));
            tables[prod_group.first]->item(row, 1)->setFlags(tables[prod_group.first]->item(row, 1)->flags() ^ Qt::ItemIsEditable);

            if(product.second->get_map()[L"Количество"]->get_wstring() == L"0") product.second->get_map()[L"Количество"]->set_wstring(L"");

            tables[prod_group.first]->setItem(row, 2, new QTableWidgetItem(QString::fromStdWString(product.second->get_map()[L"Количество"]->get_wstring())));
            tables[prod_group.first]->item(row, 2)->setFlags(tables[prod_group.first]->item(row, 2)->flags() ^ Qt::ItemIsEditable);

            tables[prod_group.first]->setItem(row, 3, new QTableWidgetItem(QString::fromStdWString(product.second->get_map()[L"Цена"]->get_wstring())));
            tables[prod_group.first]->item(row, 3)->setFlags(tables[prod_group.first]->item(row, 3)->flags() ^ Qt::ItemIsEditable);

            prodSearch_code[product.second->get_map()[L"Код"]->get_wstring()] = {tables[prod_group.first], row};

            row++;
        }
    }
}

void forms::prodTable_form::setModelPtr(dataContain::prodTree_model* model_)
{
    model = model_;
}
