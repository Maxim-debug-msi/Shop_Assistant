//
// Created by Maxim on 02.09.2022.
//

#include <QLineEdit>
#include <QStringListModel>
#include "receptFromTransit.h"

form::receptFromTransit::receptFromTransit() : QObject(nullptr), mainWgt(new QWidget),
                            table(new QTableWidget(mainWgt)), data(nullptr),
                            addProduct(new QPushButton(mainWgt)), groupList(new QComboBox(mainWgt)),
                            vl(new QVBoxLayout(mainWgt)), hl(new QHBoxLayout), gl(new QGridLayout),
                            oT_model(new models::objectTree_model(groupList)),
                            write(new QPushButton(mainWgt)), enter(new QPushButton(mainWgt)), hl2(new QHBoxLayout),
                            delProduct(new QPushButton(mainWgt)){}

form::receptFromTransit::~receptFromTransit()
{
    delete mainWgt;
}

void form::receptFromTransit::setDataPtr(implData* dataPtr)
{
    data = dataPtr;
}

void form::receptFromTransit::setupUi()
{
    std::function<void(std::unordered_map<std::wstring, variant*>&)> extractor =
            [&extractor, this](std::unordered_map<std::wstring, variant*>& map)
            {
                for(auto&& it : map)
                {
                    obj_list.push_back(new QObject);
                    obj_list[obj_list.size() - 1]->setObjectName(QString::fromStdWString(it.first));

                    oT_model->addItem(obj_list[obj_list.size() - 1], QModelIndex());

                    if(it.second != nullptr && it.second->is_map() && !it.second->map().empty())
                    {
                        extractor(it.second->map());
                    }
                }
            };

    QStringList cols;
    cols << "objectName";
    oT_model->setColumns(cols);

    //extractor(data->jsonData.prodInfo[L"group"]->map());

    groupList->setModel(oT_model);

    mainWgt->setWindowTitle("Прием с транзита");

    addProduct->setIcon(QIcon("../../icons/addIcon.png"));
    addProduct->setFixedSize(20, 20);
    addProduct->setToolTip("Добавить позицию");
    addProduct->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    delProduct->setIcon(QIcon("../../icons/deleteIcon.png"));
    delProduct->setFixedSize(20, 20);
    delProduct->setToolTip("Удалить выбранную позицию");
    delProduct->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    write->setFixedSize(80, 20);
    write->setText("Записать");

    enter->setFixedSize(80,20);
    enter->setText("Провести");

    groupList->setFixedSize(200, 20);

    hl->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hl->addWidget(addProduct);
    hl->addWidget(delProduct);
    hl->addWidget(groupList);

    hl2->setAlignment(Qt::AlignRight | Qt::AlignTop);
    hl2->addWidget(write);
    hl2->addWidget(enter);

    gl->addWidget(table);

    vl->addLayout(hl);
    vl->addLayout(gl);
    vl->addLayout(hl2);
}



