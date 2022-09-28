//
// Created by Maxim on 20.07.2022.
//

#ifndef SHOP_ASSISTANT_IMPLFORMS_H
#define SHOP_ASSISTANT_IMPLFORMS_H

#include "forms/product_form.h"
#include "forms/edit_prodGroup_form.h"
#include "forms/receptFromTransit_form.h"
#include "forms/addProd_form.h"
#include "forms/docJournal_form.h"

struct implForms
{
    forms::prodTable_form pT_f;
    forms::prodGroup_editor pG_e;
    forms::receptFromTransit_form rFT_f;
    forms::addProduct_form aP_f;
    forms::docJournal_form dJ_f;
};
#endif //SHOP_ASSISTANT_IMPLFORMS_H
