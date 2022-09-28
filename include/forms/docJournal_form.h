//
// Created by Maxim on 25.09.2022.
//

#ifndef SHOP_ASSISTANT_DOCJOURNAL_FORM_H
#define SHOP_ASSISTANT_DOCJOURNAL_FORM_H

#include <QObject>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QSpinBox>
#include <QTextBrowser>
#include <QTableWidget>
#include <map>
#include "implementations/implData.h"
#include "data/Data.h"

namespace forms {
    class docJournal_form : QObject {
    Q_OBJECT

    public:
        docJournal_form();

        ~docJournal_form() override;

        void setupUI();

        void setData_ptr(implData *);

        void setLog_ptr(QTextBrowser *);

    private slots:

        void drawTable();

    public:
        QWidget *mainWgt;

    private:
        std::map<std::wstring, QTableWidget*> tables;
        QTableWidget *mainTable;

        QLineEdit *summaryDL;
        QComboBox *docList;

        QPushButton *search_btn;

    private:
        QHBoxLayout *hl1;
        QHBoxLayout *hl11;

        QHBoxLayout *hl2;

        QVBoxLayout *vl;

    private:
        QVector<QObject*> obj_list;
        implData *data;
        models::objectTree_model *oT_model;
        QTextBrowser *log;
        std::wstring *nameBuf;
    };
}
#endif //SHOP_ASSISTANT_DOCJOURNAL_FORM_H
