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
    std::map<std::wstring, std::string> docStatusIcons{{L"Записан", "C:/Users/Maxim/CLionProjects/shop_assistant/icons/writeDocIcon.png"},{L"Проведён", "C:/Users/Maxim/CLionProjects/shop_assistant/icons/enterDocIcon.png"},
                                                   {L"Удалён", "C:/Users/Maxim/CLionProjects/shop_assistant/icons/delDocIcon.png"}};
};
#endif //SHOP_ASSISTANT_IMPLDATA_H
