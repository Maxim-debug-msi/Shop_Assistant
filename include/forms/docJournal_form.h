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
#include <QHeaderView>
#include <QCheckBox>
#include <QMdiArea>
#include <QMdiSubWindow>
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

        void setMainWindow_ptr(QMdiArea*);

    public slots:
        void drawTable();

        void openDoc(const std::wstring&);

    public:
        QWidget *mainWgt;

    private:
        std::map<std::wstring, QTableWidget*> tables;
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
        std::map<std::wstring, std::string> iconsPaths{{L"Записан", "C:/Users/Maxim/CLionProjects/shop_assistant/icons/writeDocIcon.png"},
                                                       {L"Проведён", "C:/Users/Maxim/CLionProjects/shop_assistant/icons/enterDocIcon.png"},
                                                       {L"Удалён", "C:/Users/Maxim/CLionProjects/shop_assistant/icons/delDocIcon.png"}};
    };
}
#endif //SHOP_ASSISTANT_DOCJOURNAL_FORM_H
