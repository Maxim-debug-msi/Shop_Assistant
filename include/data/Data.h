//
// Created by Maxim on 22.07.2022.
//

#ifndef SHOP_ASSISTANT_DATA_H
#define SHOP_ASSISTANT_DATA_H

#include <QTextBrowser>
#include <map>
#include <vector>
#include <memory>
#include "models/objectTree_model.h"
#include "ioWCharTDocs.hpp"

namespace dataContain
{
    struct prodContainer
    {
        void upload(const std::string&);
        void save(const std::string&);

    public:
        fileContainer products;
    };

    struct prodInfo
    {
        void upload(const std::string&);
        void save(const std::string&);

    public:
        fileContainer prodInfo;
    };

    struct addProd_doc
    {
        void upload(const std::string&);
        void save(const std::string&);

    public:
        fileContainer docs;
    };

    struct docInfo
    {
        void upload(const std::string&);
        void save(const std::string&);

    public:
        fileContainer docInfo;
    };

    std::wstring w_asctime(std::tm*);
    std::map<std::wstring, std::unordered_map<std::wstring, std::unique_ptr<variant>>&> docsRefs;
}
#endif //SHOP_ASSISTANT_DATA_H
