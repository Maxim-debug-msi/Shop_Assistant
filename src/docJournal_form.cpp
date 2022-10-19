//
// Created by Maxim on 25.09.2022.
//

#include <docJournal_form.h>
#include <addProd_form.h>

forms::docJournal_form::docJournal_form() : QObject(nullptr), mainWgt(new QWidget(nullptr)), data(nullptr),
                                     summaryDL(new QLineEdit(mainWgt)), hl4(new QHBoxLayout),
                                     oT_model(new models::objectTree_model(docList)), docList(new QComboBox(mainWgt)),
                                     log(nullptr), tableWgt(new QWidget(mainWgt)), mainWindow(nullptr),
                                     hl1(new QHBoxLayout), hl3(new QHBoxLayout), vl(new QVBoxLayout(mainWgt)),
                                     hl11(new QHBoxLayout), search_btn(new QPushButton(mainWgt)),
                                     summaryCreator(new QLineEdit(mainWgt)), creator(new QComboBox(mainWgt)),
                                     hl2(new QHBoxLayout), hl21(new QHBoxLayout), onOffCreator_btn(new QCheckBox(mainWgt)){}

forms::docJournal_form::~docJournal_form()
{
    delete mainWgt;
}

void forms::docJournal_form::setupUI()
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

    for(auto&& docType : data->docInfo.docInfo[L"Тип"]->get_map())
    {
        tables[docType.first] = new QTableWidget();
        tables[docType.first]->setColumnCount(5);
        tables[docType.first]->setRowCount(static_cast<int>(data->docsPtr[docType.first]->docsOfDate.size()));
        tables[docType.first]->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
        tables[docType.first]->setHorizontalHeaderLabels({"Номер", "Дата", "Тип", "Автор", "Комментарий"});

        int row{0};
        auto docPtr = data->docsPtr[docType.first]->docs;
        for(auto&& docDate : data->docsPtr[docType.first]->docsOfDate)
        {
            tables[docType.first]->setItem(row, 0, new QTableWidgetItem(QString::fromStdWString
            (docPtr.find(std::to_wstring(docDate))->second->get_map()[L"Номер"]->get_wstring())));

            tables[docType.first]->item(row, 0)->setIcon(QIcon(QString::fromStdString
            (iconsPaths[docPtr.find(std::to_wstring(docDate))->second->get_map()[L"Статус"]->get_wstring()])));

            tables[docType.first]->item(row, 0)->setFlags(tables[docType.first]->item
            (row, 0)->flags() ^ Qt::ItemIsEditable);

            tables[docType.first]->setItem(row, 1, new QTableWidgetItem(QString::fromStdWString
            (docPtr.find(std::to_wstring(docDate))->second->get_map()[L"Дата"]->get_wstring())));
            tables[docType.first]->item(row, 1)->setFlags(tables[docType.first]->item
            (row, 1)->flags() ^ Qt::ItemIsEditable);
            tables[docType.first]->item(row, 1)->setWhatsThis(QString::fromStdWString(std::to_wstring(docDate)));

            tables[docType.first]->setItem(row, 2, new QTableWidgetItem(QString::fromStdWString(docType.first)));

            tables[docType.first]->item(row, 2)->setFlags(tables[docType.first]->item
            (row, 2)->flags() ^ Qt::ItemIsEditable);

            tables[docType.first]->setItem(row, 3, new QTableWidgetItem(QString::fromStdWString
            (docPtr.find(std::to_wstring(docDate))->second->get_map()[L"Автор"]->get_wstring())));

            tables[docType.first]->item(row, 3)->setFlags(tables[docType.first]->item
            (row, 1)->flags() ^ Qt::ItemIsEditable);

            tables[docType.first]->setItem(row, 4, new QTableWidgetItem(QString::fromStdWString
            (docPtr.find(std::to_wstring(docDate))->second->get_map()[L"Комментарий"]->get_wstring())));

            tables[docType.first]->item(row, 4)->setFlags(tables[docType.first]->item
            (row, 3)->flags() ^ Qt::ItemIsEditable);

            row++;
        }
        tables[docType.first]->resizeColumnsToContents();

        QObject::connect(tables[docType.first], &QTableWidget::cellDoubleClicked, [this, docType](int row, int col)
        {
            //std::wcout << tables[docType.first]->item(row, 1)->text().toStdWString() << std::endl;
            openDoc(tables[docType.first]->item(row, 1)->whatsThis().toStdWString());
        });
    }

    QObject::connect(search_btn, SIGNAL(clicked(bool)), this, SLOT(drawTable()));
}

void forms::docJournal_form::setData_ptr(implData* data_)
{
    data = data_;
}

void forms::docJournal_form::setLog_ptr(QTextBrowser* log_)
{
    log = log_;
}

void forms::docJournal_form::drawTable()
{
    std::wstring key{docList->currentText().toStdWString()};

    if(!tableWgt->layout()->isEmpty())
    {
        auto widget = tableWgt->layout()->takeAt(0)->widget();
        widget->setVisible(false);
        tableWgt->layout()->removeWidget(widget);
    }

    tableWgt->layout()->addWidget(tables[key]);
    tables[key]->setVisible(true);
}

void forms::docJournal_form::openDoc(const std::wstring& docNum)
{

    switch (std::stoi(data->docInfo.docInfo[L"Тип"]->get_map()[docList->currentText().toStdWString()]->get_wstring()))
    {
        case 1:
        {
            auto newAP_f = new forms::addProduct_form(docNum);
            auto subWindow = new QMdiSubWindow(mainWindow);

            subWindow->setWidget(newAP_f->mainWgt);
            subWindow->setAttribute(Qt::WA_DeleteOnClose);

            newAP_f->setDataPtr(data);
            newAP_f->setLog_ptr(log);
            newAP_f->setupUI();

            mainWindow->addSubWindow(subWindow);

            subWindow->show();

            QObject::connect(newAP_f->mainWgt, &QWidget::destroyed, [newAP_f, subWindow, this]()
            {
                delete newAP_f;
                mainWindow->removeSubWindow(subWindow);
            });

            break;
        }
        default:
        {
            break;
        }
    }
}

void forms::docJournal_form::setMainWindow_ptr(QMdiArea* mainWindow_)
{
    mainWindow = mainWindow_;
}


