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
    struct docContainer
    {
        fileContainer docs;
        std::unordered_map<std::wstring, std::vector<time_t>> docsOfCreator;
        std::vector<time_t> docsOfDate;
    };

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
        void upload(const std::string&, std::map<std::wstring, dataContain::docContainer*>&);
        void save(const std::string&);

    public:
        docContainer doc;
    };

    struct docInfo
    {
        void upload(const std::string&);
        void save(const std::string&);

    public:
        fileContainer docInfo;
    };

    std::wstring w_asctime(std::tm*);
}
#endif //SHOP_ASSISTANT_DATA_H
