//
// Created by Maxim on 05.09.2022.
//

#ifndef SHOP_ASSISTANT_ADDPROD_FORM_H
#define SHOP_ASSISTANT_ADDPROD_FORM_H

#include <QObject>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QSpinBox>
#include <QTextBrowser>
#include <QToolButton>
#include <QMenu>
#include <ctime>
#include "implementations/implData.h"
#include "data/Data.h"

namespace forms {
    class addProduct_form : QObject {
    Q_OBJECT

    public:
        addProduct_form();

        ~addProduct_form() override;

        void setupUI();

        void setDataPtr(implData*);

        void setLog_ptr(QTextBrowser*);

        void saveDoc();

        void blockEdit(const bool&);

    private slots:
        void editedForm();

        void uneditedForm();

        void addProduct();

    public:
        QWidget *mainWgt;

    private:
        std::time_t time = std::time(nullptr);
        std::tm* const pTInfo = std::localtime(&time);

        QLineEdit* docNum;
        QLineEdit* summaryDocNum;

        QLineEdit* summaryDocDate;
        QLineEdit* docDate;

        QComboBox* groupList;
        QLineEdit* summaryGL;

        QLineEdit* EANCode;
        QLineEdit* summaryEANCode;

        QLineEdit *code;
        QLineEdit* summaryCode;

        QLineEdit *name;
        QLineEdit* summaryName;

        QDoubleSpinBox *count;
        QLineEdit* summaryCount;

        QLineEdit* summaryRPrice;
        QDoubleSpinBox* rPrice;

        QDoubleSpinBox* price;
        QLineEdit* summaryPrice;

        QSpinBox *expirationDate;
        QLineEdit* summaryDate;

        QLineEdit* summaryEndDate;
        QDateEdit* endDate;

        QPushButton* enterProd_btn;
        QPushButton* deleteProd_btn;
        QPushButton* writeProd_btn;
        QToolButton* actions_btn;

        QMenu* menu;
        QAction* remEnter_act;
        QAction* remDel_act;

        QLineEdit* status;
        QLineEdit* creator;

    private:
        QVBoxLayout *vl;
        QHBoxLayout *hl1;
        QHBoxLayout *hl2;
        QHBoxLayout *hl3;
        QHBoxLayout *hl4;
        QHBoxLayout *hl5;
        QHBoxLayout *hl6;
        QHBoxLayout* hl7;
        QHBoxLayout* hl8;
        QHBoxLayout* hl9;
        QHBoxLayout* hl10;
        QHBoxLayout* hl11;
        QHBoxLayout* hl12;
        QHBoxLayout* hl13;

    private:
        QVector<QObject*> obj_list;
        implData* data;
        models::objectTree_model* oT_model;
        QTextBrowser* log;
        std::wstring* nameBuf;
        bool isHasEdit;
    };
}
#endif //SHOP_ASSISTANT_ADDPROD_FORM_H
