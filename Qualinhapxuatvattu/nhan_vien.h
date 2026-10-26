#ifndef NHAN_VIEN_H
#define NHAN_VIEN_H

#include "app.h"

int timNV(const DS_NV& ds, const std::string& ma);
bool chenNV(DS_NV& ds, Nhanvien* nv);
void nhapNhanVien(DS_NV& dsnv);
void inNhanVien(const DS_NV& dsnv);
void giaiPhongNV(DS_NV& ds);

#endif
