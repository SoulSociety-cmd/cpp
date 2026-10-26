#ifndef GIAO_DIEN_H
#define GIAO_DIEN_H

#include "app.h"
#include <vector>

void clearScreen();
void setConsoleColor(int fg, bool bold = false);
void resetConsoleColor();
std::string trim(const std::string& s);
std::string toUpper(std::string s);
void pauseScreen();
void drawBox(const std::string& title, const std::vector<std::string>& lines, int w = 56);
void drawHeader(const std::string& title);
int getKey();
void setHighlight(bool on);
void markOrigin();
void gotoRow(int row);
void showCursor(bool show);
void drawItem(const std::string& text, bool selected, int w);
int selectMenu(const std::string& title, const std::vector<std::string>& items,
    const std::vector<std::string>& info = std::vector<std::string>(), int w = 56);
std::string inputStr(const std::string& label, size_t maxLen = 0, bool allowEmpty = false);
int inputInt(const std::string& label, int lo, int hi);
bool confirm(const std::string& q, const std::vector<std::string>& info = std::vector<std::string>());
std::string colL(std::string s, int w);
std::string colR(std::string s, int w);
std::string fmtMoney(double v);
bool laNamNhuan(int y);
int soNgayTrongThang(int m, int y);
bool parseDate(const std::string& s, int& d, int& m, int& y);
long dateKey(const std::string& s);
std::string inputDate(const std::string& label);
double inputDouble(const std::string& label, double lo, double hi, bool loExclusive);
void showTable(const std::string& title, const std::vector<std::string>& head,
    const std::vector<std::string>& rows, const std::vector<std::string>& foot,
    int w = 72, int pageSize = 15);

#endif
