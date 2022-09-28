//
// Created by Maxim on 25.09.2022.
//

#include <docJournal_form.h>

forms::docJournal_form::docJournal_form() : QObject(nullptr), mainWgt(new QWidget(nullptr)), data(nullptr),
                                     summaryDL(new QLineEdit(mainWgt)),
                                     oT_model(new models::objectTree_model(docList)), docList(new QComboBox(mainWgt)),
                                     log(nullptr), nameBuf(new std::wstring), mainTable(new QTableWidget(mainWgt)),
                                     hl1(new QHBoxLayout), hl2(new QHBoxLayout), vl(new QVBoxLayout(mainWgt)),
                                     hl11(new QHBoxLayout), search_btn(new QPushButton(mainWgt)){}

forms::docJournal_form::~docJournal_form()
{
    delete mainWgt;
}

void forms::docJournal_form::setupUI()
{
    std::function<void(std::unordered_map<std::wstring, std::unique_ptr<variant>>&)> extractor =
            [&extractor, this](std::unordered_map<std::wstring, std::unique_ptr<variant>>& map)
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

    docList->setFixedSize(150, 20);

    search_btn->setFixedSize(80, 20);
    search_btn->setText("Поиск");

    hl11->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl11->addWidget(summaryDL);
    hl11->addWidget(docList);

    hl1->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl1->addLayout(hl11);

    hl2->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl2->addWidget(search_btn);

    vl->addLayout(hl1);
    vl->addLayout(hl2);
    vl->addWidget(mainTable);

    for(auto&& docType : data->docInfo.docInfo[L"Тип"]->get_map())
    {
        tables[docType.first] = new QTableWidget(mainWgt);

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

}

