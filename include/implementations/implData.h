//
// Created by Maxim on 22.07.2022.
//

#ifndef SHOP_ASSISTANT_IMPLDATA_H
#define SHOP_ASSISTANT_IMPLDATA_H

#include "data/Data.h"

struct implData
{
    dataContain::prodContainer prodContain;
    dataContain::prodInfo prodInfo;
    dataContain::addProd_doc addProdDoc;
    dataContain::docInfo docInfo;
    std::map<std::wstring, dataContain::docContainer*> docsPtr;
    std::map<std::wstring, std::string> docStatusIcons{{L"Записан", "../../icons/writeDocIcon.png"},
                                                       {L"Проведён","../../icons/enterDocIcon.png"},
                                                       {L"Удалён", "../../icons/delDocIcon.png"}};
};
#endif //SHOP_ASSISTANT_IMPLDATA_H
