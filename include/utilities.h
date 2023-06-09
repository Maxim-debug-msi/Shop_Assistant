//
// Created by Maxim on 17.11.2022.
//

#ifndef SHOP_ASSISTANT_UTILITIES_H
#define SHOP_ASSISTANT_UTILITIES_H

#include <QString>
#include <QObject>
#include <QTextBrowser>
#include <QtPrintSupport>
#include <string>
#include <ctime>
#include "impls/implData.h"
#include "BarcodeFormat.h"
#include "MultiFormatWriter.h"
#include "Matrix.h"
#include "BitMatrix.h"
#include "TextUtfEncoding.h"
#include "stb_image_write.h"

namespace utl
{

    std::wstring w_asctime(std::tm *);

    void logger(const QString &message, QTextBrowser* log);

    QStringList getAvailablePrinters();

    QStringList getProductGroupsList(implData*);

    void getProductGroupsTree(implData*, QObject*);

    void barcodeGen(const std::wstring& codeIn);

    void inc_productCode(std::wstring& prod_code);

    QString toStrCode(const int& code);
}

#endif //SHOP_ASSISTANT_UTILITIES_H
