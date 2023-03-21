#ifndef DEFS_H
#define DEFS_H

#define DEBUG

#ifdef DEBUG

#define PROD_JSON  "../../data/products/prodInfo.json"
#define PROD_DB3   "../../data/products/products_data.db3"
#define BAR_GEN    "../../data/barcodeGen/barcode.png"
#define TERM_LABEL "../../data/LabelTemplates/ThermalLabelTemplate.tiff"
#define CB_ICO     "../../icons/checkbox.ico"

#endif //DEBUG

#ifdef RELEASE

#define PROD_JSON "../data/products/prodInfo.json"
#define PROD_DB3  "../data/products/products_data.db3"
#define BAR_GEN   "../data/barcodeGen/barcode.png"
#define TERM_LABEL "../data/LabelTemplates/ThermalLabelTemplate.png"
#define CB_ICO     "../icons/checkbox.ico"

#endif //RELEASE

#endif //DEFS_H