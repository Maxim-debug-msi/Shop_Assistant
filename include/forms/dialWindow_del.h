//
// Created by Maxim on 21.11.2022.
//

#ifndef SHOP_ASSISTANT_DIALWINDOW_DEL_H
#define SHOP_ASSISTANT_DIALWINDOW_DEL_H

#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>
#include <QLayout>
#include <QComboBox>
#include <QTextBrowser>
#include "ioWCharTDocs.hpp"
#include "models/objectTree_model.h"
#include "implementations/implData.h"

namespace form {
    class dialWindow_del : public QDialog {
    Q_OBJECT

    public:
        dialWindow_del(implData*, QString*, QWidget *parent = nullptr);

        ~dialWindow_del() override;

    private slots:
        void currentGroupName();

    signals:
        void deleting();

    private:
        void extractor(std::unordered_map<std::wstring, variant*>& map);

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
        models::objectTree_model* oT_model;
        QObject* rootModelObj;
        implData* data;
        QString* groupForDel;
    };
}

#endif //SHOP_ASSISTANT_DIALWINDOW_DEL_H
