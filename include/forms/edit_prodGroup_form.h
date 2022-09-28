//
// Created by Maxim on 11.08.2022.
//

#ifndef SHOP_ASSISTANT_EDIT_PRODGROUP_FORM_H
#define SHOP_ASSISTANT_EDIT_PRODGROUP_FORM_H

#include <QObject>
#include <QTreeView>
#include <QPushButton>
#include <QLayout>
#include "implementations/implData.h"
#include "data/Data.h"

namespace forms
{

    class prodGroup_editor : QObject
    {
    Q_OBJECT

    public:
        prodGroup_editor();

        ~prodGroup_editor() override;

        void setDataPtr(implData*);

        void setupUI();

    public:
        QWidget *mainWgt;

    private:
        QTreeView *viewModel;
        models::objectTree_model *oT_model;
        QVector<QObject*> obj_list;
    private:
        QPushButton* remElem_btn;
        QPushButton* addElem_btn;
        QLineEdit* textLine;
        QGridLayout* gl;
        QHBoxLayout* hl;
        QHBoxLayout* hl1;
        QHBoxLayout* hl2;
        QVBoxLayout* vl;

        implData *data;
    };
}
#endif //SHOP_ASSISTANT_EDIT_PRODGROUP_FORM_H
