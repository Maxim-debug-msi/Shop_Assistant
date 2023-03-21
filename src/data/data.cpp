//
// Created by Maxim on 22.07.2022.
//
#include <functional>
#include "data/data.h"
#include "defs.h"

database::jsonData::jsonData()
{
    prodInfoUpload(PROD_JSON);
}

database::jsonData::~jsonData()
{
    prodInfo.fwrite(PROD_JSON);
}

void database::jsonData::prodInfoUpload(const std::string &file_path)
{
    prodInfo.fread(file_path);

    if(prodInfo.search(L"current_code") == prodInfo.end())
    {
        prodInfo.insert({L"current_code", new variant(1)});
    }
    if(prodInfo.search(L"backup_codes") == prodInfo.end())
    {
        prodInfo.insert({L"backup_codes", new variant(tag::vecInt())});
    }
    if(prodInfo.search(L"groups") == prodInfo.end())
    {
        prodInfo.insert({L"groups", new variant(tag::mapWStrVar())});
    }
}

database::productData::productData() : prod_db_(QSqlDatabase::addDatabase("QSQLITE")), query_(prod_db_)
{
    prod_db_.setDatabaseName(PROD_DB3);
    if(!prod_db_.open())
    {
        QDialog dialog;
        dialog.setModal(true);
        dialog.exec();
    }
    if(!prod_db_.tables().contains("Products"))
    {
        query_.prepare("CREATE TABLE Products(_code INTEGER NOT NULL PRIMARY KEY, _group TEXT, _provider TEXT, "
                      "_name TEXT NOT NULL, _print_name TEXT NOT NULL, _barcode TEXT NOT NULL, _qrcode TEXT NOT NULL,"
                      "_count REAL NOT NULL, _exp_date INTEGER, _retail_price REAL NOT NULL, _trade_price REAL NOT NULL, "
                      "_mark BOOLEAN NOT NULL)");
        if(!query_.exec())
        {
            std::wcout << lastErrorOfQuery() << std::endl;
        }
        query_.clear();
    }
    if(!prod_db_.tables().contains("DataMatrix"))
    {
        query_.prepare("CREATE TABLE DataMatrix(_dm TEXT PRIMARY KEY NOT NULL UNIQUE, _code INTEGER NOT NULL)");
        if(!query_.exec())
        {
            std::wcout << lastErrorOfQuery() << std::endl;
        }
        query_.clear();
    }
}

database::productData::~productData()
{
    prod_db_.close();
}

void database::productData::insertProduct(const productProperties& s)
{
    if(productExist(s.code))
    {
        query_.prepare("UPDATE Products SET _code = :code, _group = :group, _provider = :provider, _name = :name,"
                      " _print_name = :print_name, _barcode = :barcode, _qrcode = :qrcode, _count = :count,"
                      " _exp_date = :exp_date, _retail_price = :retail_price, _trade_price = :trade_price, _mark = :mark"
                      " WHERE _code = :code");
    }
    else
    {
        query_.prepare("INSERT INTO Products(_code, _group, _provider, _name, _print_name, _barcode, _qrcode, _count,"
                      " _exp_date, _retail_price, _trade_price, _mark)"
                      " VALUES (:code, :group, :provider, :name, :print_name, :barcode, :qrcode, :count,"
                      " :exp_date, :retail_price, :trade_price, :mark)");
    }

    query_.bindValue(":code", s.code);
    query_.bindValue(":group", s.group);
    query_.bindValue(":provider", s.provider);
    query_.bindValue(":name", s.name);
    query_.bindValue(":print_name", s.print_name);
    query_.bindValue(":barcode", s.barcode);
    query_.bindValue(":qrcode", s.qrcode);
    query_.bindValue(":count", s.count);
    query_.bindValue(":exp_date", s.exp_date);
    query_.bindValue(":retail_price", s.retail_price);
    query_.bindValue(":trade_price", s.trade_price);
    query_.bindValue(":mark", s.mark);

    if (!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
    }
    query_.clear();
}

std::wstring database::productData::lastErrorOfQuery() {
    return query_.lastError().text().toStdWString();
}

std::wstring database::productData::lastErrorOfDatabase() {
    return prod_db_.lastError().text().toStdWString();
}

int database::productData::deleteProduct(const int &code)
{
    query_.prepare("SELECT _code FROM products WHERE _code = :code");
    query_.bindValue(":code", code);
    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
    }
    query_.next();
    int result{query_.value(0).toInt()};
    query_.clear();

    query_.prepare("DELETE FROM Products WHERE _code = :code");
    query_.bindValue(":code", code);

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
    }
    return result;
}

bool database::productData::productExist(const int &code) {
    query_.prepare("SELECT * FROM Products WHERE _code = :code");
    query_.bindValue(":code", code);

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
        return false;
    }
    else
    {
        return query_.next();
    }
}

database::productProperties database::productData::getProduct(const int &code)
{
    query_.prepare("SELECT _code, _group, _provider, _name, _print_name, _barcode, _qrcode, _count,"
                  "_exp_date, _retail_price, _trade_price, _mark FROM Products WHERE _code = :_code");
    query_.bindValue(":_code", code);

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
        return {};
    }
    else
    {
        QSqlRecord rec{query_.record()};
        query_.next();
        return {.code = query_.value(rec.indexOf("_code")).toInt(),
                .group{query_.value(rec.indexOf("_group")).toString()},
                .provider{query_.value(rec.indexOf("_provider")).toString()},
                .name{query_.value(rec.indexOf("_name")).toString()},
                .print_name{query_.value(rec.indexOf("_print_name")).toString()},
                .barcode{query_.value(rec.indexOf("_barcode")).toString()},
                .qrcode{query_.value(rec.indexOf("_qrcode")).toString()},
                .count = query_.value(rec.indexOf("_count")).toDouble(),
                .exp_date = query_.value(rec.indexOf("_exp_date")).toInt(),
                .retail_price = query_.value(rec.indexOf("_retail_price")).toDouble(),
                .trade_price = query_.value(rec.indexOf("_trade_price")).toDouble(),
                .mark = query_.value(rec.indexOf("_mark")).toBool()
                };
    }
}

database::productProperties database::productData::getProduct(const QString& bar_qr_code)
{
    query_.prepare("SELECT _code, _group, _provider, _name, _print_name, _barcode, _qrcode, _count,"
                   "_exp_date, _retail_price, _trade_price, _mark FROM Products WHERE _barcode = :code OR _qrcode = :qrcode");
    query_.bindValue(":code", bar_qr_code);

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
        return {};
    }
    else
    {
        QSqlRecord rec{query_.record()};
        query_.next();
        return {.code = query_.value(rec.indexOf("_code")).toInt(),
                .group{query_.value(rec.indexOf("_group")).toString()},
                .provider{query_.value(rec.indexOf("_provider")).toString()},
                .name{query_.value(rec.indexOf("_name")).toString()},
                .print_name{query_.value(rec.indexOf("_print_name")).toString()},
                .barcode{query_.value(rec.indexOf("_barcode")).toString()},
                .qrcode{query_.value(rec.indexOf("_qrcode")).toString()},
                .count = query_.value(rec.indexOf("_count")).toDouble(),
                .exp_date = query_.value(rec.indexOf("_exp_date")).toInt(),
                .retail_price = query_.value(rec.indexOf("_retail_price")).toDouble(),
                .trade_price = query_.value(rec.indexOf("_trade_price")).toDouble(),
                .mark = query_.value(rec.indexOf("_mark")).toBool()
        };
    }
}

std::vector<database::productProperties> database::productData::getProducts(const QString &group) {
    query_.prepare("SELECT * FROM Products WHERE _group = :group");
    query_.bindValue(":group", group);

    std::vector<database::productProperties> result;

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
        return result;
    }
    else
    {
        QSqlRecord rec = query_.record();

        while(query_.next())
        {
            result.push_back({.code = query_.value(rec.indexOf("_code")).toInt(),
                                     .group{query_.value(rec.indexOf("_group")).toString()},
                                     .provider{query_.value(rec.indexOf("_provider")).toString()},
                                     .name{query_.value(rec.indexOf("_name")).toString()},
                                     .print_name{query_.value(rec.indexOf("_print_name")).toString()},
                                     .barcode{query_.value(rec.indexOf("_barcode")).toString()},
                                     .qrcode{query_.value(rec.indexOf("_qrcode")).toString()},
                                     .count = query_.value(rec.indexOf("_count")).toDouble(),
                                     .exp_date = query_.value(rec.indexOf("_exp_date")).toInt(),
                                     .retail_price = query_.value(rec.indexOf("_retail_price")).toDouble(),
                                     .trade_price = query_.value(rec.indexOf("_trade_price")).toDouble(),
                                     .mark = query_.value(rec.indexOf("_mark")).toBool()
                                     });
        }
    }
    return result;
}

int database::productData::rowsCount()
{
    query_.clear();
    query_.prepare("SELECT * FROM Products");
    query_.exec();

    return query_.size();
}

bool database::productData::groupExist(const QString &group)
{
    query_.prepare("SELECT * FROM Products WHERE _group = :group");
    query_.bindValue(":group", group);

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
        return false;
    }

    return query_.next();
}

void database::productData::changeGroup(const QString& old_group, const QString &new_group)
{
    query_.prepare("UPDATE Products SET _group = :new_group WHERE _group = :old_group");
    query_.bindValue(":new_group", new_group);
    query_.bindValue(":old_group", old_group);

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
    }
}

std::vector<int> database::productData::deleteProducts(const QString& group)
{
    query_.prepare("SELECT _code FROM Products WHERE _group = :group");
    query_.bindValue(":group", group);

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
        return{};
    }

    std::vector<int> result;

    while(query_.next())
    {
        QSqlRecord rec = query_.record();
        result.push_back(query_.value(rec.indexOf("code")).toInt());
    }
    query_.clear();

    query_.prepare("DELETE FROM Products WHERE _group = :group");
    query_.bindValue(":group", group);

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
        return{};
    }

    return result;
}

QString database::productData::getProductName(const int& code)
{
    query_.prepare("SELECT _name FROM Products WHERE _code = :code");
    query_.bindValue(":code", code);

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
        return {};
    }
    query_.next();

    return query_.value(0).toString();
}

void database::productData::saveDM(const std::vector<QString>& vec, const int& code)
{
    for(auto&& dm : vec)
    {
        query_.prepare("INSERT INTO DataMatrix(_dm, _code) VALUES(:dm, :code)");
        query_.bindValue(":dm", dm);
        query_.bindValue(":code", code);

        if(!query_.exec())
        {
            std::wcout << lastErrorOfQuery() << std::endl;
        }
        query_.clear();
    }
}

int database::productData::getProductCode(const QString &bar_qr_code)
{
    query_.prepare("SELECT _code FROM Products WHERE _barcode = :barcode");
    query_.bindValue(":barcode", bar_qr_code);

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
    }
    else
    {
        return query_.value(0).toInt();
    }

    query_.prepare("SELECT _code FROM Products WHERE _qrcode = :qrcode");
    query_.bindValue(":qrcode", bar_qr_code);

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
    }
    else
    {
        return query_.value(0).toInt();
    }

    return 0;
}

QVector<QString> database::productData::getDM(const int &code)
{
    query_.prepare("SELECT _dm FROM DataMatrix WHERE _code = :code");
    query_.bindValue(":code", code);

    QVector<QString> result;

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
    }
    else
    {
        QSqlRecord rec = query_.record();

        while(query_.next())
        {
            result.push_back(query_.value(rec.indexOf("_dm")).toString());
        }
        query_.clear();
    }
    return result;
}

void database::productData::deleteDM(const QString &dm)
{
    query_.prepare("DELETE FROM DataMatrix WHERE _dm = :dm");
    query_.bindValue(":dm", dm);

    if(!query_.exec())
    {
        std::wcout << lastErrorOfQuery() << std::endl;
    }
}


database::productProperties &database::productProperties::operator=(const database::productProperties &oth) noexcept {
    if(this != &oth)
    {
        code = oth.code;
        group = oth.group;
        provider = oth.provider;
        name = oth.name;
        print_name = oth.print_name;
        barcode = oth.barcode;
        qrcode = oth.qrcode;
        count = oth.count;
        exp_date = oth.exp_date;
        retail_price = oth.retail_price;
        trade_price = oth.trade_price;
        mark = oth.mark;
    }
    return *this;
}
