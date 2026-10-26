#ifndef HOA_DON_H
#define HOA_DON_H

#include "app.h"
#include <vector>

PTRHD timHD(const DS_NV& ds, const std::string& so, int* idx = NULL);
PTRCTHD timCT(PTRCTHD head, const std::string& ma);
void themCuoiCT(PTRCTHD& head, const CT_HOADON& ct);
bool xoaCT(PTRCTHD& head, const std::string& ma, CT_HOADON* out);
double triGiaCT(const CT_HOADON& c);
double triGiaHD(PTRCTHD head);
void capNhatTon(treeVT dsvt, const CT_HOADON& ct, char loai, bool hoan);
std::string tenLoai(char l);
std::vector<std::string> thongTinHD(const Nhanvien* nv, const HOADON& hd, treeVT dsvt);
void themVTVaoHD(treeVT dsvt, HOADON& hd);
void xoaVTDangLap(treeVT dsvt, HOADON& hd);
void huyHoaDon(treeVT dsvt, HOADON& hd);
void lapHoaDon(treeVT& dsvt, DS_NV& dsnv);
void inHoaDon(treeVT dsvt, const DS_NV& dsnv);

#endif
