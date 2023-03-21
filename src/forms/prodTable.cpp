//
// Created by Maxim on 21.07.2022.
//

#include "forms/prodTable.h"

form::prodTable::prodTable(implData* data_, QTextBrowser* log_, QWidget* parent) : QWidget(parent),
view_model(new QTreeView(this)), obj_tree_model(new models::objectTree_model(view_model)),
table_board(new QWidget(this)), data(data_), mdiArea(nullptr), topMenu_l(new QHBoxLayout(this)),
main_l(new QVBoxLayout(this)), middle_l(new QHBoxLayout(this)), show_hide_model_btn(new QPushButton(this)),
open_prod_card_btn(new QPushButton(this)), log(log_), root_model_obj(new QObject(this)),
topMenu2_l(new QHBoxLayout(this)), printer_btn(new QPushButton(this))
{}

void form::prodTable::showTable()
{
    std::wstring key{obj_tree_model->data(view_model->selectionModel()->currentIndex(), Qt::DisplayRole).toString().toStdWString()};

    if(!table_board->layout()->isEmpty())
    {
        auto widget = table_board->layout()->takeAt(0)->widget();
        widget->setVisible(false);
        table_board->layout()->removeWidget(widget);
    }

    if(data->prodContain.products.search(key) == data->prodContain.products.end())
    {
        return;
    }

    table_board->layout()->addWidget(tables[key]);
    tables[key]->setVisible(true);
}

void form::prodTable::setupUI()
{
    std::function<void(std::unordered_map<std::wstring, variant*>&, QObject*)> extractor =
            [&extractor](std::unordered_map<std::wstring, variant*>& map, QObject* parent)
            {
                for(auto&& it : map)
                {
                    auto* obj = new QObject(parent);
                    obj->setObjectName(QString::fromStdWString(it.first));

                    if(it.second != nullptr && it.second->is_map() && !it.second->map().empty())
                    {
                        extractor(it.second->map(), obj);
                    }
                }
            };

    QStringList cols;
    cols << "objectName";
    obj_tree_model->setColumns(cols);

    if(!data->prodInfo.prodInfo[L"groups"]->map().empty())
    {
        extractor(data->prodInfo.prodInfo[L"groups"]->map(), root_model_obj);
        for(auto&& obj : root_model_obj->children())
        {
            obj_tree_model->addItem(obj, QModelIndex());
        }
    }
    //extractor(data->prodInfo.prodInfo[L"group"]->map(), root_model_obj);
    //obj_tree_model->addItem(root_model_obj->children().first(), QModelIndex());

    table_board->setLayout(new QGridLayout(table_board));

    view_model->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    view_model->setHeaderHidden(true);
    view_model->setModel(obj_tree_model);
    view_model->setFixedWidth(250);

    show_hide_model_btn->setText("<<");
    show_hide_model_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    show_hide_model_btn->setFixedWidth(30);

    open_prod_card_btn->setIcon(QIcon("../../icons/addIcon.png"));
    open_prod_card_btn->setFixedSize(30, 30);
    open_prod_card_btn->setToolTip("Добавить товар");

    printer_btn->setIcon(QIcon("../../icons/printer.png"));
    printer_btn->setFixedSize(30, 30);
    printer_btn->setToolTip("Печать ценников");

    topMenu_l->addWidget(open_prod_card_btn);
    topMenu_l->addWidget(printer_btn);
    topMenu_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    topMenu2_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    middle_l->addWidget(view_model);
    middle_l->addWidget(show_hide_model_btn);
    middle_l->addWidget(table_board);

    main_l->addLayout(topMenu_l);
    main_l->addLayout(middle_l);

    this->setWindowTitle("Товары");
    this->setLayout(main_l);

    for(auto&& prod_group : data->prodContain.products)
    {
        tables[prod_group.first] = new QTableWidget(table_board);
        tables[prod_group.first]->setVisible(false);
        tables[prod_group.first]->setRowCount(static_cast<int>(prod_group.second->map().size()));
        tables[prod_group.first]->setColumnCount(4);
        tables[prod_group.first]->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        tables[prod_group.first]->setHorizontalHeaderLabels({"Код товара", "Наименование", "шт/кг", "Цена"});

        int row{0};
        for(auto&& product : prod_group.second->map())
        {
            tables[prod_group.first]->setItem(row, 0, new QTableWidgetItem
            (QString::fromStdWString(product.second->map()[L"code"]->wstring())));
            tables[prod_group.first]->item(row, 0)->setFlags(tables[prod_group.first]->item
            (row, 0)->flags() ^ Qt::ItemIsEditable);

            tables[prod_group.first]->setItem(row, 1, new QTableWidgetItem(QString::fromStdWString
            (product.second->map()[L"name"]->wstring())));
            tables[prod_group.first]->item(row, 1)->setFlags(tables[prod_group.first]->item
            (row, 1)->flags() ^ Qt::ItemIsEditable);

            tables[prod_group.first]->setItem(row, 2, new QTableWidgetItem(QString::fromStdWString
            (product.second->map()[L"count"]->wstring())));
            tables[prod_group.first]->item(row, 2)->setFlags(tables[prod_group.first]->item
            (row, 2)->flags() ^ Qt::ItemIsEditable);

            tables[prod_group.first]->setItem(row, 3, new QTableWidgetItem(QString::fromStdWString
            (product.second->map()[L"trade_price"]->wstring())));
            tables[prod_group.first]->item(row, 3)->setFlags(tables[prod_group.first]->item
            (row, 3)->flags() ^ Qt::ItemIsEditable);

            prodSearch_code[product.second->map()[L"code"]->wstring()] = {tables[prod_group.first], row};

            row++;
        }
        QObject::connect(tables[prod_group.first], &QTableWidget::cellDoubleClicked, [this, prod_group](int row, int col)
        {
            openProdCard(tables[prod_group.first]->item(row, 0)->text().toStdWString(),
                         obj_tree_model->data(view_model->selectionModel()->currentIndex(), Qt::DisplayRole).toString().toStdWString());
        });
    }

    QObject::connect(view_model->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
                     this, SLOT(showTable()));

    QObject::connect(show_hide_model_btn, &QPushButton::clicked, [this]()
    {
        if(view_model->isHidden())
        {
            view_model->show();
            show_hide_model_btn->setText("<<");
        }
        else
        {
            view_model->hide();
            show_hide_model_btn->setText(">>");
        }
    });

    QObject::connect(open_prod_card_btn, &QPushButton::clicked, [this]()
    {
        form::make_productCard(mdiArea, data, log);
    });

    QObject::connect(printer_btn, &QPushButton::clicked, this, &form::prodTable::printer);
}

void form::prodTable::openProdCard(const std::wstring& code, const std::wstring& group)
{
    auto* pC_f = form::make_productCard(mdiArea, data, log, code, group);
    if(pC_f != nullptr)
    {
        QObject::connect(pC_f, &form::prodCard::productsChanged, this, &form::prodTable::refillingTable);
    }
}

void form::prodTable::setMdiAreaPtr(QMdiArea* mdiArea_)
{
    mdiArea = mdiArea_;
}

void form::prodTable::refillingTable(const std::wstring &key)
{
    tables[key]->setVisible(false);
    tables[key]->clear();
    tables[key]->setRowCount(static_cast<int>(data->prodContain.products.search(key)->second->map().size()));
    tables[key]->setColumnCount(4);
    tables[key]->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tables[key]->setHorizontalHeaderLabels({"Код товара", "Наименование", "шт/кг", "Цена"});

    int row{0};
    for(auto&& product : data->prodContain.products.search(key)->second->map())
    {
        tables[key]->setItem(row, 0, new QTableWidgetItem(QString::fromStdWString
        (product.second->map()[L"code"]->wstring())));
        tables[key]->item(row, 0)->setFlags(tables[key]->item(row, 0)->flags() ^ Qt::ItemIsEditable);

        tables[key]->setItem(row, 1, new QTableWidgetItem(QString::fromStdWString
        (product.second->map()[L"name"]->wstring())));
        tables[key]->item(row, 1)->setFlags(tables[key]->item(row, 1)->flags() ^ Qt::ItemIsEditable);

        tables[key]->setItem(row, 2, new QTableWidgetItem(QString::fromStdWString
        (product.second->map()[L"count"]->wstring())));
        tables[key]->item(row, 2)->setFlags(tables[key]->item(row, 2)->flags() ^ Qt::ItemIsEditable);

        tables[key]->setItem(row, 3, new QTableWidgetItem(QString::fromStdWString
        (product.second->map()[L"trade_price"]->wstring())));
        tables[key]->item(row, 3)->setFlags(tables[key]->item(row, 3)->flags() ^ Qt::ItemIsEditable);

        prodSearch_code[product.second->map()[L"code"]->wstring()] = {tables[key], row};

        row++;
    }
    QObject::connect(tables[key], &QTableWidget::cellDoubleClicked, [this, key](int row, int col)
    {
        openProdCard(tables[key]->item(row, 0)->text().toStdWString(),
                     obj_tree_model->data(view_model->selectionModel()->currentIndex(), Qt::DisplayRole).toString().toStdWString());
    });
}

void form::prodTable::printer()
{
    form::make_thermalLabelPrint(mdiArea, data, log);
}

void form::make_prodTable(QMdiArea* mdiArea, implData* data, QTextBrowser* log)
{
    auto* subWindow = new QMdiSubWindow(mdiArea);
    auto* pT_f = new form::prodTable(data, log, subWindow);

    subWindow->setWidget(pT_f);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(subWindow);

    pT_f->setMdiAreaPtr(mdiArea);
    pT_f->setupUI();

    subWindow->showMaximized();

    QObject::connect(pT_f, &QWidget::destroyed, [subWindow, mdiArea]()
    {
        //mdiArea->closeActiveSubWindow();
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });
}

