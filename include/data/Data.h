//
// Created by Maxim on 22.07.2022.
//

#ifndef SHOP_ASSISTANT_DATA_H
#define SHOP_ASSISTANT_DATA_H

#include <map>
#include <vector>
#include <memory>
#include "models/objectTree_model.h"
#include "ioWCharTDocs.hpp"

namespace dataContain
{
    struct prod_data
    {
        void upload(const std::string&);
        void save(const std::string&);

    public:
        fileContainer products;
    };

    struct prodTree_model
    {
        void upload(const std::string&);
        void save(const std::string&);

    public:
        fileContainer groupMap_;
    };
}
#endif //SHOP_ASSISTANT_DATA_H
