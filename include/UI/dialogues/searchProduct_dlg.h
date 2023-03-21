//
// Created by Maxim on 02.03.2023.
//

#ifndef SHOP_ASSISTANT_SEARCHPRODUCT_DLG_H
#define SHOP_ASSISTANT_SEARCHPRODUCT_DLG_H

#endif //SHOP_ASSISTANT_SEARCHPRODUCT_DLG_H

#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>
#include <QLayout>

namespace form_dialog{
    class searchProduct_dlg final : public QDialog{
    //Q_OBJECT

    public:
        explicit searchProduct_dlg(QWidget* parent = nullptr);
        ~searchProduct_dlg() override;
        QString getInput() const;

    private:
        QLineEdit *text;
        QPushButton *accept_btn;
        QPushButton *decline_btn;

    private:
        QHBoxLayout *main_l;
    };
}
