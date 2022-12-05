//
// Created by Maxim on 25.09.2022.
//

#include <docJournal.h>

form::docJournal::docJournal(QWidget* parent) : QObject(nullptr), mainWgt(new QWidget(parent)), data(nullptr),
                                     summaryDL(new QLineEdit(mainWgt)), hl4(new QHBoxLayout), table(new QTableWidget),
                                     oT_model(new models::objectTree_model(docList)), docList(new QComboBox(mainWgt)),
                                     log(nullptr), tableWgt(new QWidget(mainWgt)), mainWindow(nullptr),
                                     hl1(new QHBoxLayout), hl3(new QHBoxLayout), vl(new QVBoxLayout(mainWgt)),
                                     hl11(new QHBoxLayout), search_btn(new QPushButton(mainWgt)),
                                     summaryCreator(new QLineEdit(mainWgt)), creator(new QComboBox(mainWgt)),
                                     hl2(new QHBoxLayout), hl21(new QHBoxLayout), onOffCreator_btn(new QCheckBox(mainWgt)){}

form::docJournal::~docJournal()
{
    delete mainWgt;
}

void form::docJournal::setupUI()
{
    std::function<void(std::unordered_map<std::wstring, variant*>&)> extractor =
            [&extractor, this](std::unordered_map<std::wstring, variant*>& map)
            {
                for(auto&& it : map)
                {
                    obj_list.push_back(new QObject);
                    obj_list.last()->setObjectName(QString::fromStdWString(it.first));

                    oT_model->addItem(obj_list.last(), QModelIndex());

                    if(it.second != nullptr && it.second->is_map() && !it.second->is_empty_map())
                    {
                        extractor(it.second->get_map());
                    }
                }
            };

    QStringList cols;
    cols << "objectName";
    oT_model->setColumns(cols);

    extractor(data->docInfo.docInfo[L"Тип"]->get_map());

    mainWgt->setWindowTitle("Журнал документов");

    docList->setModel(oT_model);

    summaryDL->setText("Тип документа:");
    summaryDL->setFrame(false);
    summaryDL->setReadOnly(true);
    summaryDL->setFixedSize(150, 20);
    docList->setFixedSize(250, 20);

    summaryCreator->setText("Автор:");
    summaryCreator->setFrame(false);
    summaryCreator->setReadOnly(true);
    summaryCreator->setFixedSize(150, 20);
    creator->setFixedSize(250, 20);
    onOffCreator_btn->setToolTip("Включить\\выключить отбор по автору");

    search_btn->setFixedSize(80, 20);
    search_btn->setText("Поиск");

    tableWgt->setLayout(new QGridLayout(tableWgt));
    tableWgt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tableWgt->layout()->addWidget(table);

    hl11->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl11->addWidget(summaryDL);
    hl11->addWidget(docList);

    hl1->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl1->addLayout(hl11);

    hl21->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl21->addWidget(summaryCreator);
    hl21->addWidget(creator);
    hl21->addWidget(onOffCreator_btn);

    hl2->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl2->addLayout(hl21);

    hl3->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl3->addWidget(search_btn);

    hl4->addWidget(tableWgt);

    vl->addLayout(hl1);
    vl->addLayout(hl2);
    vl->addLayout(hl3);
    vl->addLayout(hl4);

    mainWgt->setLayout(vl);

    QObject::connect(search_btn, SIGNAL(clicked(bool)), this, SLOT(drawTable()));
}

void form::docJournal::setData_ptr(implData* data_)
{
    data = data_;
}

void form::docJournal::setLog_ptr(QTextBrowser* log_)
{
    log = log_;
}

void form::docJournal::drawTable()
{
    table->setVisible(false);
    table->clear();

    table->setColumnCount(5);
    table->setRowCount(static_cast<int>(data->docsPtr[docList->currentText().toStdWString()]->docsOfDate.size()));
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    table->setHorizontalHeaderLabels({"Номер", "Дата", "Тип", "Автор", "Комментарий"});

    int row{0};
    auto docPtr = data->docsPtr[docList->currentText().toStdWString()]->docs;
    for(auto&& docDate : data->docsPtr[docList->currentText().toStdWString()]->docsOfDate)
    {
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdWString
        (docPtr.search(std::to_wstring(docDate))->second->get_map()[L"Номер"]->get_wstring())));

        table->item(row, 0)->setIcon(QIcon(QString::fromStdString
        (data->docStatusIcons[docPtr.search(std::to_wstring(docDate))->second->get_map()[L"Статус"]->get_wstring()])));
        table->item(row, 0)->setFlags(table->item(row, 0)->flags() ^ Qt::ItemIsEditable);

        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdWString
        (docPtr.search(std::to_wstring(docDate))->second->get_map()[L"Дата"]->get_wstring())));
        table->item(row, 1)->setFlags(table->item(row, 1)->flags() ^ Qt::ItemIsEditable);
        table->item(row, 1)->setWhatsThis(QString::fromStdWString(std::to_wstring(docDate)));

        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdWString(docList->currentText().toStdWString())));
        table->item(row, 2)->setFlags(table->item(row, 2)->flags() ^ Qt::ItemIsEditable);

        table->setItem(row, 3, new QTableWidgetItem(QString::fromStdWString
        (docPtr.search(std::to_wstring(docDate))->second->get_map()[L"Автор"]->get_wstring())));
        table->item(row, 3)->setFlags(table->item(row, 1)->flags() ^ Qt::ItemIsEditable);

        table->setItem(row, 4, new QTableWidgetItem(QString::fromStdWString
        (docPtr.search(std::to_wstring(docDate))->second->get_map()[L"Комментарий"]->get_wstring())));
        table->item(row, 4)->setFlags(table->item(row, 3)->flags() ^ Qt::ItemIsEditable);

        row++;
    }
    table->resizeColumnsToContents();

    QObject::connect(table, &QTableWidget::cellDoubleClicked, [this](int row, int col)
    {
        openDoc(table->item(row, 1)->whatsThis().toStdWString(), row);

    });

    table->setVisible(true);
}

void form::docJournal::openDoc(const std::wstring& docNum, int& row)
{

    switch (std::stoi(data->docInfo.docInfo[L"Тип"]->get_map()[docList->currentText().toStdWString()]->get_wstring()))
    {
        case 1:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

void form::docJournal::setMainWindow_ptr(QMdiArea* mainWindow_)
{
    mainWindow = mainWindow_;
}

implData *form::docJournal::getData_ptr()
{
    return data;
}



