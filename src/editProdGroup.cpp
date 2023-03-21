//
// Created by Maxim on 11.08.2022.
//

#include "editProdGroup.h"

form::editProdGroup::editProdGroup(implData* data_, QTextBrowser* log_, QWidget* parent) : QWidget(parent),
view_model(new QTreeView(this)), root_model_obj(new QObject(this)),
obj_tree_model(new models::objectTree_model(view_model)), data(data_), remove_elem_btn(new QPushButton(this)),
add_elem_btn(new QPushButton(this)), log(log_), main_l(new QVBoxLayout(this)),
buttons_l(new QHBoxLayout(this)), model_l(new QGridLayout(this))
{}

form::editProdGroup::~editProdGroup()
{

}

void form::editProdGroup::setupUI()
{
    QStringList cols;
    cols << "objectName";
    obj_tree_model->setColumns(cols);

    if(!data->prodInfo.prodInfo[L"groups"]->map().empty())
    {
        groupExtractor(data->prodInfo.prodInfo[L"groups"]->map(), root_model_obj);
        for(auto&& obj : root_model_obj->children())
        {
            obj_tree_model->addItem(obj, QModelIndex());
        }
    }

    view_model->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    view_model->setHeaderHidden(true);
    view_model->setModel(obj_tree_model);
    view_model->setFixedWidth(250);
    view_model->viewport()->installEventFilter(this);

    remove_elem_btn->setIcon(QIcon("../../icons/deleteIcon.png"));
    remove_elem_btn->setFixedSize(20, 20);
    remove_elem_btn->setToolTip("Удалить группу");

    add_elem_btn->setIcon(QIcon("../../icons/addIcon.png"));
    add_elem_btn->setFixedSize(20, 20);
    add_elem_btn->setToolTip("Добавить группу");

    buttons_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    buttons_l->addWidget(add_elem_btn);
    buttons_l->addWidget(remove_elem_btn);

    model_l->addWidget(view_model);

    main_l->addLayout(buttons_l);
    main_l->addLayout(model_l);

    this->setWindowTitle("Редактор товарных групп");
    this->setWindowIcon(QIcon("../../icons/form.png"));
    this->setLayout(main_l);

    QObject::connect(remove_elem_btn, &QPushButton::clicked, this, &form::editProdGroup::removeSelectedGroup);
    QObject::connect(add_elem_btn, &QPushButton::clicked, this, &form::editProdGroup::addNewGroup);
}

void form::editProdGroup::groupExtractor(std::unordered_map<std::wstring, variant *>& map, QObject* parent)
{
    for(auto&& it : map)
    {
        auto* obj = new QObject(parent);
        obj->setObjectName(QString::fromStdWString(it.first));

        if(it.second->is_map() && !it.second->map().empty())
        {
            groupExtractor(it.second->map(), obj);
        }
    }
}

void form::editProdGroup::addNewGroup()
{
    auto *dlg = new dialWindow_str(this);
    QObject* obj;
    std::unordered_map<std::wstring, variant*>* parentMap;

    if(view_model->selectionModel()->isSelected(view_model->selectionModel()->currentIndex()))
    {
        obj = new QObject(static_cast<QObject *>(view_model->selectionModel()->currentIndex().internalPointer()));
        parentMap = &data->prodInfo.prodInfo.search(obj->parent()->objectName().toStdWString())->second->map();
    }
    else
    {
        obj = new QObject(this);
    }
    switch (dlg->exec()) {
        case QDialog::Accepted:
            object_name = dlg->getInput();
            obj->setObjectName(object_name);
            if(view_model->selectionModel()->isSelected(view_model->selectionModel()->currentIndex()))
            {
                parentMap->insert({object_name.toStdWString(), new variant(tag::mapWStrVar())});
            }
            else
            {
                data->prodInfo.prodInfo[L"groups"]->map().insert({object_name.toStdWString(), new variant(tag::mapWStrVar())});
            }
            delete obj;
            refreshModel();
            saveChanges("");
            break;

        case QDialog::Rejected:
            delete obj;
            break;

    }

    dlg->deleteLater();
}

void form::editProdGroup::removeSelectedGroup()
{
    if(!view_model->selectionModel()->isSelected(view_model->selectionModel()->currentIndex()))
    {
        utl::logger("Нет выбранной группы!", log);
        return;
    }

    auto* group_for_delete = static_cast<QObject*>(view_model->selectionModel()->currentIndex().internalPointer());

    std::wstring deleting_group_name = group_for_delete->objectName().toStdWString();

    auto selected_group_products_it = data->prodContain.products.search(deleting_group_name);

    if(!group_for_delete->children().empty())
    {
        utl::logger("Данная группа включает в себя одну или несколько подгрупп. "
                    "Для удаления необходимо очистить все подгруппы.", log);
        return;
    }
    else if(selected_group_products_it == data->prodContain.products.end())
    {
        if(group_for_delete->parent()->objectName().isEmpty())
        {
            data->prodInfo.prodInfo[L"groups"]->map().erase(deleting_group_name);
        }
        else
        {
            data->prodInfo.prodInfo.search(group_for_delete->parent()->objectName().toStdWString())->second->map().
                    erase(data->prodInfo.prodInfo.search(deleting_group_name));
        }
        refreshModel();
        saveChanges("");
        return;
    }
    else
    {
        QString name_of_new_group;
        auto* dlg_del = new delete_group_dialog(data, &name_of_new_group, this);
        auto* product_out = &selected_group_products_it->second->map();
        std::unordered_map<std::wstring, variant*>* product_in;

        switch (dlg_del->exec())
        {
            case QDialog::Accepted:
                product_in = &data->prodContain.products.search(name_of_new_group.toStdWString())->second->map();

                for(auto&& it : *product_out)
                {
                    it.second->map()[L"groups"]->wstring() = name_of_new_group.toStdWString();
                }

                product_in->merge(*product_out);
                data->prodContain.products.erase(deleting_group_name);

                if(group_for_delete->parent() == root_model_obj)
                {
                    data->prodInfo.prodInfo[L"groups"]->map().erase(deleting_group_name);
                }
                else
                {
                    data->prodInfo.prodInfo.search(group_for_delete->parent()->objectName().toStdWString())->second->map().
                            erase(data->prodInfo.prodInfo.search(deleting_group_name));
                }

                refreshModel();
                saveChanges("");
                break;

            case QDialog::Rejected:

                break;

        }

        QObject::connect(dlg_del, &form::delete_group_dialog::deleting, [this, deleting_group_name](){
            data->prodInfo.prodInfo.search(deleting_group_name)->second->map().
            erase(data->prodInfo.prodInfo.search(deleting_group_name));
            data->prodContain.products.erase(deleting_group_name);

            refreshModel();
            saveChanges("");
        });
    }
}

void form::editProdGroup::refreshModel()
{
    view_model->hide();

    auto* tmpPtr = root_model_obj;
    delete tmpPtr;
    root_model_obj = new QObject(this);

    auto* tmpModelPtr = obj_tree_model;
    delete tmpModelPtr;
    obj_tree_model = new models::objectTree_model(view_model);

    QStringList cols;
    cols << "objectName";
    obj_tree_model->setColumns(cols);

    if(!data->prodInfo.prodInfo[L"groups"]->map().empty())
    {
        groupExtractor(data->prodInfo.prodInfo[L"groups"]->map(), root_model_obj);
        for(auto&& obj : root_model_obj->children())
        {
            obj_tree_model->addItem(obj, QModelIndex());
        }
    }

    view_model->setModel(obj_tree_model);
    view_model->show();
}

void form::editProdGroup::saveChanges(const std::string& filePath)
{
    data->prodContain.save("../../data/products/products.json");
    data->prodInfo.save("../../data/products/prodInfo.json");
}

bool form::editProdGroup::eventFilter(QObject *watcher, QEvent *event)
{
    if(watcher == view_model->viewport() && event->type() == QEvent::MouseButtonRelease)
    {
        auto* me = dynamic_cast<QMouseEvent*>(event);
        QModelIndex index = view_model->indexAt(me->pos());

        if(!index.isValid())
        {
            view_model->clearSelection();
            view_model->clearFocus();
        }
        return true;
    }
    return QWidget::eventFilter(watcher, event);
}

void form::create_editProdGroup(QMdiArea* mdiArea, implData* data, QTextBrowser* log)
{
    auto subWindow = new QMdiSubWindow(mdiArea);
    auto newPG_e = new form::editProdGroup(data, log, subWindow);

    subWindow->setWidget(newPG_e);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    newPG_e->setupUI();

    mdiArea->addSubWindow(subWindow);

    subWindow->show();

    QObject::connect(newPG_e, &QWidget::destroyed, [subWindow, mdiArea]()
    {
        //mdiArea->closeActiveSubWindow();
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });
}
