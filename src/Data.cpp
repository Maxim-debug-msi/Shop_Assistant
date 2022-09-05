//
// Created by Maxim on 22.07.2022.
//

#include <functional>
#include "data/Data.h"

void dataContain::prod_data::upload(const std::string & filePath)
{
    products.fread(filePath);
}

void dataContain::prod_data::save(const std::string & filePath)
{
    products.fwrite(filePath);
}

void dataContain::prodTree_model::upload(const std::string& filePath)
{
    groupMap_.fread(filePath);
}

void dataContain::prodTree_model::save(const std::string & filePath)
{
    groupMap_.fwrite(filePath);
}


