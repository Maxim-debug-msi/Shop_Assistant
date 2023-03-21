//
// Created by Maxim on 17.11.2022.
//
#include "utilities.h"
#include "defs.h"

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
    //std::string filepath = "../../data/barcodeGen/barcode.png";
    stbi_write_png(BAR_GEN, bitmap.width(), bitmap.height(), 1, bitmap.data(), 0);
}

void utl::inc_productCode(std::wstring& prod_code)
{
    auto it  = prod_code.end() - 1;

    while(true)
    {
        if (*it == L'9') {
            *it = L'0';
            --it;
        } else {
            ++(*it);
            break;
        }
    }
}

QStringList utl::getProductGroupsList(implData* data)
{
    QStringList result;

    std::function<void(std::unordered_map<std::wstring, variant*>&)> extractor =
            [&extractor, &result](std::unordered_map<std::wstring, variant*>& map)
            {
                for(auto&& it : map)
                {
                    result << QString::fromStdWString(it.first);

                    if(it.second != nullptr && it.second->is_map() && !it.second->map().empty())
                    {
                        extractor(it.second->map());
                    }
                }
            };

    extractor(data->jsonData.prodInfo[L"groups"]->map());

    return result;
}

void utl::getProductGroupsTree(implData* data, QObject* root)
{
    std::function<void(std::unordered_map<std::wstring, variant*>&, QObject*)> extractor =
            [&extractor](std::unordered_map<std::wstring, variant*>& map, QObject* parent)
            {
                for(auto&& it : map)
                {
                    auto* obj = new QObject(parent);
                    obj->setObjectName(QString::fromStdWString(it.first));

                    if(it.second != nullptr && it.second->is_map() && !it.second->map().empty())
                    {
                        extractor(it.second->map(), obj);
                    }
                }
            };

    extractor(data->jsonData.prodInfo[L"groups"]->map(), root);
}

QString utl::toStrCode(const int& code)
{
    QString codeStr{QString::number(code)};
    return QString(7 - codeStr.size(), '0') + codeStr;
}
