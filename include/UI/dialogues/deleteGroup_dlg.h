//
// Created by Maxim on 21.11.2022.
//

#ifndef SHOP_ASSISTANT_DELETEGROUP_DLG_H
#define SHOP_ASSISTANT_DELETEGROUP_DLG_H

#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>
#include <QLayout>
#include <QComboBox>
#include <QTextBrowser>
#include <QStringList>
#include "json.hpp"
#include "models/objectTree_model.h"
#include "impls/implData.h"

namespace form_dialog {
    class deleteGroup_dlg : public QDialog {
    Q_OBJECT

    public:
        deleteGroup_dlg(implData*, QString*, QWidget *parent = nullptr);

        ~deleteGroup_dlg() override;

    private slots:
        void currentGroupName();

    private:
        QStringList getProductGroups(std::unordered_map<std::wstring, variant*>& map);

    private:
        QLineEdit* d_groupList;
        QComboBox *groupList;
        QPushButton *accept_btn;
        QPushButton* delete_btn;
        QPushButton *decline_btn;

    private:
        QVBoxLayout *main_l;
        QHBoxLayout* groupList_l;
        QHBoxLayout* buttons_l;

    private:
        implData* data_;
        QString* groupForDel_;
    };
}

#endif //SHOP_ASSISTANT_DELETEGROUP_DLG_H
