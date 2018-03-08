#pragma once
#include <QtCore>

using namespace std;
class Find {
    int *next, lenP, lenS;
    QString s, p;
public:
    Find(QString a, QString b)
    {
        s = a;
        p = b;
        lenP = p.length();
        lenS = s.length();
    }
    int fastfind();
    void getnext();
};

int Find::fastfind() {
    int posP = 0, posT = 0;
    lenP = p.length();
    int lenT = s.length();
    while (posP < lenP && posT < lenT)
        if (posP == -1 || p[posP] == s[posT])
        {
            posT++; posP++;
        }
        else
            posP = next[posP];
    if (posP < lenP) return -1;
    else
        return (posT - lenP);
}
void Find::getnext() {
    int j = 0, k = -1;
    next = new int[lenP];
    next[0] = -1;
    while (j < lenP - 1)
    {
        if (k == -1 || p[j] == p[k])
        {
            j++; k++;
            next[j] = k;
        }
        else k = next[k];
    }
}
