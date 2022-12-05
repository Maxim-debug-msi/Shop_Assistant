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

    if(prodInfo.search(L"Код") == prodInfo.end())
    {
        tag::wString ws;
        prodInfo.insert(std::pair<std::wstring, variant*>(L"Код", new variant(ws)));
        prodInfo[L"Код"]->set_wstring(L"000001");
    }
    if(prodInfo.search(L"РезервныйКод") == prodInfo.end())
    {
        tag::vecWStr vws;
        prodInfo.insert(std::pair<std::wstring, variant*>(L"РезервныйКод", new variant(vws)));
    }
    if(prodInfo.search(L"Группы") == prodInfo.end())
    {
        tag::mapWStrVar mwsv;
        prodInfo.insert(std::pair<std::wstring, variant*>(L"Группы", new variant(mwsv)));
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

    if(doc.docs.search(L"Документы") == doc.docs.end())
    {
        tag::mapWStrVar mws;
        doc.docs.insert(std::pair<std::wstring, variant*>(L"Документы", new variant(mws)));
    }
    if(doc.docs.search(L"Номер") == doc.docs.end())
    {
        tag::wString ws;
        doc.docs.insert(std::pair<std::wstring, variant*>(L"Номер", new variant(ws)));
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


