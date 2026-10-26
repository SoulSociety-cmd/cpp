#ifndef LUU_DOC_FILE_H
#define LUU_DOC_FILE_H

#include "app.h"
#include <string>

bool luuFile(const std::string& fn, treeVT dsvt, const DS_NV& dsnv);
bool docFile(const std::string& fn, treeVT& dsvt, DS_NV& dsnv, std::string& err);
void menuFile(treeVT& dsvt, DS_NV& dsnv);
int demVT(treeVT t);
int demHD(const DS_NV& ds);

#endif
