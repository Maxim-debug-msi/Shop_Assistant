//
// Created by Maxim on 11.08.2022.
//

#ifndef SHOP_ASSISTANT_EDITPRODGROUP_H
#define SHOP_ASSISTANT_EDITPRODGROUP_H

#include <QObject>
#include <QTreeView>
#include <QPushButton>
#include <QLayout>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QMouseEvent>
#include <unordered_map>
#include "impls/implData.h"
#include "data/data.h"
//#include "forms/add_group_dialog.h"
//#include "forms/delete_group_dialog.h"
#include "utilities.h"


namespace form
{

    class editProdGroup : public QWidget
    {
    Q_OBJECT

    public:
        editProdGroup(implData*, QTextBrowser*, QWidget* parent = nullptr);

        ~editProdGroup() override;

        void setupUI();

    private:
        void groupExtractor(std::unordered_map<std::wstring, variant*>&, QObject*);
        void refreshModel();
        void saveChanges(const std::string&);

    private slots:
        void addNewGroup();
        void removeSelectedGroup();

    protected:
        bool eventFilter(QObject* watcher, QEvent* event);

    private:
        QObject* root_model_obj;
        QTreeView *view_model;
        QTextBrowser* log;
        QString object_name;
        models::objectTree_model *obj_tree_model;
        implData *data;
    private:
        QPushButton* remove_elem_btn;
        QPushButton* add_elem_btn;

    private:
        QVBoxLayout* main_l;
        QHBoxLayout* buttons_l;
        QGridLayout* model_l;
    };

    void create_editProdGroup(QMdiArea*, implData*, QTextBrowser* log);
}
#endif //SHOP_ASSISTANT_EDITPRODGROUP_H
