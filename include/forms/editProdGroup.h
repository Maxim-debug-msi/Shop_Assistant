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
#include "implementations/implData.h"
#include "data/Data.h"
#include "forms/dialWindow_str.h"
#include "forms/dialWindow_del.h"
#include "utilities.h"


namespace form
{

    class editProdGroup : public QObject
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

    public:
        QWidget *mainWgt;

    private:
        QObject* rootModelObj;
        QTreeView *viewModel;
        QTextBrowser* log;
        QString objName;
        models::objectTree_model *oT_model;
        implData *data;
    private:
        QPushButton* remElem_btn;
        QPushButton* addElem_btn;

    private:
        QVBoxLayout* main_l;
        QHBoxLayout* buttons_l;
        QGridLayout* model_l;
    };

    void create_editProdGroup(QMdiArea*, implData*, QTextBrowser* log);
}
#endif //SHOP_ASSISTANT_EDITPRODGROUP_H
