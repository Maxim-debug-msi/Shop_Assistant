//
// Created by Maxim on 21.07.2022.
//

#include "forms/prodTable.h"

form::prodTable::prodTable(implData* data_, QTextBrowser* log_, QWidget* parent) : QWidget(parent),
viewModel(new QTreeView(this)), oT_model(new models::objectTree_model(viewModel)),
tableBoard(new QWidget(this)), data(data_), mainWindow(nullptr), topMenu_l(new QHBoxLayout(this)),
main_l(new QVBoxLayout(this)), middle_l(new QHBoxLayout(this)), showHideModel_btn(new QPushButton(this)),
openProdCard_btn(new QPushButton(this)), log(log_), rootModelObj(new QObject(this)),
topMenu2_l(new QHBoxLayout(this)), printer_btn(new QPushButton(this))
{
    //mainWgt->setAttribute((Qt::WA_DeleteOnClose));
}

void form::prodTable::showTable()
{
    std::wstring key{oT_model->data(viewModel->selectionModel()->currentIndex(), Qt::DisplayRole).toString().toStdWString()};

    if(!tableBoard->layout()->isEmpty())
    {
        auto widget = tableBoard->layout()->takeAt(0)->widget();
        widget->setVisible(false);
        tableBoard->layout()->removeWidget(widget);
    }

    if(data->prodContain.products.search(key) == data->prodContain.products.end())
    {
        return;
    }

    tableBoard->layout()->addWidget(tables[key]);
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

                    if(it.second != nullptr && it.second->is_map() && !it.second->is_empty_map())
                    {
                        extractor(it.second->get_map(), obj);
                    }
                }
            };

    extractor(data->prodInfo.prodInfo[L"Группы"]->get_map(), rootModelObj);

    QStringList cols;
    cols << "objectName";
    oT_model->setColumns(cols);
    oT_model->addItem(rootModelObj->children().first(), QModelIndex());

    tableBoard->setLayout(new QGridLayout(tableBoard));

    viewModel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    viewModel->setHeaderHidden(true);
    viewModel->setModel(oT_model);
    viewModel->setFixedWidth(250);

    showHideModel_btn->setText("<<");
    showHideModel_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    showHideModel_btn->setFixedWidth(30);

    openProdCard_btn->setIcon(QIcon("../../icons/addIcon.png"));
    openProdCard_btn->setFixedSize(30, 30);
    openProdCard_btn->setToolTip("Добавить товар");

    printer_btn->setIcon(QIcon("../../icons/printer.png"));
    printer_btn->setFixedSize(30, 30);
    printer_btn->setToolTip("Печать ценников");

    topMenu_l->addWidget(openProdCard_btn);
    topMenu_l->addWidget(printer_btn);
    topMenu_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    topMenu2_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    //topMenu2_l->addWidget(printer_btn);

    middle_l->addWidget(viewModel);
    middle_l->addWidget(showHideModel_btn);
    middle_l->addWidget(tableBoard);

    main_l->addLayout(topMenu_l);
    main_l->addLayout(middle_l);

    this->setWindowTitle("Товары");
    this->setLayout(main_l);

    QObject::connect(viewModel->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
                     this, SLOT(showTable()));

    QObject::connect(showHideModel_btn, &QPushButton::clicked, [this]()
    {
        if(viewModel->isHidden())
        {
            viewModel->show();
            showHideModel_btn->setText("<<");
        }
        else
        {
            viewModel->hide();
            showHideModel_btn->setText(">>");
        }
    });

    QObject::connect(openProdCard_btn, &QPushButton::clicked, [this]()
    {
        form::make_productCard(mainWindow, data, log);
    });

    for(auto&& prod_group : data->prodContain.products)//build tables
    {
        tables[prod_group.first] = new QTableWidget(tableBoard);
        tables[prod_group.first]->setVisible(false);
        tables[prod_group.first]->setRowCount(static_cast<int>(prod_group.second->map_size()));
        tables[prod_group.first]->setColumnCount(4);
        tables[prod_group.first]->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        tables[prod_group.first]->setHorizontalHeaderLabels({"Код товара", "Наименование", "шт/кг", "Цена"});

        int row{0};
        for(auto&& product : prod_group.second->get_map())
        {
            tables[prod_group.first]->setItem(row, 0, new QTableWidgetItem
            (QString::fromStdWString(product.second->get_map()[L"код"]->get_wstring())));
            tables[prod_group.first]->item(row, 0)->setFlags(tables[prod_group.first]->item
            (row, 0)->flags() ^ Qt::ItemIsEditable);

            tables[prod_group.first]->setItem(row, 1, new QTableWidgetItem(QString::fromStdWString
            (product.second->get_map()[L"имя"]->get_wstring())));
            tables[prod_group.first]->item(row, 1)->setFlags(tables[prod_group.first]->item
            (row, 1)->flags() ^ Qt::ItemIsEditable);

            tables[prod_group.first]->setItem(row, 2, new QTableWidgetItem(QString::fromStdWString
            (product.second->get_map()[L"количество"]->get_wstring())));
            tables[prod_group.first]->item(row, 2)->setFlags(tables[prod_group.first]->item
            (row, 2)->flags() ^ Qt::ItemIsEditable);

            tables[prod_group.first]->setItem(row, 3, new QTableWidgetItem(QString::fromStdWString
            (product.second->get_map()[L"р_цена"]->get_wstring())));
            tables[prod_group.first]->item(row, 3)->setFlags(tables[prod_group.first]->item
            (row, 3)->flags() ^ Qt::ItemIsEditable);

            prodSearch_code[product.second->get_map()[L"код"]->get_wstring()] = {tables[prod_group.first], row};

            row++;
        }
        QObject::connect(tables[prod_group.first], &QTableWidget::cellDoubleClicked, [this, prod_group](int row, int col)
        {
            openProdCard(tables[prod_group.first]->item(row, 0)->text().toStdWString(),
                         oT_model->data(viewModel->selectionModel()->currentIndex(), Qt::DisplayRole).toString().toStdWString());
        });
    }
    QObject::connect(printer_btn, &QPushButton::clicked, this, &form::prodTable::printer);
}

void form::prodTable::openProdCard(const std::wstring& code, const std::wstring& group)
{
    auto* pC_f = form::make_productCard(mainWindow, data, log, code, group);
    if(pC_f != nullptr)
    {
        QObject::connect(pC_f, &form::prodCard::productsChanged, this, &form::prodTable::refillingTable);
    }
}

void form::prodTable::setMainWindow_ptr(QMdiArea* mdiArea)
{
    mainWindow = mdiArea;
}

void form::prodTable::refillingTable(const std::wstring &key)
{
    tables[key]->setVisible(false);
    tables[key]->clear();
    tables[key]->setRowCount(static_cast<int>(data->prodContain.products.search(key)->second->map_size()));
    tables[key]->setColumnCount(4);
    tables[key]->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tables[key]->setHorizontalHeaderLabels({"Код товара", "Наименование", "шт/кг", "Цена"});

    int row{0};
    for(auto&& product : data->prodContain.products.search(key)->second->get_map())
    {
        tables[key]->setItem(row, 0, new QTableWidgetItem(QString::fromStdWString
        (product.second->get_map()[L"код"]->get_wstring())));
        tables[key]->item(row, 0)->setFlags(tables[key]->item(row, 0)->flags() ^ Qt::ItemIsEditable);

        tables[key]->setItem(row, 1, new QTableWidgetItem(QString::fromStdWString
        (product.second->get_map()[L"имя"]->get_wstring())));
        tables[key]->item(row, 1)->setFlags(tables[key]->item(row, 1)->flags() ^ Qt::ItemIsEditable);

        tables[key]->setItem(row, 2, new QTableWidgetItem(QString::fromStdWString
        (product.second->get_map()[L"количество"]->get_wstring())));
        tables[key]->item(row, 2)->setFlags(tables[key]->item(row, 2)->flags() ^ Qt::ItemIsEditable);

        tables[key]->setItem(row, 3, new QTableWidgetItem(QString::fromStdWString
        (product.second->get_map()[L"р_цена"]->get_wstring())));
        tables[key]->item(row, 3)->setFlags(tables[key]->item(row, 3)->flags() ^ Qt::ItemIsEditable);

        prodSearch_code[product.second->get_map()[L"код"]->get_wstring()] = {tables[key], row};

        row++;
    }
    QObject::connect(tables[key], &QTableWidget::cellDoubleClicked, [this, key](int row, int col)
    {
        openProdCard(tables[key]->item(row, 0)->text().toStdWString(),
                     oT_model->data(viewModel->selectionModel()->currentIndex(), Qt::DisplayRole).toString().toStdWString());
    });
}

void form::prodTable::printer()
{
    /*
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);

    printDialog.setWindowTitle("Печать ценников");

    if(printDialog.exec() == QPrintDialog::Accepted)
    {
        QTextBrowser text(this);
        text.setMarkdown("asaasa");
        text.print(&printer);
    }
     */
    form::make_termoPrint(mainWindow, data, log, this);
}

void form::make_prodTable(QMdiArea* mdiArea, implData* data, QTextBrowser* log)
{
    auto* subWindow = new QMdiSubWindow(mdiArea);
    auto* pT_f = new form::prodTable(data, log, subWindow);

    subWindow->setWidget(pT_f);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    pT_f->setMainWindow_ptr(mdiArea);
    pT_f->setupUI();

    mdiArea->addSubWindow(subWindow);

    subWindow->showMaximized();

    QObject::connect(pT_f, &QWidget::destroyed, [subWindow, mdiArea]()
    {
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });


}

