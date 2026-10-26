#ifndef APP_H
#define APP_H

#include <cstddef>
#include <string>

const int MAXNV = 500;
extern const std::string DEFAULT_FILE;

struct Vattu {
    std::string MAVT, TENVT, DVT;
    int SOLUONGTON;
};

struct nodeVT {
    Vattu vt;
    nodeVT* left;
    nodeVT* right;
};
typedef nodeVT* treeVT;

struct CT_HOADON {
    std::string MAVT;
    int SOLUONG;
    float DONGIA;
    float VAT;
};

struct nodeCTHD {
    CT_HOADON cthd;
    nodeCTHD* next;
};
typedef nodeCTHD* PTRCTHD;

struct HOADON {
    std::string SOHD;
    std::string NGAYLAP;
    char LOAI;
    PTRCTHD dscthd = NULL;
};

struct nodeHD {
    HOADON hd;
    nodeHD* next;
};
typedef nodeHD* PTRHD;

struct Nhanvien {
    std::string MANV, HO, TEN, PHAI;
    PTRHD dshd = NULL;
};

struct DS_NV {
    Nhanvien* nodes[MAXNV];
    int n = 0;
};

#endif
