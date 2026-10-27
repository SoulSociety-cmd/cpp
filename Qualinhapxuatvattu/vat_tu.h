#ifndef VAT_TU_H
#define VAT_TU_H

#include "app.h"
#include <vector>

treeVT timVT(treeVT t, const std::string& ma);
bool themVT(treeVT& t, const Vattu& v);
bool xoaVT(treeVT& t, const std::string& ma);
void duyetLNR(treeVT t, std::vector<Vattu>& ds);
void giaiPhongCay(treeVT& t);
bool vattuDaDuocDung(const DS_NV& ds, const std::string& ma);
void themVatTu(treeVT& dsvt);
void xoaVatTu(treeVT& dsvt, const DS_NV& dsnv);
void suaVatTu(treeVT& dsvt, DS_NV& dsnv);
void menuNhapVatTu(treeVT& dsvt, DS_NV& dsnv);
void inVatTuTon(treeVT dsvt);

#endif
