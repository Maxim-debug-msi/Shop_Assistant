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

    if(prodInfo.find(L"Код") == prodInfo.end())
    {
        tag::wString ws;
        prodInfo.get().insert(std::pair<std::wstring, variant*>(L"Код", new variant(ws)));
        prodInfo[L"Код"]->set_wstring(L"000001");
    }
    if(prodInfo.find(L"РезервныйКод") == prodInfo.end())
    {
        tag::vecWStr vws;
        prodInfo.get().insert(std::pair<std::wstring, variant*>(L"РезервныйКод", new variant(vws)));
    }
    if(prodInfo.find(L"Группы") == prodInfo.end())
    {
        tag::mapWStrVar mwsv;
        prodInfo.get().insert(std::pair<std::wstring, variant*>(L"Группы", new variant(mwsv)));
        prodInfo[L"Группы"]->get_map().insert(std::pair<std::wstring, variant*>(L"Товары", new variant(mwsv)));
    }
}

void dataContain::prodInfo::save(const std::string& filePath)
{
    prodInfo.fwrite(filePath);
}

void dataContain::addProd_doc::upload(const std::string& filePath, std::map<std::wstring, dataContain::docContainer*>& docsPtr)
{
    doc.docs.fread(filePath);

    if(doc.docs.find(L"Документы") == doc.docs.end())
    {
        tag::mapWStrVar mws;
        doc.docs.get().insert(std::pair<std::wstring, variant*>(L"Документы", new variant(mws)));
    }
    if(doc.docs.find(L"Номер") == doc.docs.end())
    {
        tag::wString ws;
        doc.docs.get().insert(std::pair<std::wstring, variant*>(L"Номер", new variant(ws)));
        doc.docs[L"Номер"]->set_wstring(L"000001");
    }

    for(auto&& creator : doc.docs[L"Документы"]->get_map())
    {
        doc.docsOfCreator[creator.first];
        for(auto&& date : creator.second->get_map())
        {
            doc.docsOfCreator[creator.first].push_back(std::stoll(date.first));
            doc.docsOfDate.push_back(std::stoll(date.first));
        }

        std::sort(doc.docsOfCreator[creator.first].begin(), doc.docsOfCreator[creator.first].end(),
                  [](time_t& a, time_t& b)
        {
           return a > b;
        });
    }

    std::sort(doc.docsOfDate.begin(), doc.docsOfDate.end(),
              [](time_t& a, time_t& b)
              {
                  return a > b;
              });

    docsPtr.insert({L"Заведение товара", &doc});

}

void dataContain::addProd_doc::save(const std::string& filePath)
{
    doc.docs.fwrite(filePath);
}

void dataContain::docInfo::upload(const std::string& filePath)
{
    docInfo.fread(filePath);
}

void dataContain::docInfo::save(const std::string& filePath)
{
    docInfo.fwrite(filePath);
}

std::wstring dataContain::w_asctime(std::tm* pTInfo)
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

