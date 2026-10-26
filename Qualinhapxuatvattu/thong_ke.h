#ifndef THONG_KE_H
#define THONG_KE_H

#include "app.h"
#include <string>

bool nhapKhoang(std::string& tu, std::string& den, long& k1, long& k2);
void thongKeHoaDon(const DS_NV& dsnv);
void top10VatTu(treeVT dsvt, const DS_NV& dsnv);
void doanhThuNam(const DS_NV& dsnv);

#endif
