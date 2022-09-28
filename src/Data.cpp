//
// Created by Maxim on 22.07.2022.
//

#include <functional>
#include "data/Data.h"

void dataContain::prodContainer::upload(const std::string & filePath)
{
    products.fread(filePath);
}

void dataContain::prodContainer::save(const std::string & filePath)
{
    products.fwrite(filePath);
}

void dataContain::prodInfo::upload(const std::string& filePath)
{
    prodInfo.fread(filePath);
}

void dataContain::prodInfo::save(const std::string& filePath)
{
    prodInfo.fwrite(filePath);
}

void dataContain::addProd_doc::upload(const std::string& filePath)
{
    docs.fread(filePath);

    if(docs.find(L"Документы") == docs.end())
    {
        tag::mapWStrVar mws;
        docs.get().insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Документы", std::make_unique<variant>(mws)));
    }
    if(docs.find(L"Номер") == docs.end())
    {
        tag::wString ws;
        docs.get().insert(std::pair<std::wstring, std::unique_ptr<variant>>(L"Номер", std::make_unique<variant>(ws)));
        docs[L"Номер"]->set_wstring(L"000001");
    }
    docsRefs.insert({L"Заведение товара", docs.get()});
}

void dataContain::addProd_doc::save(const std::string& filePath)
{
    docs.fwrite(filePath);
}

std::wstring dataContain::w_asctime(std::tm* pTInfo)
{
    std::wstring buf;

    buf += std::to_wstring(pTInfo->tm_mday);
    buf += L".";

    if(pTInfo->tm_mon < 10)
    {
        buf += L"0";
    }

    buf += std::to_wstring(pTInfo->tm_mon + 1);
    buf += L".";
    buf += std::to_wstring(1900 + pTInfo->tm_year);
    buf += L" ";

    buf += std::to_wstring(pTInfo->tm_hour);
    buf += L":";
    buf += std::to_wstring(pTInfo->tm_min);
    buf += L":";
    buf += std::to_wstring(pTInfo->tm_sec);

    return buf;
}

void dataContain::docInfo::upload(const std::string& filePath)
{
    docInfo.fread(filePath);
}

void dataContain::docInfo::save(const std::string& filePath)
{
    docInfo.fwrite(filePath);
}
