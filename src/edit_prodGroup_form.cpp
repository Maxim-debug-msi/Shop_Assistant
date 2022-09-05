//
// Created by Maxim on 11.08.2022.
//

#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
#include <unordered_map>
#include "edit_prodGroup_form.h"

forms::prodGroup_editor::prodGroup_editor() : mainWgt(new QWidget(nullptr)), viewModel(new QTreeView(mainWgt)),
oT_model(new models::objectTree_model(viewModel)), data(nullptr), model(nullptr), remElem_btn(new QPushButton),
addElem_btn(new QPushButton), textLine(new QLineEdit), gl(new QGridLayout),
hl(new QHBoxLayout), hl1(new QHBoxLayout), hl2(new QHBoxLayout), vl(new QVBoxLayout){}

forms::prodGroup_editor::~prodGroup_editor()
{
    //delete mainWgt;
}

void forms::prodGroup_editor::setModelPtr(dataContain::prodTree_model* model_)
{
    model = model_;
}

void forms::prodGroup_editor::setDataPtr(dataContain::prod_data* data_)
{
    data = data_;
}

void forms::prodGroup_editor::build()
{
    std::function<void(std::unordered_map<std::wstring, std::unique_ptr<variant>>&, QObject*)> extractor =
            [&extractor, this](std::unordered_map<std::wstring, std::unique_ptr<variant>>& map, QObject* parent)
            {
                for(auto&& it : map)
                {
                    obj_list.push_back(new QObject(parent));
                    obj_list[obj_list.size() - 1]->setObjectName(QString::fromStdWString(it.first));

                    if(it.second->is_map() && !it.second->is_empty_map())
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

    viewModel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    viewModel->setHeaderHidden(true);
    viewModel->setModel(oT_model);
    viewModel->setFixedWidth(250);

    remElem_btn->setText("Удалить выбранную группу");

    addElem_btn->setText("Добавить подгруппу");

    textLine->setPlaceholderText("Укажите название подгруппы");

    gl->addWidget(viewModel);

    hl1->addWidget(textLine);
    hl1->addWidget(addElem_btn);

    hl2->addWidget(remElem_btn);

    vl->addLayout(hl1);
    vl->addLayout(hl2);

    hl->addLayout(gl);
    hl->addLayout(vl);

    mainWgt->setWindowTitle("Редактор товарных групп");
    mainWgt->setLayout(hl);

    QObject::connect(addElem_btn, &QPushButton::clicked, [this](){
        if(textLine->text() == "") return;

        auto* obj = new QObject;
        obj->setObjectName(textLine->text());
        obj->setParent(static_cast<QObject*>(viewModel->selectionModel()->currentIndex().internalPointer()));

        std::wstring key = obj->parent()->objectName().toStdWString();
        auto& it = model->groupMap_.find(key)->second;

        tag::mapWStrVar mwsv;
        it->get_map().insert(std::pair<std::wstring, std::unique_ptr<variant>>(textLine->text().toStdWString(),
                                                                                     std::make_unique<variant>(mwsv)));
    });

    QObject::connect(remElem_btn, &QPushButton::clicked, [this](){
        auto*        obj = static_cast<QObject*>(viewModel->selectionModel()->currentIndex().internalPointer());
        auto&        map = model->groupMap_.find(obj->parent()->objectName().toStdWString())->second->get_map();
        auto         itForDel = map.find(obj->objectName().toStdWString());

        model->groupMap_.find(obj->parent()->objectName().toStdWString())->second->get_map().erase(itForDel);
        obj->setObjectName("*удаление*");
    });
}
