//
// Created by Maxim on 31.10.2022.
//

#ifndef SHOP_ASSISTANT_PROD_CARD_H
#define SHOP_ASSISTANT_PROD_CARD_H

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
#include <QMdiArea>
#include <QTableWidget>
#include <QMdiSubWindow>
#include "UI/MainWindow.h"
#include <ctime>
#include "implementations/implData.h"
#include "data/Data.h"

namespace forms
{
class prod_card : public QObject
{
    Q_OBJECT

public:
    prod_card(implData*, QTextBrowser*, QWidget* parent = nullptr, const std::wstring& prodCode = L"");
    ~prod_card() override;

    void setupUI();

signals:
    void productAdded(const std::wstring& group, const std::wstring& code);
    void productDeleted(const std::wstring& group, const std::wstring& code);

private:
    void fillFields();
    void blockEdit();

private slots:
    void saveProduct_s();
    void deleteProduct_s();
    void editProduct_s();

public:
    QWidget* mainWgt;

private:
    QLineEdit* d_group;
    QComboBox* group;

    QLineEdit* d_code;
    QLineEdit* code;

    QLineEdit* d_name;            //d_* - description
    QLineEdit* name;

    QLineEdit* d_printName;
    QLineEdit* printName;

    QLineEdit* d_barCode;
    QLineEdit* barCode;

    QLineEdit* d_oPrice;
    QDoubleSpinBox* oPrice;

    QLineEdit* d_rPrice;
    QDoubleSpinBox* rPrice;

    QLineEdit* d_expDate;
    QSpinBox* expDate;

    QToolButton* actions_btn;
    QMenu* actions;
    QAction* saveProduct;
    QAction* deleteProduct;
    QAction* editProduct;

private:
    QVBoxLayout* vl;  //main
    QHBoxLayout* hl;  //menu
    QHBoxLayout* hl0; //group
    QHBoxLayout* hl1; //code
    QHBoxLayout* hl2; //name
    QHBoxLayout* hl3; //print name
    QHBoxLayout* hl4; //bar code
    QHBoxLayout* hl5; //o_price
    QHBoxLayout* hl6; //r_price
    QHBoxLayout* hl7; //expiration date


private:
    QObject* rootModelObj;
    implData* data;
    models::objectTree_model* oT_model;
    QTextBrowser* log;
    std::wstring* productCode;
    int* formStatus; // 0-edit, 1-enter, 2-delete, 3-new
    bool formEdited{false};

    std::map<std::wstring, int> docStatusNum{{L"edit", 0},
                                             {L"enter", 1},
                                             {L"delete", 2},
                                             {L"new", 3}};
};

void make_productCard(QMdiArea*, implData*, QTextBrowser*, std::map<std::wstring, QTableWidget*>* prodTables = nullptr,
                      const std::wstring& code_ = L"");
}

#endif //SHOP_ASSISTANT_PROD_CARD_H
