//
// Created by Maxim on 17.11.2022.
//
#ifndef SHOP_ASSISTANT_MODALDIALOGSTRING_H
#define SHOP_ASSISTANT_MODALDIALOGSTRING_H

#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>
#include <QLayout>

namespace form {
    class dialWindow_str : public QDialog {
    Q_OBJECT

    public:
        dialWindow_str(QWidget *parent = nullptr);

        ~dialWindow_str() override;

        QString getInput() const;

    private:
        QLineEdit *text;
        QPushButton *accept_btn;
        QPushButton *decline_btn;

    private:
        QHBoxLayout *main_l;
    };
}

#endif //SHOP_ASSISTANT_MODALDIALOGSTRING_H
