#pragma once
//#include <windows.h>
#include <string>
#include <cstdlib>
#include<iostream>
#include<conio.h>
#include<fstream>
#include <set>
using namespace std;
const int NO_OF_FILES = 100;
const int LIMIT = 1000;
using namespace std;


string removeSpecials(string strWord) {
    int i = 0, len = strWord.size(), j = 0;
    string newStr = "";
    while (i<len) {
        char cc = strWord[i];
        if ((cc >= '0' && cc <= '9') || (cc >= 'a' && cc <= 'z')) {
            newStr += strWord[i];
        }
        i++;
    }
    return newStr;
}

#include "Queue.h"
#include "Stack.h"
#include "Node.h"
#include "BooleanM.h"
#include "Fastfind.h"
