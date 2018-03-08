#pragma once
#include <QTCore>
using namespace std;
struct Result {
    double PageRank;
    int line;
    int file;
    QString word;
    QString content;
    Result *next;
};

struct SearchNodeTop {
    //	int file_num;
    QString first_word;
    QString first_PageRank;
    QString last_word;
    QString last_PageRank;
};

struct SearchNodeBT {
    QString word;
    QString PageRank;
    SearchNodeBT *next;
};

struct SearchNode {
    QString word;
    int file;
    SearchNode *next;
};

struct Index {
    int num;
    QString file;
};
