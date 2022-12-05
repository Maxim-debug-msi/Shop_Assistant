//
// Created by Maxim on 11.08.2022.
//

#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
#include <unordered_map>
#include "editProdGroup.h"

form::editProdGroup::editProdGroup(implData* data_, QTextBrowser* log_, QWidget* parent) : mainWgt(new QWidget(parent)),
QObject(nullptr), viewModel(new QTreeView(mainWgt)), rootModelObj(new QObject(mainWgt)),
oT_model(new models::objectTree_model(viewModel)), data(data_), remElem_btn(new QPushButton(mainWgt)),
addElem_btn(new QPushButton(mainWgt)), log(log_), main_l(new QVBoxLayout(mainWgt)),
buttons_l(new QHBoxLayout(mainWgt)), model_l(new QGridLayout(mainWgt))
{}

form::editProdGroup::~editProdGroup()
{
    delete oT_model;
}

void form::editProdGroup::setupUI()
{
    groupExtractor(data->prodInfo.prodInfo[L"Группы"]->get_map(), rootModelObj);

    QStringList cols;
    cols << "objectName";
    oT_model->setColumns(cols);

    oT_model->addItem(rootModelObj->children().first(), QModelIndex());

    viewModel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    viewModel->setHeaderHidden(true);
    viewModel->setModel(oT_model);
    viewModel->setFixedWidth(250);

    remElem_btn->setIcon(QIcon("../../icons/deleteIcon.png"));
    remElem_btn->setFixedSize(20, 20);
    remElem_btn->setToolTip("Удалить группу");

    addElem_btn->setIcon(QIcon("../../icons/addIcon.png"));
    addElem_btn->setFixedSize(20, 20);
    addElem_btn->setToolTip("Добавить группу");

    buttons_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    buttons_l->addWidget(addElem_btn);
    buttons_l->addWidget(remElem_btn);

    model_l->addWidget(viewModel);

    main_l->addLayout(buttons_l);
    main_l->addLayout(model_l);

    mainWgt->setWindowTitle("Редактор товарных групп");
    mainWgt->setLayout(main_l);

    QObject::connect(remElem_btn, &QPushButton::clicked, this, &form::editProdGroup::removeSelectedGroup);

    QObject::connect(addElem_btn, &QPushButton::clicked, this, &form::editProdGroup::addNewGroup);
}

void form::editProdGroup::groupExtractor(std::unordered_map<std::wstring, variant *>& map, QObject* parent)
{
    for(auto&& it : map)
    {
        auto* obj = new QObject(parent);
        obj->setObjectName(QString::fromStdWString(it.first));

        if(it.second->is_map() && !it.second->is_empty_map())
        {
            groupExtractor(it.second->get_map(), obj);
        }
    }
}

void form::editProdGroup::addNewGroup()
{
    tag::mapWStrVar mwsv;
    auto* obj = new QObject(static_cast<QObject*>(viewModel->selectionModel()->currentIndex().internalPointer()));
    auto& parentMap = data->prodInfo.prodInfo.search(obj->parent()->objectName().toStdWString())->second;
    auto* dlg = new dialWindow_str(mainWgt);

    switch (dlg->exec())
    {
        case QDialog::Accepted:
            objName = dlg->getInput();
            obj->setObjectName(objName);
            parentMap->get_map().insert(std::pair<std::wstring, variant*>(objName.toStdWString(), new variant(mwsv)));
            delete obj;
            refreshModel();
            saveChanges("");
            break;

        case QDialog::Rejected:
            delete obj;
            break;

    }
}

void form::editProdGroup::removeSelectedGroup()
{
    auto* obj = static_cast<QObject*>(viewModel->selectionModel()->currentIndex().internalPointer());

    objName = obj->objectName();

    auto goodsMapIt = data->prodContain.products.search(obj->objectName().toStdWString());

    if(obj->objectName() == "Товары")
    {
        utl::logger("Нельзя удалить основную группу!", log);
        return;
    }
    else if(!obj->children().empty())
    {
        utl::logger("Данная группа включает в себя одну или несколько подгрупп. "
                    "Для удаления необходимо очистить все подгруппы.", log);
        return;
    }
    else if(goodsMapIt == data->prodContain.products.end())
    {
        data->prodInfo.prodInfo.search(obj->parent()->objectName().toStdWString())->second->get_map().
        erase(data->prodInfo.prodInfo.search(obj->objectName().toStdWString()));
        refreshModel();
        saveChanges("");
        return;
    }
    else if(goodsMapIt != data->prodContain.products.end() || goodsMapIt->second->is_empty_map())
    {
        auto* dlg_del = new dialWindow_del(data, &objName, mainWgt);
        auto* productOut = &data->prodContain.products.search(obj->objectName().toStdWString())->second->get_map();
        std::unordered_map<std::wstring, variant*>* productIn{nullptr};

        switch (dlg_del->exec())
        {
            case QDialog::Accepted:
                productIn = &data->prodContain.products.search(objName.toStdWString())->second->get_map();

                for(auto&& it : *productOut)
                {
                    it.second->get_map()[L"группа"]->set_wstring(objName.toStdWString());
                }

                productIn->merge(*productOut);
                data->prodContain.products.erase(obj->objectName().toStdWString());
                data->prodInfo.prodInfo.search(obj->parent()->objectName().toStdWString())->second->get_map().
                erase(data->prodInfo.prodInfo.search(obj->objectName().toStdWString()));
                refreshModel();
                saveChanges("");
                break;

            case QDialog::Rejected:

                break;

        }

        QObject::connect(dlg_del, &form::dialWindow_del::deleting, [this, obj](){
            data->prodInfo.prodInfo.search(obj->objectName().toStdWString())->second->get_map().
            erase(data->prodInfo.prodInfo.search(obj->objectName().toStdWString()));
            data->prodContain.products.erase(obj->objectName().toStdWString());

            refreshModel();
            saveChanges("");
        });
    }
}

void form::editProdGroup::refreshModel()
{
    viewModel->hide();

    auto* tmpPtr = rootModelObj;
    delete tmpPtr;
    rootModelObj = new QObject(mainWgt);

    auto* tmpModelPtr = oT_model;
    delete tmpModelPtr;
    oT_model = new models::objectTree_model(viewModel);

    groupExtractor(data->prodInfo.prodInfo[L"Группы"]->get_map(), rootModelObj);
    QStringList cols;
    cols << "objectName";
    oT_model->setColumns(cols);
    oT_model->addItem(rootModelObj->children().first(), QModelIndex());
    viewModel->setModel(oT_model);

    viewModel->show();
}

void form::editProdGroup::saveChanges(const std::string& filePath)
{
    data->prodContain.save("../../data/products/products.json");
    data->prodInfo.save("../../data/products/prodInfo.json");
}

void form::create_editProdGroup(QMdiArea* mdiArea, implData* data, QTextBrowser* log)
{
    auto subWindow = new QMdiSubWindow(mdiArea);
    auto newPG_e = new form::editProdGroup(data, log, subWindow);

    subWindow->setWidget(newPG_e->mainWgt);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    newPG_e->setupUI();

    mdiArea->addSubWindow(subWindow);

    subWindow->show();

    QObject::connect(newPG_e->mainWgt, &QWidget::destroyed, [newPG_e, subWindow, mdiArea]()
    {
        delete newPG_e;
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });
}
