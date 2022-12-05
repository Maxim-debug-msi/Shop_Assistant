//
// Created by Maxim on 31.10.2022.
//

#ifndef SHOP_ASSISTANT_PRODCARD_H
#define SHOP_ASSISTANT_PRODCARD_H

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
#include <utility>
#include "implementations/implData.h"
#include <ctime>
#include "data/Data.h"
#include "utilities.h"

namespace form
{

class prodCard : public QWidget
{
    Q_OBJECT

public:
    prodCard(implData*, QTextBrowser*, QWidget* parent = nullptr, std::wstring  prodCode = L"",
             std::wstring  prodGroup = L"");

    ~prodCard() override = default;

    void setupUI();

signals:
    void productsChanged(const std::wstring& group);

private:
    void fillFields();
    void blockEdit();
    void saveChanges(const std::string&);

private slots:
    void saveProduct_s();
    void deleteProduct_s();
    void editProduct_s();
    void formEdited_s();
    void formSaved_s();

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
    std::wstring productCode;
    std::wstring productGroup;
    std::wstring oldGroup;
    int formStatus{3}; // 0-edit, 1-enter, 2-delete, 3-new
};

prodCard* make_productCard(QMdiArea*, implData*, QTextBrowser*, const std::wstring& code_ = L"", const std::wstring& group_ = L"");
}

#endif //SHOP_ASSISTANT_PRODCARD_H
