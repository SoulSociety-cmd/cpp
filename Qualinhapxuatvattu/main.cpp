#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
#include <bits/stdc++.h>
using namespace std;

const int MAXNV = 500;

/* ===================== KHAI BAO (giu nguyen) ===================== */
struct Vattu {
    string MAVT, TENVT, DVT;
    int SOLUONGTON;
};

struct nodeVT {
    Vattu vt;
    nodeVT *left;
    nodeVT *right;
};
typedef nodeVT *treeVT;

struct CT_HOADON {
    string MAVT;
    int SOLUONG;
    float DONGIA;
    float VAT;
};

struct nodeCTHD {
    CT_HOADON cthd;
    nodeCTHD *next;
};
typedef nodeCTHD *PTRCTHD;

struct HOADON {
    string SOHD;
    string NGAYLAP;
    char LOAI;
    PTRCTHD dscthd = NULL;
};

struct nodeHD {
    HOADON hd;
    nodeHD *next;
};
typedef nodeHD *PTRHD;

struct Nhanvien {
    string MANV, HO, TEN, PHAI;
    PTRHD dshd = NULL;
};

struct DS_NV {
    Nhanvien *nodes[MAXNV];
    int n = 0;
};

/* ===================== TIEN ICH GIAO DIEN ===================== */
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

string toUpper(string s) {
    for (size_t i = 0; i < s.size(); i++) s[i] = toupper((unsigned char)s[i]);
    return s;
}

int getKey();  // khai bao truoc

void pauseScreen() {
    cout << "\n  Nhan phim bat ky de tiep tuc...";
    getKey();
}

// Ve khung: tieu de + danh sach dong
void drawBox(const string &title, const vector<string> &lines, int w = 56) {
    string bar(w, '=');
    cout << "  +" << bar << "+\n";
    int pad = (w - (int)title.size()) / 2;
    if (pad < 0) pad = 0;
    cout << "  |" << string(pad, ' ') << title
         << string(max(0, w - pad - (int)title.size()), ' ') << "|\n";
    cout << "  +" << bar << "+\n";
    for (size_t i = 0; i < lines.size(); i++) {
        string l = " " + lines[i];
        if ((int)l.size() > w) l = l.substr(0, w);
        cout << "  |" << l << string(w - l.size(), ' ') << "|\n";
    }
    cout << "  +" << bar << "+\n";
}

void drawHeader(const string &title) {
    clearScreen();
    drawBox(title, vector<string>());
    cout << "\n";
}

/* ===================== DIEU KHIEN BANG PHIM MUI TEN ===================== */
enum { K_UP = 1, K_DOWN, K_ENTER, K_ESC, K_OTHER };

int getKey() {
#ifdef _WIN32
    int c = _getch();
    if (c == 0 || c == 224) {
        int d = _getch();
        if (d == 72) return K_UP;
        if (d == 80) return K_DOWN;
        return K_OTHER;
    }
    if (c == 13) return K_ENTER;
    if (c == 27) return K_ESC;
    return K_OTHER;
#else
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int res = K_OTHER;
    int c = getchar();
    if (c == EOF) {
        res = K_ESC;
    } else if (c == 27) {
        newt.c_cc[VMIN] = 0;
        newt.c_cc[VTIME] = 1;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        int c2 = getchar();
        if (c2 == '[') {
            int c3 = getchar();
            if (c3 == 'A') res = K_UP;
            else if (c3 == 'B') res = K_DOWN;
        } else if (c2 == EOF) {
            clearerr(stdin);
            res = K_ESC;
        }
    } else if (c == '\n') {
        res = K_ENTER;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return res;
#endif
}

void setHighlight(bool on) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), on ? 0xF0 : 0x07);
#else
    cout << (on ? "\033[7m" : "\033[0m");
#endif
}

// ---- Ve lai tung dong thay vi xoa ca man hinh (chong chop) ----
int originY = 0;  // dong dau tien cua menu (dung cho Windows)

void markOrigin() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    originY = info.dwCursorPosition.Y;
#endif
}

void gotoRow(int row) {
#ifdef _WIN32
    COORD c;
    c.X = 0;
    c.Y = (SHORT)(originY + row);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
#else
    cout << "\033[" << (row + 1) << ";1H";
#endif
}

void showCursor(bool show) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(h, &ci);
    ci.bVisible = show ? TRUE : FALSE;
    SetConsoleCursorInfo(h, &ci);
#else
    cout << (show ? "\033[?25h" : "\033[?25l");
#endif
}

void drawItem(const string &text, bool selected, int w) {
    string l = string(selected ? "> " : "  ") + text;
    l.resize(w - 1, ' ');
    cout << "  |";
    if (selected) setHighlight(true);
    cout << " " << l;
    if (selected) setHighlight(false);
    cout << "|";
}

// Menu chon bang phim Len/Xuong. Tra ve chi so muc chon, hoac -1 neu bam Esc.
// Chi xoa man hinh 1 lan luc mo menu; khi doi muc chi ve lai 2 dong bi anh huong.
int selectMenu(const string &title, const vector<string> &items,
               const vector<string> &info = vector<string>(), int w = 56) {
    int n = (int)items.size();
    int cur = 0;

    clearScreen();
    markOrigin();
    string bar(w, '=');
    cout << "  +" << bar << "+\n";
    string t = title;
    if ((int)t.size() > w) t.resize(w);
    int pad = (w - (int)t.size()) / 2;
    cout << "  |" << string(pad, ' ') << t << string(w - pad - t.size(), ' ') << "|\n";
    cout << "  +" << bar << "+\n";
    for (int i = 0; i < n; i++) {
        drawItem(items[i], i == cur, w);
        cout << "\n";
    }
    cout << "  +" << bar << "+\n";
    cout << "  [Len/Xuong] Chon   [Enter] Xac nhan   [Esc] Quay lai\n";
    if (!info.empty()) {
        cout << "\n";
        for (size_t i = 0; i < info.size(); i++) cout << "  " << info[i] << "\n";
    }
    showCursor(false);
    cout << flush;

    int result;
    while (true) {
        int k = getKey();
        int old = cur;
        if (k == K_UP) cur = (cur + n - 1) % n;
        else if (k == K_DOWN) cur = (cur + 1) % n;
        else if (k == K_ENTER) { result = cur; break; }
        else if (k == K_ESC) { result = -1; break; }
        if (cur != old) {
            gotoRow(3 + old);
            drawItem(items[old], false, w);
            gotoRow(3 + cur);
            drawItem(items[cur], true, w);
            cout << flush;
        }
    }
    showCursor(true);
    cout << flush;
    return result;
}

// Nhap chuoi. allowEmpty=true: cho phep bo trong (dung khi sua de giu gia tri cu)
string inputStr(const string &label, size_t maxLen = 0, bool allowEmpty = false) {
    string s;
    while (true) {
        cout << "  " << label << ": ";
        getline(cin, s);
        s = trim(s);
        if (s.empty() && !allowEmpty) {
            cout << "  ! Khong duoc de trong.\n";
            continue;
        }
        if (s.find('|') != string::npos) {
            cout << "  ! Khong duoc chua ky tu '|'.\n";
            continue;
        }
        if (maxLen > 0 && s.size() > maxLen) {
            cout << "  ! Toi da " << maxLen << " ky tu.\n";
            continue;
        }
        return s;
    }
}

int inputInt(const string &label, int lo, int hi) {
    string s;
    while (true) {
        cout << "  " << label << ": ";
        getline(cin, s);
        s = trim(s);
        bool ok = !s.empty() && s.size() < 10;
        for (size_t i = 0; i < s.size() && ok; i++)
            if (!isdigit((unsigned char)s[i])) ok = false;
        if (ok) {
            int v = atoi(s.c_str());
            if (v >= lo && v <= hi) return v;
        }
        cout << "  ! Nhap so nguyen tu " << lo << " den " << hi << ".\n";
    }
}

bool confirm(const string &q, const vector<string> &info = vector<string>()) {
    vector<string> opt;
    opt.push_back("Khong");
    opt.push_back("Co");
    return selectMenu(q, opt, info) == 1;
}

/* ===================== TIEN ICH: COT, TIEN, NGAY, BANG ===================== */
string colL(string s, int w) {
    if ((int)s.size() > w) s = s.substr(0, w);
    s.resize(w, ' ');
    return s;
}

string colR(string s, int w) {
    if ((int)s.size() > w) s = s.substr(0, w);
    return string(w - s.size(), ' ') + s;
}

// Dinh dang tien: ngan cach hang nghin, chi hien thap phan khi that su co le
string fmtMoney(double v) {
    bool neg = v < 0;
    if (neg) v = -v;
    bool frac = fabs(v - floor(v + 0.5)) >= 0.005;
    ostringstream os;
    os << fixed << setprecision(frac ? 2 : 0) << v;
    string s = os.str();
    size_t dot = s.find('.');
    string ip = (dot == string::npos) ? s : s.substr(0, dot);
    string rest = (dot == string::npos) ? "" : s.substr(dot);
    string out;
    int cnt = 0;
    for (int i = (int)ip.size() - 1; i >= 0; i--) {
        out += ip[i];
        if (++cnt % 3 == 0 && i > 0) out += ',';
    }
    reverse(out.begin(), out.end());
    return (neg ? "-" : "") + out + rest;
}

bool laNamNhuan(int y) { return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0); }

int soNgayTrongThang(int m, int y) {
    static const int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && laNamNhuan(y)) return 29;
    return d[m - 1];
}

// Ngay dang dd/mm/yyyy
bool parseDate(const string &s, int &d, int &m, int &y) {
    if (s.size() != 10 || s[2] != '/' || s[5] != '/') return false;
    for (int i = 0; i < 10; i++)
        if (i != 2 && i != 5 && !isdigit((unsigned char)s[i])) return false;
    d = atoi(s.substr(0, 2).c_str());
    m = atoi(s.substr(3, 2).c_str());
    y = atoi(s.substr(6, 4).c_str());
    if (y < 1900 || y > 2200 || m < 1 || m > 12) return false;
    return d >= 1 && d <= soNgayTrongThang(m, y);
}

// Doi ngay sang so yyyymmdd de so sanh; 0 neu sai
long dateKey(const string &s) {
    int d, m, y;
    if (!parseDate(s, d, m, y)) return 0;
    return y * 10000L + m * 100 + d;
}

string inputDate(const string &label) {
    while (true) {
        string s = inputStr(label, 10);
        if (dateKey(s) != 0) return s;
        cout << "  ! Ngay khong hop le (dang dd/mm/yyyy, vi du 05/03/2026).\n";
    }
}

double inputDouble(const string &label, double lo, double hi, bool loExclusive) {
    while (true) {
        string s = inputStr(label);
        char *e;
        double v = strtod(s.c_str(), &e);
        bool ok = (*e == '\0') && v <= hi && (loExclusive ? v > lo : v >= lo);
        if (ok) return v;
        cout << "  ! Nhap so " << (loExclusive ? "lon hon " : "tu ") << fmtMoney(lo)
             << " den " << fmtMoney(hi) << ".\n";
    }
}

// Hien bang co phan trang: Len/Xuong doi trang, Enter trang ke (trang cuoi thi thoat), Esc thoat
void showTable(const string &title, const vector<string> &head, const vector<string> &rows,
               const vector<string> &foot, int w = 72, int pageSize = 15) {
    int total = (int)rows.size();
    int pages = max(1, (total + pageSize - 1) / pageSize);
    int pg = 0;
    while (true) {
        clearScreen();
        drawBox(title, vector<string>(), w);
        cout << "\n";
        for (size_t i = 0; i < head.size(); i++) cout << "  " << head[i] << "\n";
        cout << "  " << string(w, '-') << "\n";
        if (total == 0) cout << "  (Khong co du lieu)\n";
        for (int i = pg * pageSize; i < min(total, (pg + 1) * pageSize); i++)
            cout << "  " << rows[i] << "\n";
        cout << "  " << string(w, '-') << "\n";
        if (pg == pages - 1)
            for (size_t i = 0; i < foot.size(); i++) cout << "  " << foot[i] << "\n";
        cout << "\n  Trang " << (pg + 1) << "/" << pages;
        if (pages > 1) cout << "   [Len/Xuong] Chuyen trang";
        cout << "   [Esc/Enter] Thoat\n";
        int k = getKey();
        if (k == K_ESC) break;
        if (k == K_DOWN || k == K_ENTER) {
            if (pg < pages - 1) pg++;
            else if (k == K_ENTER) break;
        } else if (k == K_UP && pg > 0) {
            pg--;
        }
    }
}

/* ===================== VAT TU (cay nhi phan tim kiem theo MAVT) ===================== */
treeVT timVT(treeVT t, const string &ma) {
    while (t != NULL) {
        if (ma == t->vt.MAVT) return t;
        t = (ma < t->vt.MAVT) ? t->left : t->right;
    }
    return NULL;
}

bool themVT(treeVT &t, const Vattu &v) {
    if (t == NULL) {
        t = new nodeVT;
        t->vt = v;
        t->left = t->right = NULL;
        return true;
    }
    if (v.MAVT == t->vt.MAVT) return false;
    return (v.MAVT < t->vt.MAVT) ? themVT(t->left, v) : themVT(t->right, v);
}

bool xoaVT(treeVT &t, const string &ma) {
    if (t == NULL) return false;
    if (ma < t->vt.MAVT) return xoaVT(t->left, ma);
    if (ma > t->vt.MAVT) return xoaVT(t->right, ma);
    // tim thay nut can xoa
    if (t->left == NULL) {
        treeVT del = t;
        t = t->right;
        delete del;
    } else if (t->right == NULL) {
        treeVT del = t;
        t = t->left;
        delete del;
    } else {
        // 2 con: thay bang nut trai nhat cua cay con phai
        treeVT p = t->right;
        while (p->left != NULL) p = p->left;
        t->vt = p->vt;
        return xoaVT(t->right, p->vt.MAVT);
    }
    return true;
}

void duyetLNR(treeVT t, vector<Vattu> &ds) {
    if (t == NULL) return;
    duyetLNR(t->left, ds);
    ds.push_back(t->vt);
    duyetLNR(t->right, ds);
}

void giaiPhongCay(treeVT &t) {
    if (t == NULL) return;
    giaiPhongCay(t->left);
    giaiPhongCay(t->right);
    delete t;
    t = NULL;
}

// Vat tu da xuat hien trong hoa don nao chua? (khong cho xoa neu da dung)
bool vattuDaDuocDung(const DS_NV &ds, const string &ma) {
    for (int i = 0; i < ds.n; i++)
        for (PTRHD h = ds.nodes[i]->dshd; h != NULL; h = h->next)
            for (PTRCTHD c = h->hd.dscthd; c != NULL; c = c->next)
                if (c->cthd.MAVT == ma) return true;
    return false;
}

/* --- Chuc nang a: nhap vat tu (them / xoa / hieu chinh) --- */
void themVatTu(treeVT &dsvt) {
    drawHeader("THEM VAT TU");
    Vattu v;
    v.MAVT = toUpper(inputStr("Ma vat tu (toi da 10 ky tu)", 10));
    if (timVT(dsvt, v.MAVT) != NULL) {
        cout << "\n  ! Ma vat tu da ton tai.\n";
        return;
    }
    v.TENVT = inputStr("Ten vat tu");
    v.DVT = inputStr("Don vi tinh");
    v.SOLUONGTON = inputInt("So luong ton (>= 0)", 0, 1000000000);
    themVT(dsvt, v);
    cout << "\n  => Da them vat tu " << v.MAVT << ".\n";
}

void xoaVatTu(treeVT &dsvt, const DS_NV &dsnv) {
    drawHeader("XOA VAT TU");
    string ma = toUpper(inputStr("Ma vat tu can xoa", 10));
    treeVT p = timVT(dsvt, ma);
    if (p == NULL) {
        cout << "\n  ! Khong tim thay ma vat tu.\n";
        return;
    }
    cout << "  Ten: " << p->vt.TENVT << " | DVT: " << p->vt.DVT
         << " | Ton: " << p->vt.SOLUONGTON << "\n";
    if (vattuDaDuocDung(dsnv, ma)) {
        cout << "\n  ! Vat tu da co trong hoa don, khong the xoa.\n";
        return;
    }
    vector<string> inf;
    inf.push_back("Ma: " + ma + " | Ten: " + p->vt.TENVT + " | Ton: " + to_string(p->vt.SOLUONGTON));
    if (confirm("Xac nhan xoa vat tu?", inf)) {
        xoaVT(dsvt, ma);
        cout << "\n  => Da xoa vat tu " << ma << ".\n";
    } else {
        cout << "\n  Da huy.\n";
    }
}

void suaVatTu(treeVT &dsvt) {
    drawHeader("HIEU CHINH VAT TU");
    string ma = toUpper(inputStr("Ma vat tu can sua", 10));
    treeVT p = timVT(dsvt, ma);
    if (p == NULL) {
        cout << "\n  ! Khong tim thay ma vat tu.\n";
        return;
    }
    cout << "  Hien tai -> Ten: " << p->vt.TENVT << " | DVT: " << p->vt.DVT
         << " | Ton: " << p->vt.SOLUONGTON << " (khong duoc sua)\n";
    cout << "  (De trong de giu nguyen)\n";
    string ten = inputStr("Ten vat tu moi", 0, true);
    string dvt = inputStr("Don vi tinh moi", 0, true);
    if (!ten.empty()) p->vt.TENVT = ten;
    if (!dvt.empty()) p->vt.DVT = dvt;
    cout << "\n  => Da cap nhat.\n";
}

void menuNhapVatTu(treeVT &dsvt, DS_NV &dsnv) {
    vector<string> m;
    m.push_back("Them vat tu");
    m.push_back("Xoa vat tu");
    m.push_back("Hieu chinh vat tu");
    m.push_back("Quay lai");
    while (true) {
        int c = selectMenu("NHAP VAT TU", m);
        if (c == -1 || c == 3) return;
        if (c == 0) themVatTu(dsvt);
        else if (c == 1) xoaVatTu(dsvt, dsnv);
        else suaVatTu(dsvt);
        pauseScreen();
    }
}

/* --- Chuc nang b: in vat tu ton kho theo ten tang dan --- */
void inVatTuTon(treeVT dsvt) {
    vector<Vattu> ds;
    duyetLNR(dsvt, ds);
    stable_sort(ds.begin(), ds.end(), [](const Vattu &a, const Vattu &b) {
        return a.TENVT < b.TENVT;
    });
    vector<string> head, rows, foot;
    head.push_back(colL("Ma VT", 12) + colL("Ten vat tu", 26) + colL("Don vi tinh", 14) +
                   colR("So luong ton", 12));
    for (size_t i = 0; i < ds.size(); i++)
        rows.push_back(colL(ds[i].MAVT, 12) + colL(ds[i].TENVT, 26) + colL(ds[i].DVT, 14) +
                       colR(to_string(ds[i].SOLUONGTON), 12));
    foot.push_back("Tong: " + to_string(ds.size()) + " vat tu");
    showTable("DANH SACH VAT TU TON KHO", head, rows, foot);
}

/* ===================== NHAN VIEN (mang con tro, sap theo MANV) ===================== */
int timNV(const DS_NV &ds, const string &ma) {
    for (int i = 0; i < ds.n; i++)
        if (ds.nodes[i]->MANV == ma) return i;
    return -1;
}

// Chen giu thu tu tang dan theo MANV
bool chenNV(DS_NV &ds, Nhanvien *nv) {
    if (ds.n >= MAXNV) return false;
    int pos = ds.n;
    while (pos > 0 && ds.nodes[pos - 1]->MANV > nv->MANV) {
        ds.nodes[pos] = ds.nodes[pos - 1];
        pos--;
    }
    ds.nodes[pos] = nv;
    ds.n++;
    return true;
}

/* --- Chuc nang c: nhap nhan vien --- */
void nhapNhanVien(DS_NV &dsnv) {
    drawHeader("NHAP NHAN VIEN");
    if (dsnv.n >= MAXNV) {
        cout << "  ! Danh sach da day (" << MAXNV << " nhan vien).\n";
        return;
    }
    cout << "  Hien co: " << dsnv.n << "/" << MAXNV << " nhan vien\n\n";
    string ma = toUpper(inputStr("Ma nhan vien"));
    if (timNV(dsnv, ma) >= 0) {
        cout << "\n  ! Ma nhan vien da ton tai.\n";
        return;
    }
    Nhanvien *nv = new Nhanvien;
    nv->MANV = ma;
    nv->HO = inputStr("Ho");
    nv->TEN = inputStr("Ten");
    vector<string> gm, gi;
    gm.push_back("Nam");
    gm.push_back("Nu");
    gi.push_back("Ma NV: " + nv->MANV);
    gi.push_back("Ho ten: " + nv->HO + " " + nv->TEN);
    int g;
    do {
        g = selectMenu("CHON PHAI", gm, gi);
    } while (g < 0);
    nv->PHAI = (g == 0) ? "Nam" : "Nu";
    nv->dshd = NULL;
    chenNV(dsnv, nv);
    cout << "\n  => Da them nhan vien " << ma << ".\n";
}

/* --- Chuc nang d: in nhan vien theo ten tang dan, trung ten thi theo ho --- */
void inNhanVien(const DS_NV &dsnv) {
    vector<Nhanvien *> tmp(dsnv.nodes, dsnv.nodes + dsnv.n);
    stable_sort(tmp.begin(), tmp.end(), [](Nhanvien *a, Nhanvien *b) {
        if (a->TEN != b->TEN) return a->TEN < b->TEN;
        return a->HO < b->HO;
    });
    vector<string> head, rows, foot;
    head.push_back(colL("Ma NV", 12) + colL("Ho", 22) + colL("Ten", 14) + colL("Phai", 8));
    for (size_t i = 0; i < tmp.size(); i++)
        rows.push_back(colL(tmp[i]->MANV, 12) + colL(tmp[i]->HO, 22) + colL(tmp[i]->TEN, 14) +
                       colL(tmp[i]->PHAI, 8));
    foot.push_back("Tong: " + to_string(tmp.size()) + " nhan vien");
    showTable("DANH SACH NHAN VIEN", head, rows, foot);
}

void giaiPhongNV(DS_NV &ds) {
    for (int i = 0; i < ds.n; i++) {
        PTRHD h = ds.nodes[i]->dshd;
        while (h != NULL) {
            PTRCTHD c = h->hd.dscthd;
            while (c != NULL) {
                PTRCTHD cn = c->next;
                delete c;
                c = cn;
            }
            PTRHD hn = h->next;
            delete h;
            h = hn;
        }
        delete ds.nodes[i];
    }
    ds.n = 0;
}

/* ===================== HOA DON ===================== */
PTRHD timHD(const DS_NV &ds, const string &so, int *idx = NULL) {
    for (int i = 0; i < ds.n; i++)
        for (PTRHD h = ds.nodes[i]->dshd; h != NULL; h = h->next)
            if (h->hd.SOHD == so) {
                if (idx) *idx = i;
                return h;
            }
    return NULL;
}

PTRCTHD timCT(PTRCTHD head, const string &ma) {
    for (; head != NULL; head = head->next)
        if (head->cthd.MAVT == ma) return head;
    return NULL;
}

void themCuoiCT(PTRCTHD &head, const CT_HOADON &ct) {
    PTRCTHD n = new nodeCTHD;
    n->cthd = ct;
    n->next = NULL;
    if (head == NULL) head = n;
    else {
        PTRCTHD p = head;
        while (p->next != NULL) p = p->next;
        p->next = n;
    }
}

bool xoaCT(PTRCTHD &head, const string &ma, CT_HOADON *out) {
    PTRCTHD prev = NULL, p = head;
    while (p != NULL && p->cthd.MAVT != ma) {
        prev = p;
        p = p->next;
    }
    if (p == NULL) return false;
    if (out) *out = p->cthd;
    if (prev) prev->next = p->next;
    else head = p->next;
    delete p;
    return true;
}

// Tri gia 1 dong = SL * Don gia * (1 + %VAT/100)
double triGiaCT(const CT_HOADON &c) {
    return (double)c.SOLUONG * c.DONGIA * (1.0 + c.VAT / 100.0);
}

double triGiaHD(PTRCTHD head) {
    double s = 0;
    for (; head != NULL; head = head->next) s += triGiaCT(head->cthd);
    return s;
}

// Cap nhat ton: phieu N cong, phieu X tru. hoan=true la dao nguoc (khi xoa/huy)
void capNhatTon(treeVT dsvt, const CT_HOADON &ct, char loai, bool hoan) {
    treeVT p = timVT(dsvt, ct.MAVT);
    if (p == NULL) return;
    int delta = (loai == 'N') ? ct.SOLUONG : -ct.SOLUONG;
    if (hoan) delta = -delta;
    p->vt.SOLUONGTON += delta;
}

string tenLoai(char l) { return l == 'N' ? "Nhap" : "Xuat"; }

vector<string> thongTinHD(const Nhanvien *nv, const HOADON &hd, treeVT dsvt) {
    vector<string> v;
    v.push_back("NV lap : " + nv->MANV + " - " + nv->HO + " " + nv->TEN);
    v.push_back("So HD  : " + hd.SOHD + " | Ngay: " + hd.NGAYLAP + " | Loai: " + tenLoai(hd.LOAI));
    v.push_back("");
    v.push_back(colL("Ma VT", 11) + colL("Ten vat tu", 18) + colR("SL", 7) + colR("Don gia", 13) +
                colR("VAT%", 6) + colR("Tri gia", 15));
    int cnt = 0;
    for (PTRCTHD c = hd.dscthd; c != NULL; c = c->next, cnt++) {
        treeVT p = timVT(dsvt, c->cthd.MAVT);
        v.push_back(colL(c->cthd.MAVT, 11) + colL(p ? p->vt.TENVT : "?", 18) +
                    colR(to_string(c->cthd.SOLUONG), 7) + colR(fmtMoney(c->cthd.DONGIA), 13) +
                    colR(fmtMoney(c->cthd.VAT), 6) + colR(fmtMoney(triGiaCT(c->cthd)), 15));
    }
    if (cnt == 0) v.push_back("(chua co vat tu nao)");
    v.push_back("Tong tri gia: " + fmtMoney(triGiaHD(hd.dscthd)));
    return v;
}

void themVTVaoHD(treeVT dsvt, HOADON &hd) {
    drawHeader("THEM VAT TU VAO HOA DON " + hd.SOHD);
    string ma = toUpper(inputStr("Ma vat tu", 10));
    treeVT p = timVT(dsvt, ma);
    if (p == NULL) {
        cout << "\n  ! Ma vat tu khong ton tai.\n";
        return;
    }
    if (timCT(hd.dscthd, ma) != NULL) {
        cout << "\n  ! Vat tu da co trong hoa don nay.\n";
        return;
    }
    cout << "  Ten: " << p->vt.TENVT << " | DVT: " << p->vt.DVT
         << " | Ton hien co: " << p->vt.SOLUONGTON << "\n";
    int sl = inputInt("So luong (>= 1)", 1, 1000000000);
    if (hd.LOAI == 'X' && sl > p->vt.SOLUONGTON) {
        cout << "\n  ! Khong du hang de xuat. Ton hien co: " << p->vt.SOLUONGTON << "\n";
        return;
    }
    if (hd.LOAI == 'N' && (long long)p->vt.SOLUONGTON + sl > 2000000000LL) {
        cout << "\n  ! So luong ton vuot gioi han cho phep.\n";
        return;
    }
    CT_HOADON ct;
    ct.MAVT = ma;
    ct.SOLUONG = sl;
    ct.DONGIA = (float)inputDouble("Don gia (> 0)", 0, 1e12, true);
    ct.VAT = (float)inputDouble("%VAT (0 - 100)", 0, 100, false);
    themCuoiCT(hd.dscthd, ct);
    capNhatTon(dsvt, ct, hd.LOAI, false);
    cout << "\n  => Da them. Ton moi cua " << ma << ": " << p->vt.SOLUONGTON << "\n";
}

// Chi xoa duoc vat tu cua hoa don dang lap (chua ghi)
void xoaVTDangLap(treeVT dsvt, HOADON &hd) {
    if (hd.dscthd == NULL) {
        drawHeader("XOA VAT TU DANG LAP");
        cout << "  ! Hoa don chua co vat tu nao.\n";
        pauseScreen();
        return;
    }
    vector<string> items, mas;
    for (PTRCTHD c = hd.dscthd; c != NULL; c = c->next) {
        treeVT p = timVT(dsvt, c->cthd.MAVT);
        mas.push_back(c->cthd.MAVT);
        items.push_back(c->cthd.MAVT + " - " + (p ? p->vt.TENVT : "?") + " (SL: " +
                        to_string(c->cthd.SOLUONG) + ")");
    }
    vector<string> inf;
    inf.push_back("Chon vat tu can xoa khoi hoa don dang lap (Esc: huy).");
    int k = selectMenu("XOA VAT TU DANG LAP", items, inf);
    if (k < 0) return;
    CT_HOADON out;
    if (xoaCT(hd.dscthd, mas[k], &out)) {
        capNhatTon(dsvt, out, hd.LOAI, true);
        drawHeader("XOA VAT TU DANG LAP");
        cout << "  => Da xoa " << mas[k] << " va hoan lai ton kho.\n";
        pauseScreen();
    }
}

// Huy hoa don dang lap: hoan ton kho va giai phong danh sach chi tiet
void huyHoaDon(treeVT dsvt, HOADON &hd) {
    PTRCTHD c = hd.dscthd;
    while (c != NULL) {
        capNhatTon(dsvt, c->cthd, hd.LOAI, true);
        PTRCTHD nx = c->next;
        delete c;
        c = nx;
    }
    hd.dscthd = NULL;
}

/* --- Chuc nang e: lap hoa don nhap / xuat --- */
void lapHoaDon(treeVT &dsvt, DS_NV &dsnv) {
    drawHeader("LAP HOA DON NHAP / XUAT");
    if (dsnv.n == 0) {
        cout << "  ! Chua co nhan vien nao. Hay nhap nhan vien truoc.\n";
        pauseScreen();
        return;
    }
    if (dsvt == NULL) {
        cout << "  ! Chua co vat tu nao. Hay nhap vat tu truoc.\n";
        pauseScreen();
        return;
    }
    string manv = toUpper(inputStr("Ma nhan vien lap hoa don"));
    int idx = timNV(dsnv, manv);
    if (idx < 0) {
        cout << "\n  ! Khong tim thay nhan vien.\n";
        pauseScreen();
        return;
    }
    string so = toUpper(inputStr("So hoa don (toi da 20 ky tu)", 20));
    if (timHD(dsnv, so) != NULL) {
        cout << "\n  ! So hoa don da ton tai.\n";
        pauseScreen();
        return;
    }
    string ngay = inputDate("Ngay lap (dd/mm/yyyy)");

    vector<string> lm, li;
    lm.push_back("N - Phieu nhap");
    lm.push_back("X - Phieu xuat");
    li.push_back("So HD: " + so + " | Ngay: " + ngay);
    int l = selectMenu("CHON LOAI HOA DON", lm, li);
    if (l < 0) return;  // Esc: bo, chua co gi thay doi

    Nhanvien *nv = dsnv.nodes[idx];
    HOADON hd;
    hd.SOHD = so;
    hd.NGAYLAP = ngay;
    hd.LOAI = (l == 0) ? 'N' : 'X';
    hd.dscthd = NULL;

    vector<string> menu;
    menu.push_back("Them vat tu");
    menu.push_back("Xoa vat tu dang lap");
    menu.push_back("Ghi hoa don");
    menu.push_back("Huy hoa don");
    while (true) {
        int c = selectMenu("LAP HOA DON", menu, thongTinHD(nv, hd, dsvt));
        if (c == 0) {
            themVTVaoHD(dsvt, hd);
            pauseScreen();
        } else if (c == 1) {
            xoaVTDangLap(dsvt, hd);
        } else if (c == 2) {
            if (hd.dscthd == NULL) {
                drawHeader("GHI HOA DON");
                cout << "  ! Hoa don phai co it nhat 1 vat tu.\n";
                pauseScreen();
                continue;
            }
            PTRHD n = new nodeHD;
            n->hd = hd;
            n->next = NULL;
            if (nv->dshd == NULL) nv->dshd = n;
            else {
                PTRHD t = nv->dshd;
                while (t->next != NULL) t = t->next;
                t->next = n;
            }
            drawHeader("GHI HOA DON");
            cout << "  => Da ghi hoa don " << so << ". Ton kho da duoc cap nhat.\n";
            pauseScreen();
            return;
        } else {  // Huy (muc 3) hoac Esc
            if (hd.dscthd == NULL) {
                return;
            }
            vector<string> inf;
            inf.push_back("Ton kho se duoc hoan lai nhu truoc khi lap.");
            if (confirm("Huy hoa don dang lap?", inf)) {
                huyHoaDon(dsvt, hd);
                return;
            }
        }
    }
}

/* --- Chuc nang f: in hoa don theo so hoa don --- */
void inHoaDon(treeVT dsvt, const DS_NV &dsnv) {
    drawHeader("IN HOA DON");
    string so = toUpper(inputStr("Nhap so hoa don", 20));
    int idx = -1;
    PTRHD h = timHD(dsnv, so, &idx);
    if (h == NULL) {
        cout << "\n  ! Khong tim thay hoa don.\n";
        pauseScreen();
        return;
    }
    const Nhanvien *nv = dsnv.nodes[idx];
    vector<string> head, rows, foot;
    head.push_back("So hoa don : " + h->hd.SOHD);
    head.push_back("Ngay lap   : " + h->hd.NGAYLAP);
    head.push_back("Ho ten NV  : " + nv->HO + " " + nv->TEN);
    head.push_back("Loai       : " + tenLoai(h->hd.LOAI));
    head.push_back("");
    head.push_back(colL("Ten vat tu", 24) + colR("So luong", 10) + colR("Don gia", 14) +
                   colR("VAT%", 6) + colR("Tri gia", 16));
    for (PTRCTHD c = h->hd.dscthd; c != NULL; c = c->next) {
        treeVT p = timVT(dsvt, c->cthd.MAVT);
        rows.push_back(colL(p ? p->vt.TENVT : c->cthd.MAVT, 24) +
                       colR(to_string(c->cthd.SOLUONG), 10) + colR(fmtMoney(c->cthd.DONGIA), 14) +
                       colR(fmtMoney(c->cthd.VAT), 6) + colR(fmtMoney(triGiaCT(c->cthd)), 16));
    }
    foot.push_back("Tong tri gia HD : " + fmtMoney(triGiaHD(h->hd.dscthd)));
    showTable("HOA DON " + so, head, rows, foot, 72, 12);
}

/* ===================== THONG KE ===================== */
bool nhapKhoang(string &tu, string &den, long &k1, long &k2) {
    tu = inputDate("Tu ngay (dd/mm/yyyy)");
    den = inputDate("Den ngay (dd/mm/yyyy)");
    k1 = dateKey(tu);
    k2 = dateKey(den);
    if (k1 > k2) {
        cout << "\n  ! 'Tu ngay' phai truoc hoac bang 'Den ngay'.\n";
        pauseScreen();
        return false;
    }
    return true;
}

struct RowHD {
    long key;
    string so, ngay, loai, ten;
    double tg;
};

/* --- Chuc nang g: liet ke hoa don trong khoang thoi gian --- */
void thongKeHoaDon(const DS_NV &dsnv) {
    drawHeader("THONG KE HOA DON THEO KHOANG THOI GIAN");
    string tu, den;
    long k1, k2;
    if (!nhapKhoang(tu, den, k1, k2)) return;
    vector<RowHD> ds;
    for (int i = 0; i < dsnv.n; i++)
        for (PTRHD h = dsnv.nodes[i]->dshd; h != NULL; h = h->next) {
            long k = dateKey(h->hd.NGAYLAP);
            if (k < k1 || k > k2) continue;
            RowHD r;
            r.key = k;
            r.so = h->hd.SOHD;
            r.ngay = h->hd.NGAYLAP;
            r.loai = tenLoai(h->hd.LOAI);
            r.ten = dsnv.nodes[i]->HO + " " + dsnv.nodes[i]->TEN;
            r.tg = triGiaHD(h->hd.dscthd);
            ds.push_back(r);
        }
    stable_sort(ds.begin(), ds.end(), [](const RowHD &a, const RowHD &b) {
        if (a.key != b.key) return a.key < b.key;
        return a.so < b.so;
    });
    vector<string> head, rows, foot;
    head.push_back("          Tu ngay : " + tu + "        Den ngay : " + den);
    head.push_back("");
    head.push_back(colL("So HD", 12) + colL("Ngay lap", 12) + colL("Loai HD", 8) +
                   colL("Ho ten NV lap", 24) + colR("Tri gia HD", 16));
    for (size_t i = 0; i < ds.size(); i++)
        rows.push_back(colL(ds[i].so, 12) + colL(ds[i].ngay, 12) + colL(ds[i].loai, 8) +
                       colL(ds[i].ten, 24) + colR(fmtMoney(ds[i].tg), 16));
    foot.push_back("Tong so hoa don: " + to_string(ds.size()));
    showTable("BANG LIET KE CAC HOA DON TRONG KHOANG THOI GIAN", head, rows, foot, 74);
}

/* --- Chuc nang h: 10 vat tu co doanh thu cao nhat (tinh tren hoa don xuat) --- */
void top10VatTu(treeVT dsvt, const DS_NV &dsnv) {
    drawHeader("TOP 10 VAT TU DOANH THU CAO NHAT");
    string tu, den;
    long k1, k2;
    if (!nhapKhoang(tu, den, k1, k2)) return;
    map<string, double> dt;
    for (int i = 0; i < dsnv.n; i++)
        for (PTRHD h = dsnv.nodes[i]->dshd; h != NULL; h = h->next) {
            long k = dateKey(h->hd.NGAYLAP);
            if (h->hd.LOAI != 'X' || k < k1 || k > k2) continue;
            for (PTRCTHD c = h->hd.dscthd; c != NULL; c = c->next)
                dt[c->cthd.MAVT] += triGiaCT(c->cthd);
        }
    vector<pair<double, string> > v;
    for (map<string, double>::iterator it = dt.begin(); it != dt.end(); ++it)
        v.push_back(make_pair(it->second, it->first));
    stable_sort(v.begin(), v.end(), [](const pair<double, string> &a, const pair<double, string> &b) {
        return a.first > b.first;
    });
    if (v.size() > 10) v.resize(10);
    vector<string> head, rows, foot;
    head.push_back("          Tu ngay : " + tu + "        Den ngay : " + den);
    head.push_back("");
    head.push_back(colL("Hang", 6) + colL("Ma VT", 12) + colL("Ten vat tu", 24) +
                   colL("DVT", 10) + colR("Doanh thu", 18));
    for (size_t i = 0; i < v.size(); i++) {
        treeVT p = timVT(dsvt, v[i].second);
        rows.push_back(colL(to_string(i + 1), 6) + colL(v[i].second, 12) +
                       colL(p ? p->vt.TENVT : "?", 24) + colL(p ? p->vt.DVT : "?", 10) +
                       colR(fmtMoney(v[i].first), 18));
    }
    foot.push_back("(Doanh thu tinh tren hoa don xuat, da gom VAT)");
    showTable("10 VAT TU CO DOANH THU CAO NHAT", head, rows, foot, 72);
}

/* --- Chuc nang i: doanh thu theo thang trong 1 nam --- */
void doanhThuNam(const DS_NV &dsnv) {
    drawHeader("THONG KE DOANH THU THEO NAM");
    int nam = inputInt("Nhap nam (1900 - 2200)", 1900, 2200);
    double thang[12] = {0};
    for (int i = 0; i < dsnv.n; i++)
        for (PTRHD h = dsnv.nodes[i]->dshd; h != NULL; h = h->next) {
            int d, m, y;
            if (h->hd.LOAI != 'X' || !parseDate(h->hd.NGAYLAP, d, m, y) || y != nam) continue;
            thang[m - 1] += triGiaHD(h->hd.dscthd);
        }
    vector<string> head, rows, foot;
    head.push_back(colL("THANG", 12) + colR("DOANH THU", 24));
    double tong = 0;
    for (int m = 0; m < 12; m++) {
        rows.push_back(colL(to_string(m + 1), 12) + colR(fmtMoney(thang[m]), 24));
        tong += thang[m];
    }
    foot.push_back(colL("CA NAM", 12) + colR(fmtMoney(tong), 24));
    showTable("BANG THONG KE DOANH THU NAM " + to_string(nam), head, rows, foot, 40, 12);
}

/* ===================== LUU / DOC FILE ===================== */
const string DEFAULT_FILE = "QLVT.txt";
/* Dinh dang (moi dong 1 ban ghi, cac truong cach nhau boi '|'):
   V|MAVT|TENVT|DVT|TON          - vat tu (ghi theo tien tu de dung lai dung hinh cay)
   N|MANV|HO|TEN|PHAI            - nhan vien
   H|SOHD|NGAYLAP|LOAI           - hoa don cua nhan vien N ngay phia tren
   C|MAVT|SOLUONG|DONGIA|VAT     - chi tiet cua hoa don H ngay phia tren */

void ghiCay(ofstream &f, treeVT t) {
    if (t == NULL) return;
    f << "V|" << t->vt.MAVT << "|" << t->vt.TENVT << "|" << t->vt.DVT << "|"
      << t->vt.SOLUONGTON << "\n";
    ghiCay(f, t->left);
    ghiCay(f, t->right);
}

bool luuFile(const string &fn, treeVT dsvt, const DS_NV &dsnv) {
    ofstream f(fn.c_str());
    if (!f) return false;
    f << setprecision(9);
    ghiCay(f, dsvt);
    for (int i = 0; i < dsnv.n; i++) {
        const Nhanvien *nv = dsnv.nodes[i];
        f << "N|" << nv->MANV << "|" << nv->HO << "|" << nv->TEN << "|" << nv->PHAI << "\n";
        for (PTRHD h = nv->dshd; h != NULL; h = h->next) {
            f << "H|" << h->hd.SOHD << "|" << h->hd.NGAYLAP << "|" << h->hd.LOAI << "\n";
            for (PTRCTHD c = h->hd.dscthd; c != NULL; c = c->next)
                f << "C|" << c->cthd.MAVT << "|" << c->cthd.SOLUONG << "|"
                  << (double)c->cthd.DONGIA << "|" << (double)c->cthd.VAT << "\n";
        }
    }
    f.flush();
    return (bool)f;
}

vector<string> splitStr(const string &s, char d) {
    vector<string> r;
    string cur;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == d) {
            r.push_back(cur);
            cur.clear();
        } else cur += s[i];
    }
    r.push_back(cur);
    return r;
}

bool parseInt(const string &s, long long &v) {
    if (s.empty()) return false;
    char *e;
    v = strtoll(s.c_str(), &e, 10);
    return *e == '\0';
}

bool parseNum(const string &s, double &v) {
    if (s.empty()) return false;
    char *e;
    v = strtod(s.c_str(), &e);
    return *e == '\0';
}

bool coChiTietSai(treeVT t, const DS_NV &ds) {
    for (int i = 0; i < ds.n; i++)
        for (PTRHD h = ds.nodes[i]->dshd; h != NULL; h = h->next)
            for (PTRCTHD c = h->hd.dscthd; c != NULL; c = c->next)
                if (timVT(t, c->cthd.MAVT) == NULL) return true;
    return false;
}

// Doc vao cau truc tam; chi thay du lieu hien tai khi file hoan toan hop le
bool docFile(const string &fn, treeVT &dsvt, DS_NV &dsnv, string &err) {
    ifstream f(fn.c_str());
    if (!f) {
        err = "Khong mo duoc file " + fn;
        return false;
    }
    treeVT t = NULL;
    DS_NV tmp;
    Nhanvien *curN = NULL;
    PTRHD lastH = NULL;
    PTRCTHD lastC = NULL;
    string line;
    int ln = 0;
    bool ok = true;
    while (ok && getline(f, line)) {
        ln++;
        line = trim(line);
        if (line.empty()) continue;
        vector<string> p = splitStr(line, '|');
        long long a;
        double d1, d2;
        if (p[0] == "V" && p.size() == 5) {
            Vattu v;
            v.MAVT = p[1];
            v.TENVT = p[2];
            v.DVT = p[3];
            if (v.MAVT.empty() || v.MAVT.size() > 10 || v.TENVT.empty() || v.DVT.empty() ||
                !parseInt(p[4], a) || a < 0 || a > 2000000000LL) ok = false;
            else {
                v.SOLUONGTON = (int)a;
                if (!themVT(t, v)) ok = false;
            }
        } else if (p[0] == "N" && p.size() == 5) {
            if (p[1].empty() || p[2].empty() || p[3].empty() || tmp.n >= MAXNV ||
                timNV(tmp, p[1]) >= 0) ok = false;
            else {
                Nhanvien *nv = new Nhanvien;
                nv->MANV = p[1];
                nv->HO = p[2];
                nv->TEN = p[3];
                nv->PHAI = p[4];
                nv->dshd = NULL;
                chenNV(tmp, nv);
                curN = nv;
                lastH = NULL;
                lastC = NULL;
            }
        } else if (p[0] == "H" && p.size() == 4) {
            if (curN == NULL || p[1].empty() || p[1].size() > 20 ||
                (p[3] != "N" && p[3] != "X") || dateKey(p[2]) == 0 ||
                timHD(tmp, p[1]) != NULL) ok = false;
            else {
                PTRHD h = new nodeHD;
                h->hd.SOHD = p[1];
                h->hd.NGAYLAP = p[2];
                h->hd.LOAI = p[3][0];
                h->hd.dscthd = NULL;
                h->next = NULL;
                if (lastH == NULL) curN->dshd = h;
                else lastH->next = h;
                lastH = h;
                lastC = NULL;
            }
        } else if (p[0] == "C" && p.size() == 5) {
            if (lastH == NULL || p[1].empty() || !parseInt(p[2], a) || a <= 0 ||
                a > 2000000000LL || !parseNum(p[3], d1) || d1 < 0 || !parseNum(p[4], d2) ||
                d2 < 0 || d2 > 100) ok = false;
            else {
                PTRCTHD c = new nodeCTHD;
                c->cthd.MAVT = p[1];
                c->cthd.SOLUONG = (int)a;
                c->cthd.DONGIA = (float)d1;
                c->cthd.VAT = (float)d2;
                c->next = NULL;
                if (lastC == NULL) lastH->hd.dscthd = c;
                else lastC->next = c;
                lastC = c;
            }
        } else ok = false;
        if (!ok) err = "Du lieu sai o dong " + to_string(ln);
    }
    if (ok && coChiTietSai(t, tmp)) {
        ok = false;
        err = "Hoa don co ma vat tu khong ton tai";
    }
    if (!ok) {
        giaiPhongCay(t);
        giaiPhongNV(tmp);
        return false;
    }
    giaiPhongCay(dsvt);
    giaiPhongNV(dsnv);
    dsvt = t;
    dsnv = tmp;
    return true;
}

void menuFile(treeVT &dsvt, DS_NV &dsnv) {
    vector<string> m;
    m.push_back("Luu du lieu vao file");
    m.push_back("Doc du lieu tu file");
    m.push_back("Quay lai");
    while (true) {
        int c = selectMenu("LUU / DOC FILE", m);
        if (c == -1 || c == 2) return;
        if (c == 0) {
            drawHeader("LUU DU LIEU");
            string fn = inputStr("Ten file (Enter = " + DEFAULT_FILE + ")", 0, true);
            if (fn.empty()) fn = DEFAULT_FILE;
            if (luuFile(fn, dsvt, dsnv)) cout << "\n  => Da luu vao " << fn << ".\n";
            else cout << "\n  ! Khong ghi duoc file " << fn << ".\n";
        } else {
            drawHeader("DOC DU LIEU");
            string fn = inputStr("Ten file (Enter = " + DEFAULT_FILE + ")", 0, true);
            if (fn.empty()) fn = DEFAULT_FILE;
            vector<string> inf;
            inf.push_back("File: " + fn);
            if (!confirm("Doc file se THAY THE du lieu hien tai. Tiep tuc?", inf)) continue;
            string err;
            drawHeader("DOC DU LIEU");
            if (docFile(fn, dsvt, dsnv, err)) cout << "  => Da doc du lieu tu " << fn << ".\n";
            else cout << "  ! " << err << "\n  (Du lieu hien tai khong bi thay doi)\n";
        }
        pauseScreen();
    }
}

int demVT(treeVT t) { return t == NULL ? 0 : 1 + demVT(t->left) + demVT(t->right); }

int demHD(const DS_NV &ds) {
    int n = 0;
    for (int i = 0; i < ds.n; i++)
        for (PTRHD h = ds.nodes[i]->dshd; h != NULL; h = h->next) n++;
    return n;
}

/* ===================== MAIN ===================== */
int main() {
    treeVT dsvt = NULL;
    DS_NV dsnv;
    string loadMsg;

    // Tu dong nap du lieu neu co file mac dinh
    {
        ifstream t(DEFAULT_FILE.c_str());
        if (t.good()) {
            t.close();
            string err;
            if (docFile(DEFAULT_FILE, dsvt, dsnv, err)) loadMsg = "Da nap du lieu tu " + DEFAULT_FILE;
            else loadMsg = "Loi nap " + DEFAULT_FILE + ": " + err;
        }
    }

    vector<string> m;
    m.push_back("Nhap vat tu (them / xoa / hieu chinh)");
    m.push_back("In danh sach vat tu ton kho (ten tang dan)");
    m.push_back("Nhap nhan vien");
    m.push_back("In danh sach nhan vien (ten, ho tang dan)");
    m.push_back("Lap hoa don nhap / xuat");
    m.push_back("In hoa don");
    m.push_back("Thong ke hoa don trong khoang thoi gian");
    m.push_back("10 vat tu co doanh thu cao nhat");
    m.push_back("Thong ke doanh thu theo nam");
    m.push_back("Luu / doc file");
    m.push_back("Thoat");

    while (true) {
        vector<string> info;
        info.push_back("Vat tu: " + to_string(demVT(dsvt)) + " | Nhan vien: " + to_string(dsnv.n) +
                       " | Hoa don: " + to_string(demHD(dsnv)));
        if (!loadMsg.empty()) info.push_back(loadMsg);
        int c = selectMenu("QUAN LY NHAP XUAT VAT TU", m, info);

        if (c == 0) menuNhapVatTu(dsvt, dsnv);
        else if (c == 1) inVatTuTon(dsvt);
        else if (c == 2) { nhapNhanVien(dsnv); pauseScreen(); }
        else if (c == 3) inNhanVien(dsnv);
        else if (c == 4) lapHoaDon(dsvt, dsnv);
        else if (c == 5) inHoaDon(dsvt, dsnv);
        else if (c == 6) thongKeHoaDon(dsnv);
        else if (c == 7) top10VatTu(dsvt, dsnv);
        else if (c == 8) doanhThuNam(dsnv);
        else if (c == 9) menuFile(dsvt, dsnv);
        else {  // Thoat hoac Esc
            vector<string> em;
            em.push_back("Luu va thoat");
            em.push_back("Thoat khong luu");
            em.push_back("Quay lai");
            int e = selectMenu("THOAT CHUONG TRINH", em);
            if (e == 0) {
                if (luuFile(DEFAULT_FILE, dsvt, dsnv)) break;
                drawHeader("LOI");
                cout << "  ! Khong ghi duoc file " << DEFAULT_FILE << ".\n";
                pauseScreen();
            } else if (e == 1) break;
        }
    }

    giaiPhongCay(dsvt);
    giaiPhongNV(dsnv);
    return 0;
}
