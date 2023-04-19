//
// Created by Maxim on 07.04.2023.
//

#ifndef SHOP_ASSISTANT_IMPLDEVICES_H
#define SHOP_ASSISTANT_IMPLDEVICES_H

#include "BarcodeScanner.h"

struct implDevices
{
    device::BarcodeScanner scanner;

    explicit implDevices(QWidget* parent) : scanner(parent){};
};

#endif //SHOP_ASSISTANT_IMPLDEVICES_H
