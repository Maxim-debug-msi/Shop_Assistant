//
// Created by Maxim on 17.11.2022.
//

#ifndef SHOP_ASSISTANT_UTILITIES_H
#define SHOP_ASSISTANT_UTILITIES_H

#include <QString>
#include <QTextBrowser>
#include <QtPrintSupport>
#include <string>
#include <ctime>
#include "BarcodeFormat.h"
#include "MultiFormatWriter.h"
#include "Matrix.h"
#include "BitMatrix.h"
#include "TextUtfEncoding.h"
#include "stb_image_write.h"

namespace utl
{

    std::wstring w_asctime(std::tm *);

    void logger(const QString &message, QTextBrowser *);

    QStringList getAvailablePrinters();

    void barcodeGen(const std::wstring& codeIn);
}

#endif //SHOP_ASSISTANT_UTILITIES_H
