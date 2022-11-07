//
// Created by Maxim on 21.07.2022.
//

#include "product_form.h"

forms::prodTable_form::prodTable_form(implData* data_, QTextBrowser* log_, QWidget* parent) : QObject(nullptr),
mainWgt(new QWidget(parent)), viewModel(new QTreeView(mainWgt)), oT_model(new models::objectTree_model(viewModel)),
tableBoard(new QWidget(mainWgt)), data(data_), mainWindow(nullptr), topMenu_l(new QHBoxLayout(mainWgt)),
main_l(new QVBoxLayout(mainWgt)), middle_l(new QHBoxLayout(mainWgt)), table(new QTableWidget(tableBoard)),
openProdCard_btn(new QPushButton(mainWgt)), log(log_), rootModelObj(new QObject(mainWgt))
{
    //mainWgt->setAttribute((Qt::WA_DeleteOnClose));
}

forms::prodTable_form::~prodTable_form()
{
    delete rootModelObj;
    delete oT_model;
}

void forms::prodTable_form::showTable()
{
    if(table != nullptr)
    {
        table->setVisible(false);
    }
    table = tables[oT_model->data(viewModel->selectionModel()->currentIndex(),
                             Qt::DisplayRole).toString().toStdWString()];
    table->setVisible(true);
}

void forms::prodTable_form::setupUI()
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
    tableBoard->layout()->addWidget(table);

    viewModel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    viewModel->setHeaderHidden(true);
    viewModel->setModel(oT_model);
    viewModel->setFixedWidth(250);

    auto* btn = new QPushButton(mainWgt);
    btn->setText("<<");
    btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    btn->setFixedWidth(30);

    openProdCard_btn->setIcon(QIcon("../../icons/addIcon.png"));
    openProdCard_btn->setFixedSize(20, 20);
    openProdCard_btn->setToolTip("Добавить товар");

    topMenu_l->addWidget(openProdCard_btn);
    topMenu_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    middle_l->addWidget(viewModel);
    middle_l->addWidget(btn);
    middle_l->addWidget(tableBoard);

    main_l->addLayout(topMenu_l);
    main_l->addLayout(middle_l);

    mainWgt->setWindowTitle("Товары");
    mainWgt->setLayout(main_l);

    QObject::connect(viewModel->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
                     this, SLOT(showTable()));

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

    QObject::connect(openProdCard_btn, &QPushButton::clicked, [this]()
    {
        forms::make_productCard(mainWindow, data, log);
    });

    for(auto&& prod_group : data->prodContain.products.get())//build tables
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
            tables[prod_group.first]->setItem(row, 0, new QTableWidgetItem(QString::fromStdWString(product.second->get_map()[L"код"]->get_wstring())));
            tables[prod_group.first]->item(row, 0)->setFlags(tables[prod_group.first]->item(row, 0)->flags() ^ Qt::ItemIsEditable);

            tables[prod_group.first]->setItem(row, 1, new QTableWidgetItem(QString::fromStdWString(product.second->get_map()[L"имя"]->get_wstring())));
            tables[prod_group.first]->item(row, 1)->setFlags(tables[prod_group.first]->item(row, 1)->flags() ^ Qt::ItemIsEditable);

            if(product.second->get_map()[L"количество"]->get_wstring() == L"0") product.second->get_map()[L"количество"]->set_wstring(L"");

            tables[prod_group.first]->setItem(row, 2, new QTableWidgetItem(QString::fromStdWString(product.second->get_map()[L"количество"]->get_wstring())));
            tables[prod_group.first]->item(row, 2)->setFlags(tables[prod_group.first]->item(row, 2)->flags() ^ Qt::ItemIsEditable);

            tables[prod_group.first]->setItem(row, 3, new QTableWidgetItem(QString::fromStdWString(product.second->get_map()[L"р_цена"]->get_wstring())));
            tables[prod_group.first]->item(row, 3)->setFlags(tables[prod_group.first]->item(row, 3)->flags() ^ Qt::ItemIsEditable);

            prodSearch_code[product.second->get_map()[L"код"]->get_wstring()] = {tables[prod_group.first], row};

            row++;
        }
        QObject::connect(tables[prod_group.first], &QTableWidget::cellDoubleClicked, [this, prod_group](int row, int col)
        {
            openProdCard(tables[prod_group.first]->item(row, 0)->text().toStdWString(), &tables);
        });
    }
}

void forms::prodTable_form::openProdCard(const std::wstring& code, std::map<std::wstring, QTableWidget*>* tables_)
{
    forms::make_productCard(mainWindow, data, log, tables_, code);
}

void forms::prodTable_form::setMainWindow_ptr(QMdiArea* mdiArea)
{
    mainWindow = mdiArea;
}

void forms::make_prodTable(QMdiArea* mdiArea, implData* data, QTextBrowser* log)
{
    auto* subWindow = new QMdiSubWindow(mdiArea);
    auto* pT_f = new forms::prodTable_form(data, log, subWindow);

    subWindow->setWidget(pT_f->mainWgt);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    pT_f->setMainWindow_ptr(mdiArea);
    pT_f->setupUI();

    mdiArea->addSubWindow(subWindow);

    subWindow->showMaximized();

    QObject::connect(pT_f->mainWgt, &QWidget::destroyed, [pT_f, subWindow, mdiArea]()
    {
        delete pT_f;
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });


}

