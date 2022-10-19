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
#include <QCloseEvent>
#include <ctime>
#include "implementations/implData.h"
#include "data/Data.h"

namespace forms {
    class addProduct_form : public QObject {
    Q_OBJECT

    public:
        addProduct_form(const std::wstring& docNum = L"", QWidget* parent = nullptr);

        ~addProduct_form() override;

        void setupUI();

        void setDataPtr(implData*);

        void setLog_ptr(QTextBrowser*);

    private:
        void saveDoc();

        void blockFieldsEdit(const bool&);

        void blockEdit();

        void fillFields(const std::wstring& docNumber = L"");

    private slots:
        void editedForm();

        void uneditedForm();

        void addProduct();

    public:
        QWidget* mainWgt;

    private:
        std::time_t* time = new std::time_t(std::time(nullptr));
        std::tm* pTInfo = std::localtime(time);

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

        QDoubleSpinBox* optPrice;
        QLineEdit* summaryPrice;

        QSpinBox *expirationDate;
        QLineEdit* summaryDate;

        QLineEdit* summaryEndDate;
        QDateEdit* endDate;

        QLineEdit* summaryComment;
        QLineEdit* comment;

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
        QHBoxLayout* hl14;

    private:
        QVector<QObject*> modelWidgets;
        implData* data;
        models::objectTree_model* oT_model;
        QTextBrowser* log;
        std::wstring* nameBuf;
        std::wstring* docNumber;
        bool isHasEdit;

        std::map<std::wstring, int> docStates{{L"Записан", 0},
                                              {L"Проведён", 1},
                                              {L"Удалён", 2}};
    };
}
#endif //SHOP_ASSISTANT_ADDPROD_FORM_H
