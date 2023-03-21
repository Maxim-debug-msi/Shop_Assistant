//
// Created by Maxim on 25.09.2022.
//

#ifndef SHOP_ASSISTANT_DOCJOURNAL_H
#define SHOP_ASSISTANT_DOCJOURNAL_H

#include <QObject>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QSpinBox>
#include <QTextBrowser>
#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <map>
#include "impls/implData.h"
#include "data/data.h"

namespace form {
    class docJournal : QObject {
    Q_OBJECT

    public:
        docJournal(QWidget* parent = nullptr);

        ~docJournal() override;

        void setupUI();

        void setData_ptr(implData *);

        void setLog_ptr(QTextBrowser *);

        void setMainWindow_ptr(QMdiArea*);

    public slots:
        void drawTable();

        void openDoc(const std::wstring&, int&);

    private:
        implData* getData_ptr();

    public:
        QWidget *mainWgt;

    private:
        QTableWidget* table;
        QWidget* tableWgt;

        QLineEdit *summaryDL;
        QComboBox *docList;

        QLineEdit* summaryCreator;
        QComboBox* creator;
        QCheckBox* onOffCreator_btn;

        QPushButton *search_btn;

    private:
        QHBoxLayout *hl1;
        QHBoxLayout *hl11;

        QHBoxLayout* hl2;
        QHBoxLayout* hl21;

        QHBoxLayout *hl3;

        QHBoxLayout* hl4;

        QVBoxLayout *vl;

    private:
        QMdiArea* mainWindow;
        QVector<QObject*> obj_list;
        implData *data;
        models::objectTree_model *oT_model;
        QTextBrowser *log;
    };
}
#endif //SHOP_ASSISTANT_DOCJOURNAL_H
