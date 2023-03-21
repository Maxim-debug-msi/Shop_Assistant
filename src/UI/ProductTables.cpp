//
// Created by Maxim on 22.12.2022.
//
#include "UI/productTables/ProductTables.h"
#include "defs.h"

form::ProductTables::ProductTables(implData* data, QTextBrowser* log, QMdiArea* mdi, QWidget *parent) : QWidget(parent),
ui_(new Ui::ProductTables()), model_(new models::objectTree_model(this)), data_(data), log_(log), mdiArea_(mdi),
rootModelObj_(new QObject(this))
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFocus();

    ui_->setupUi(this);
    ui_->groups_tree->viewport()->installEventFilter(this);
    ui_->group_window_menu_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui_->goods_tables_menu_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui_->table_view_wgt->setLayout(new QGridLayout(ui_->table_view_wgt));
    ui_->groups_tree->setHeaderHidden(true);

    fillModel();
    fillTables();

    QObject::connect(ui_->group_tree_show_hide_btn, &QPushButton::clicked, this, &ProductTables::showHideGroupTree);

    QObject::connect(ui_->add_prod_btn, &QPushButton::clicked, this, &ProductTables::addProduct);
    QObject::connect(ui_->print_label_btn, &QPushButton::clicked, this, &ProductTables::openThermalPrintForm);
    QObject::connect(ui_->add_group_btn, &QPushButton::clicked, this, &ProductTables::addProductGroup);
    QObject::connect(ui_->delete_group_btn, &QPushButton::clicked, this, &ProductTables::deleteProductGroup);
    QObject::connect(ui_->groups_tree, &QTreeView::clicked, this, &ProductTables::changeVisibleTable);
    QObject::connect(ui_->search_btn, &QPushButton::clicked, this, &ProductTables::findProductDlg);
}

form::ProductTables::~ProductTables()
{
    delete ui_;
    this->disconnect();
}

void form::ProductTables::fillTables()
{
    QStringList group_list{utl::getProductGroupsList(data_)};

    for(auto&& group : group_list)
    {
        fillTable(group);
    }
}

void form::ProductTables::fillModel()
{
    ui_->groups_tree->hide();

    auto* tmpPtr = rootModelObj_;
    delete tmpPtr;
    rootModelObj_ = new QObject(this);

    auto* tmpModelPtr = model_;
    delete tmpModelPtr;
    model_ = new models::objectTree_model(ui_->groups_tree);

    QStringList cols;
    cols << "objectName";
    model_->setColumns(cols);

    if(!data_->jsonData.prodInfo[L"groups"]->map().empty())
    {
        utl::getProductGroupsTree(data_, rootModelObj_);
        for(auto&& obj : rootModelObj_->children())
        {
            model_->addItem(obj, QModelIndex());
        }
    }

    ui_->groups_tree->setModel(model_);
    ui_->groups_tree->show();
}

void form::ProductTables::changeVisibleTable()
{
    QString key{model_->data(ui_->groups_tree->selectionModel()->currentIndex(), Qt::DisplayRole).toString()};

    if(!ui_->table_view_wgt->layout()->isEmpty())
    {
        auto widget = ui_->table_view_wgt->layout()->takeAt(0)->widget();
        widget->setVisible(false);
        ui_->table_view_wgt->layout()->removeWidget(widget);
    }

    if(tables_.find(key) == tables_.end())
    {
        return;
    }

    ui_->table_view_wgt->layout()->addWidget(tables_[key]);
    tables_[key]->setVisible(true);
}

void form::ProductTables::openProdCard(const int& row, const int& col)
{
    int code{tables_[model_->data(ui_->groups_tree->selectionModel()->currentIndex(), Qt::DisplayRole).
    toString()]->item(row, 1)->text().toInt()};

    auto* ptr = form::makeProductCard(data_, log_, mdiArea_, code);

    QObject::connect(ptr, &form::ProductCard::changed, this, &form::ProductTables::fillTable);
}

void form::ProductTables::addProduct()
{
    auto* ptr = form::makeProductCard(data_, log_, mdiArea_);

    QObject::connect(ptr, &form::ProductCard::changed, this, &form::ProductTables::fillTable);
}

void form::ProductTables::openThermalPrintForm()
{
    form::makeThermalPrinter(data_, log_, mdiArea_);
}

void form::ProductTables::showHideGroupTree()
{
    if(ui_->groups_tree->isHidden())
    {
        ui_->groups_tree->show();
        ui_->add_group_btn->show();
        ui_->delete_group_btn->show();
        ui_->group_tree_show_hide_btn->setText("<");
    }
    else
    {
        ui_->groups_tree->hide();
        ui_->add_group_btn->hide();
        ui_->delete_group_btn->hide();
        ui_->group_tree_show_hide_btn->setText(">");
    }
}

void form::ProductTables::addProductGroup()
{
    auto *dlg = new form_dialog::addGroup_dlg(this);
    QObject* obj;
    QString obj_name;
    std::unordered_map<std::wstring, variant*>* parentMap;

    if(ui_->groups_tree->selectionModel()->isSelected(ui_->groups_tree->selectionModel()->currentIndex()))
    {
        obj = new QObject(static_cast<QObject *>(ui_->groups_tree->selectionModel()->currentIndex().internalPointer()));
        parentMap = &data_->jsonData.prodInfo.search(obj->parent()->objectName().toStdWString())->second->map();
    }
    else
    {
        obj = new QObject();
    }
    switch (dlg->exec()) {
        case QDialog::Accepted:
            obj_name = dlg->getInput();
            obj->setObjectName(obj_name);
            if(ui_->groups_tree->selectionModel()->isSelected(ui_->groups_tree->selectionModel()->currentIndex()))
            {
                parentMap->insert({obj_name.toStdWString(), new variant(tag::mapWStrVar())});
            }
            else
            {
                data_->jsonData.prodInfo[L"groups"]->map().insert({obj_name.toStdWString(), new variant(tag::mapWStrVar())});
            }
            delete obj;

            tables_.insert(obj_name, new QTableWidget(ui_->table_view_wgt));

            data_->jsonData.prodInfo.fwrite("../data/products/prodInfo.json");

            break;

        case QDialog::Rejected:
            delete obj;
            break;

    }

    dlg->deleteLater();

    fillModel();
}

void form::ProductTables::deleteProductGroup()
{
    if(!ui_->groups_tree->selectionModel()->isSelected(ui_->groups_tree->selectionModel()->currentIndex()))
    {
        utl::logger("Нет выбранной группы!", log_);
        return;
    }

    auto* group_for_delete = static_cast<QObject*>(ui_->groups_tree->selectionModel()->currentIndex().internalPointer());

    QString deleted_group_name = group_for_delete->objectName();

    if(!group_for_delete->children().empty())
    {
        utl::logger("Данная группа включает в себя одну или несколько подгрупп. "
                    "Для удаления необходимо очистить все подгруппы.", log_);
        return;
    }
    else if(!data_->productData.groupExist(deleted_group_name))
    {
        if(group_for_delete->parent()->objectName().isEmpty())
        {
            data_->jsonData.prodInfo[L"groups"]->map().erase(deleted_group_name.toStdWString());
        }
        else
        {
            data_->jsonData.prodInfo.search(group_for_delete->parent()->objectName().toStdWString())->second->map().
                    erase(data_->jsonData.prodInfo.search(deleted_group_name.toStdWString()));
        }

        data_->jsonData.prodInfo.fwrite(PROD_JSON);
        fillModel();
        return;
    }
    else
    {
        QString new_group_name = deleted_group_name;
        std::vector<int> deleted_codes;
        auto* dlg_del = new form_dialog::deleteGroup_dlg(data_, &new_group_name, this);


        switch (dlg_del->exec())
        {
            case QDialog::Accepted:

                data_->productData.changeGroup(deleted_group_name, new_group_name);

                if(group_for_delete->parent()->objectName().isEmpty())
                {
                    data_->jsonData.prodInfo[L"groups"]->map().erase(deleted_group_name.toStdWString());
                }
                else
                {
                    data_->jsonData.prodInfo.search(group_for_delete->parent()->objectName().toStdWString())->second->map().
                            erase(data_->jsonData.prodInfo.search(deleted_group_name.toStdWString()));
                }

                fillModel();
                fillTable(new_group_name);
                data_->jsonData.prodInfo.fwrite(PROD_JSON);
                utl::logger("Группа " + deleted_group_name + " удалена. Товары перемещены в группу " + new_group_name + ".", log_);
                break;

            case 3:
                if(group_for_delete->parent()->objectName().isEmpty())
                {
                    data_->jsonData.prodInfo[L"groups"]->map().erase(deleted_group_name.toStdWString());
                }
                else
                {
                    data_->jsonData.prodInfo.search(group_for_delete->parent()->objectName().toStdWString())->second->map().
                            erase(data_->jsonData.prodInfo.search(deleted_group_name.toStdWString()));
                }

                deleted_codes = data_->productData.deleteProducts(deleted_group_name);
                for(auto&& code : deleted_codes)
                {
                    data_->jsonData.prodInfo[L"backup_codes"]->vector(tag::vecInt()).push_back(code);
                }

                fillModel();
                tables_.erase(tables_.find(deleted_group_name));
                data_->jsonData.prodInfo.fwrite(PROD_JSON);

                utl::logger("Группа " + deleted_group_name + " удалена вместе с входящими в нее товарами.", log_);

                break;

            case QDialog::Rejected:

                break;

        }

        dlg_del->deleteLater();
    }
}

bool form::ProductTables::eventFilter(QObject *watcher, QEvent *event)
{
    if(watcher == ui_->groups_tree->viewport() && event->type() == QEvent::MouseButtonRelease)
    {
        auto* me = dynamic_cast<QMouseEvent*>(event);
        QModelIndex index = ui_->groups_tree->indexAt(me->pos());

        if(!index.isValid())
        {
            ui_->groups_tree->clearSelection();
            ui_->groups_tree->clearFocus();
            this->setFocus();
        }
        else
        {
            ui_->groups_tree->clicked(ui_->groups_tree->currentIndex());
            ui_->table_view_wgt->clearFocus();
        }
        return true;
    }
    else if((watcher == ui_->table_view_wgt || watcher == this) && event->type() == QEvent::MouseButtonRelease)
    {
        ui_->groups_tree->clearSelection();
        ui_->groups_tree->clearFocus();
    }
    else if(event->type() == QEvent::KeyPress)
    {
        std::wcout << 1 << std::endl;
        auto* ke = dynamic_cast<QKeyEvent*>(event);
        keyPressEvent(ke);
    }

    return QWidget::eventFilter(watcher, event);
}

void form::ProductTables::fillTable(const QString& group)
{
    if(tables_.find(group) == tables_.end())
    {
        tables_.insert(group, new QTableWidget(ui_->table_view_wgt));
        tables_[group]->setObjectName(group);
        tables_[group]->setVisible(false);
        tables_[group]->setColumnCount(4);
        tables_[group]->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        tables_[group]->setHorizontalHeaderLabels({"Наименование", "Код товара", "Шт/Кг", "Цена"});
        tables_[group]->verticalHeader()->hide();

        QObject::connect(tables_[group], &QTableWidget::cellDoubleClicked, this, &ProductTables::openProdCard);
    }

    std::vector<database::productProperties> products{data_->productData.getProducts(group)};
    tables_[group]->setRowCount(static_cast<int>(products.size()));
    int row{0};

    for(auto&& product : products)
    {
        tables_[group]->setItem(row, 0, new QTableWidgetItem(product.name));
        tables_[group]->item(row, 0)->setFlags(tables_[group]->item
                (row, 0)->flags() ^ Qt::ItemIsEditable);

        tables_[group]->setItem(row, 1, new QTableWidgetItem(utl::toStrCode(product.code)));
        tables_[group]->item(row, 1)->setFlags(tables_[group]->item
                (row, 1)->flags() ^ Qt::ItemIsEditable);

        tables_[group]->setItem(row, 2, new QTableWidgetItem(QString::number(product.count)));
        tables_[group]->item(row, 2)->setFlags(tables_[group]->item
                (row, 2)->flags() ^ Qt::ItemIsEditable);

        tables_[group]->setItem(row, 3, new QTableWidgetItem(QString::number(product.trade_price)));
        tables_[group]->item(row, 3)->setFlags(tables_[group]->item
                (row, 3)->flags() ^ Qt::ItemIsEditable);
        ++row;
    }
}

void form::ProductTables::keyPressEvent(QKeyEvent* event)
{
    this->setFocus();

    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        findProductTableRow();
        searchWord_.clear();
    }
    else
    {
        searchWord_ += event->text();
    }
}

void form::ProductTables::findProductTableRow()
{
    if(searchWord_.size() == 7)
    {
        for(auto&& table : tables_)
        {
            QList<QTableWidgetItem*> tableItems = table->findItems(searchWord_, Qt::MatchEndsWith);

            if(!tableItems.isEmpty())
            {
                QString key{tableItems.first()->tableWidget()->objectName()};

                if(!ui_->table_view_wgt->layout()->isEmpty())
                {
                    auto widget = ui_->table_view_wgt->layout()->takeAt(0)->widget();
                    widget->setVisible(false);
                    ui_->table_view_wgt->layout()->removeWidget(widget);
                }

                if(tables_.find(key) == tables_.end())
                {
                    return;
                }

                ui_->table_view_wgt->layout()->addWidget(tables_[key]);
                tables_[key]->setVisible(true);

                tables_[key]->selectRow(tableItems.first()->row());
            }
        }
    }
    else
    {
        int code = data_->productData.getProductCode(searchWord_);

        for(auto&& table : tables_)
        {
            QList<QTableWidgetItem*> tableItems = table->findItems(utl::toStrCode(code), Qt::MatchEndsWith);

            if(!tableItems.isEmpty())
            {
                QString key{tableItems.first()->tableWidget()->objectName()};

                if(!ui_->table_view_wgt->layout()->isEmpty())
                {
                    auto widget = ui_->table_view_wgt->layout()->takeAt(0)->widget();
                    widget->setVisible(false);
                    ui_->table_view_wgt->layout()->removeWidget(widget);
                }

                if(tables_.find(key) == tables_.end())
                {
                    return;
                }

                ui_->table_view_wgt->layout()->addWidget(tables_[key]);
                tables_[key]->setVisible(true);

                tables_[key]->selectRow(tableItems.first()->row());
            }
        }
    }
}

void form::ProductTables::findProductDlg()
{
    auto *dlg = new form_dialog::searchProduct_dlg(this);

    switch (dlg->exec())
    {
        case QDialog::Accepted:
            searchWord_ = dlg->getInput();
            findProductTableRow();
            break;

        case QDialog::Rejected:
            break;

    }
}

void form::makeProductTables(implData *data, QTextBrowser *log, QMdiArea* mdiArea)
{
    auto* subWindow = new QMdiSubWindow(mdiArea);
    auto* prodTables = new form::ProductTables(data, log, mdiArea, subWindow);

    subWindow->setWidget(prodTables);
    subWindow->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(subWindow);

    subWindow->showMaximized();

    QObject::connect(prodTables, &QWidget::destroyed, [subWindow, mdiArea]()
    {
        subWindow->disconnect();
        mdiArea->removeSubWindow(subWindow);
        subWindow->deleteLater();
    });
}
