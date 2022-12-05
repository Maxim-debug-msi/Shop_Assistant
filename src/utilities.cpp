//
// Created by Maxim on 17.11.2022.
//
#include "utilities.h"

std::wstring utl::w_asctime(std::tm* pTInfo)
{
    std::wstring buf;

    if(pTInfo->tm_mday < 10)
    {
        buf += L"0";
    }

    buf += std::to_wstring(pTInfo->tm_mday);
    buf += L".";

    if(pTInfo->tm_mon < 9)
    {
        buf += L"0";
    }

    buf += std::to_wstring(pTInfo->tm_mon + 1);
    buf += L".";
    buf += std::to_wstring(1900 + pTInfo->tm_year);
    buf += L" ";

    if(pTInfo->tm_hour < 10)
    {
        buf += L"0";
    }
    buf += std::to_wstring(pTInfo->tm_hour);
    buf += L":";

    if(pTInfo->tm_min < 10)
    {
        buf += L"0";
    }
    buf += std::to_wstring(pTInfo->tm_min);
    buf += L":";

    if(pTInfo->tm_sec < 10)
    {
        buf += L"0";
    }
    buf += std::to_wstring(pTInfo->tm_sec);

    return buf;
}

void utl::logger(const QString &message, QTextBrowser *log)
{
    char buffer[25];
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    const char* format = "%d.%m.%Y %HH:%MM:%SS";
    strftime(buffer, 25, format, now);

    log->setMarkdown(log->toMarkdown() + "[" + buffer + "]" + " " + message);
}

QStringList utl::getAvailablePrinters()
{
    QStringList availablePrinters;

    for(auto&& printer : QPrinterInfo::availablePrinters())
    {
        availablePrinters << printer.printerName();
    }

    return availablePrinters;
}

void utl::barcodeGen(const std::wstring& codeIn)
{
    std::string code(codeIn.begin(), codeIn.end());
    int margin{1};
    int eccLevel{1};
    ZXing::BarcodeFormat format = ZXing::BarcodeFormat::Code128;
    ZXing::MultiFormatWriter writer = ZXing::MultiFormatWriter(format).setMargin(margin).setEccLevel(eccLevel);
    ZXing::Matrix<uint8_t> bitmap;
    bitmap = ZXing::ToMatrix<uint8_t>(writer.encode(code, 340, 30));
    std::string filepath = "../../data/barcodes/" + code + ".png";
    stbi_write_png(filepath.c_str(), bitmap.width(), bitmap.height(), 1, bitmap.data(), 0);
}
