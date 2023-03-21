//
// Created by Maxim on 22.07.2022.
//

#ifndef SHOP_ASSISTANT_DATA_H
#define SHOP_ASSISTANT_DATA_H

#include <QTextBrowser>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDialog>
#include <QFile>
#include <QDate>
#include <io.h>
#include <map>
#include <vector>
#include <memory>
#include "models/objectTree_model.h"
#include "json.hpp"

namespace database
{
    struct jsonData
    {
        jsonData();
        ~jsonData();

        json prodInfo;

    private:
        void prodInfoUpload(const std::string& file_path);
    };

    struct productProperties
    {
        int     code{};
        QString group{};
        QString provider{};
        QString name{};
        QString print_name{};
        QString barcode{};
        QString qrcode{};
        double  count{};
        int     exp_date{};
        double  retail_price{};
        double  trade_price{};
        bool    mark{};

        productProperties& operator=(const productProperties& oth) noexcept;
    };
    class productData final
    {
    public:
        productData();
        ~productData();
        std::wstring lastErrorOfQuery();
        std::wstring lastErrorOfDatabase();
        int deleteProduct(const int& code);
        std::vector<int> deleteProducts(const QString& group);
        void insertProduct(const productProperties& s);
        void changeGroup(const QString& old_group, const QString& new_group);
        productProperties getProduct(const int& code);
        productProperties getProduct(const QString& bar_qr_code);
        std::vector<productProperties> getProducts(const QString& group);
        bool productExist(const int& code);
        bool groupExist(const QString& group);
        int rowsCount();
        QString getProductName(const int& code);
        void saveDM(const std::vector<QString>& vec, const int& code);
        QVector<QString> getDM(const int& code);
        void deleteDM(const QString& dm);
        int getProductCode(const QString& bar_qr_code);

    private:
        QSqlDatabase prod_db_;
        QSqlQuery query_;
    };
}
#endif //SHOP_ASSISTANT_DATA_H
