//
// Created by Maxim on 11.08.2022.
//

#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
#include <unordered_map>
#include "edit_prodGroup_form.h"

forms::prodGroup_editor::prodGroup_editor(QWidget* parent) : mainWgt(new QWidget(parent)), QObject(nullptr),
viewModel(new QTreeView(mainWgt)),
oT_model(new models::objectTree_model(viewModel)), data(nullptr), remElem_btn(new QPushButton),
addElem_btn(new QPushButton), textLine(new QLineEdit), gl(new QGridLayout), log(nullptr),
hl(new QHBoxLayout), hl1(new QHBoxLayout), hl2(new QHBoxLayout), vl(new QVBoxLayout)
{
    mainWgt->setAttribute(Qt::WA_DeleteOnClose);
}

forms::prodGroup_editor::~prodGroup_editor()
{
    delete oT_model;
    delete viewModel;
}

void forms::prodGroup_editor::setDataPtr(implData* data_)
{
    data = data_;
}

void forms::prodGroup_editor::setupUI()
{
    std::function<void(std::unordered_map<std::wstring, variant*>&, QObject*)> extractor =
            [&extractor, this](std::unordered_map<std::wstring, variant*>& map, QObject* parent)
            {
                for(auto&& it : map)
                {
                    obj_list.push_back(new QObject(parent));
                    obj_list.last()->setObjectName(QString::fromStdWString(it.first));

                    if(it.second->is_map() && !it.second->is_empty_map())
                    {
                        extractor(it.second->get_map(), obj_list[obj_list.size() - 1]);
                    }
                }
            };

    extractor(data->prodInfo.prodInfo.get()[L"Группы"]->get_map(), nullptr);

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

        auto& it = data->prodInfo.prodInfo.find(obj->parent()->objectName().toStdWString())->second;

        tag::mapWStrVar mwsv;
        it->get_map().insert(std::pair<std::wstring, variant*>(textLine->text().toStdWString(), new variant(mwsv)));
    });

    QObject::connect(remElem_btn, &QPushButton::clicked, [this](){
        auto* obj = static_cast<QObject*>(viewModel->selectionModel()->currentIndex().internalPointer());

        if(obj->objectName() == "Товары")
        {
            MainWindow::logger("Нельзя удалить основную группу!", log);
            return;
        }

        auto& parentGroupMap = data->prodInfo.prodInfo.find(obj->parent()->objectName().toStdWString())->second->get_map();
        auto& deriveGroupMap = data->prodInfo.prodInfo.find(obj->objectName().toStdWString())->second->get_map();
        auto& goodsMap = data->prodContain.products.get();
        auto& productsMap = data->prodContain.products.find(L"Товары")->second->get_map();

        std::vector<std::unordered_map<std::wstring, variant*>::iterator> it;
        std::vector<std::unordered_map<std::wstring, variant*>::iterator> end;
        size_t idx{0};

        it.push_back(deriveGroupMap.begin());
        end.push_back(deriveGroupMap.end());

        while(it[0] != end[0])
        {
            if(it[idx] == end[idx])
            {
                --idx;
                ++it[idx];

                it.pop_back();
                end.pop_back();
            }
            else
            {
                if(goodsMap.find(it[idx]->first) != goodsMap.end())
                {
                    for (auto &&product: goodsMap[it[idx]->first]->get_map())
                    {
                        product.second->get_map()[L"Группа"]->set_wstring(L"Товары");
                    }
                }
                productsMap.merge(data->prodContain.products.find(it[idx]->first)->second->get_map());

                if(!it[idx]->second->get_map().empty())
                {
                    it.push_back(it[idx]->second->get_map().begin());
                    end.push_back(it[idx]->second->get_map().end());
                    ++idx;
                }
                else
                {
                    ++it[idx];
                }
            }
        }

        productsMap.merge(data->prodContain.products.find(obj->objectName().toStdWString())->second->get_map());
        parentGroupMap.erase(parentGroupMap.find(obj->objectName().toStdWString()));
        obj->setObjectName("*удаление*");
    });
}

void forms::prodGroup_editor::setLog_ptr(QTextBrowser* logPtr)
{
    log = logPtr;
}

void forms::create_editProdGroupForm(QMdiArea* mdiArea, implData* data, QTextBrowser* log)
{
    auto subWindow = new QMdiSubWindow(mdiArea);
    auto newPG_e = new forms::prodGroup_editor(subWindow);

    subWindow->setWidget(newPG_e->mainWgt);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    newPG_e->setDataPtr(data);
    newPG_e->setLog_ptr(log);
    newPG_e->setupUI();

    mdiArea->addSubWindow(subWindow);

    subWindow->show();

    QObject::connect(newPG_e->mainWgt, &QWidget::destroyed, [newPG_e, subWindow, mdiArea]()
    {
        delete newPG_e;
        mdiArea->removeSubWindow(subWindow);
    });
}
