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
};
#endif //SHOP_ASSISTANT_IMPLDATA_H
