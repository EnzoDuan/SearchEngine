#pragma once
#pragma warning (disable:4996)
#include <fstream>
#include <sstream>
#include <iostream>
#include <sstream>
//#include <regex>
#include <windows.h>
#include "Node.h"
#include "Fastfind.h"
#include <QTCore>
#include <QTextCodec>
#include <QFileDialog>
#include <qdebug.h>
#include <string>
#include "sort.h"
using namespace std;

class BooleanRetrieval {
private:
    QString words;
    SearchNodeTop SearchList[701];						// BTree top list
    SearchNodeBT BTlist[724];							// every jieba result's real file list
    Index IndexList[1431];
    SearchNode *head_, *tail_;
    int a;

public:
    Result *resultHead, *resultTail, *h;
    BooleanRetrieval() {
        //wchar_t s[100];
        //FILE * f1;
        //int fileOpen = _wfopen_s(&f1, L"test.txt", L"rt+,ccs=UTF-8");
        QFile f1("C:/Users/kylin/Desktop/Search/in.txt");
        f1.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream f1_stream(&f1);
        f1_stream.setCodec("UTF-8");
        while (!f1_stream.atEnd())
        {
            words = f1_stream.readLine();
            //qDebug() << words;
        }
        f1.close();
        head_ = NULL; tail_ = NULL;
        resultHead = NULL;
        resultTail = NULL;
        a = 0;
    }
    /*void Wchar_tToString(string& szDst, wchar_t *wchar)
    {
        wchar_t * wText = wchar;
        DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
        char *psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
        psText = new char[dwNum];
        WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
        szDst = psText;// std::string赋值
        delete[]psText;// psText的清除
    }*/

    void Search() {
        //string buf;
        //wchar_t s[10000];
        QString buf;
        //FILE * f1;
        //int fileOpen = _wfopen_s(&f1, L"BTfileIndex.txt", L"rt+,ccs=UTF-8");
        QFile f1("C:/Users/kylin/Desktop/Search/BTfileIndex.txt");
        f1.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream f1_stream(&f1);
        f1_stream.setCodec("UTF-8");
        int BTfile_first = -1, BTfile_last = -1;		// record which file to read
        int count_line = 0;
        while (!f1_stream.atEnd())					// read top BTree file to rom
        {
            //fgetws(s, 10000, f1);
            buf = f1_stream.readLine();
            //qDebug() << buf;
            //Wchar_tToString(buf, s);
            /*int space = buf.find("word = ", 0);
            int first = buf.find("PageRank", 0);
            int second = buf.find("word = ", first);
            int third = buf.find("PageRank", second);
            int forth = buf.find("\"}", third);*/
            QRegExp rx("(\\d+) first = [{]word = \"(.*)\" PageRank = \"(.*)\"[}] last = [{]word = \"(.*)\" PageRank = \"(.*)\"[}]");
            rx.setMinimal(true);
            rx.indexIn(buf);
//			SearchList[count_line].file_num = atoi(cm[1].str().c_str());
            /*SearchList[count_line].first_word = buf.substr(space + 8, first - 3 - space - 7);
            SearchList[count_line].first_PageRank = buf.substr(first + 12, second - 12 - first - 11);
            SearchList[count_line].last_word = buf.substr(second + 8, third - 3 - second - 7);
            SearchList[count_line].last_PageRank = (third + 12, forth - 2 - third - 11);*/
            QString num = QString::number(count_line,10);
            //qDebug()<<rx.cap(2)<<rx.cap(3)<<rx.cap(4)<<rx.cap(5)+ "  " +num;
            SearchList[count_line].first_word = rx.cap(2);
            SearchList[count_line].first_PageRank = rx.cap(3);
            SearchList[count_line].last_word = rx.cap(4);
            SearchList[count_line].last_PageRank = rx.cap(5);
            count_line++;
            //if(count_line == 701) break;
        }
        QFile ifile("C:/Users/kylin/Desktop/Search/index.txt");
        ifile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream ifile_stream(&ifile);
        count_line = 0;
        while (!ifile_stream.atEnd())										// read index into rom
        {
            buf = ifile_stream.readLine();
            //qDebug() << buf;
            QRegExp rx("(\\d+) (.*)");
            rx.indexIn(buf);
            //int space = buf.find(' ', 0);
            //string s1 = buf.substr(0, space - 1);
            //string s2 = buf.substr(space + 1, (buf.length() - space - 1));
            //IndexList[count_line].num = count_line;
            //IndexList[count_line].file = s2;
            QString s1 = rx.cap(1);
            QString s2 = rx.cap(2);
            IndexList[count_line].num = count_line;
            IndexList[count_line].file = s2;
            count_line++;
        }
        ifile.close();
        int length = words.length();
        //qDebug() << words;
        /*
        char *word = new char[length + 1];
        word[length] = '\0';
        for (int i = 0; i < length; i++)
            word[i] = words.toStdString()[i];
        char *temp = strtok(word, "/");
        */
        QStringList words_list = words.split('/');
        //qDebug() << QString::number(words_list.size(),10);
        for(int i = 0; i < words_list.size(); i++){
            MainSearch(words_list[i]);
        }
        /*
        while (temp != NULL)
        {
            QString se = temp;
            qDebug() << se;
            MainSearch(se);
            temp = strtok(NULL, "/");
        }
        */
    }

    bool MainSearch(QString word) {
        //qDebug() << word;
        int begin = 0, end = 700;
        while (begin <= end)
        {
            int mid = (begin + end) / 2;
            //qDebug() << SearchList[mid].first_word << SearchList[mid].last_word ;
            if ( SearchList[mid].last_word >=  word &&  SearchList[mid].first_word <=  word)
                return SearchInBTFile(mid, word);
            else if ( word >  SearchList[mid].last_word)
                begin = mid + 1;
            else if ( word <  SearchList[mid].first_word)
                end = mid - 1;
        }
        return false;
    }

    bool SearchInBTFile(int filenum, QString word) {
        //qDebug() << QString::number(filenum,10);
        //char num1[10];
        //itoa(filenum, num1, 10);
        QString file = "C:/Users/kylin/Desktop/Search/BTfile/BTfile" + QString::number(filenum,10) + ".txt";
        //qDebug(file.toStdString().c_str());
        QFile infile(file);
        infile.open(QIODevice::ReadOnly | QIODevice::Text);
        QRegExp rx("word = \"(.*)\" PageRank = \" (.*)\"");
        rx.setMinimal(true);
        //ifstream infile(file);
        //regex r("word = \"(.*?)\" PageRank = \" (.*?)\"");
        QString buf;
        QTextStream infile_stream(&infile);
        infile_stream.setCodec("UTF-8");
        //cmatch cm;
        int count = 0;
        //while (getline(infile, buf))
        while(!infile_stream.atEnd())
        {
            buf = infile_stream.readLine();
            /*int len = buf.length();
            char *str = new char[len + 1];
            str[len] = '\0';
            for (int i = 0; i < len; i++)
                str[i] = buf.toStdString()[i];*/
            //regex_match(str, cm, r);
            rx.indexIn(buf);
            //BTlist[count].word = cm[1].str().c_str();
            //qDebug() << rx.cap(1) + "  " + rx.cap(2);
            BTlist[count].word = rx.cap(1);
            //BTlist[count].PageRank = cm[2].str().c_str();
            BTlist[count].PageRank = rx.cap(2);
            count++;
        }
        int begin = 0, end = count;   //count = 724
        while (begin <= end)
        {
            int mid = (begin + end) / 2;
            if (BTlist[mid].word == word){
                return SearchInSourseIndex(BTlist[mid]);
            }
            else if (word > BTlist[mid].word)
                begin = mid + 1;
            else if (word < BTlist[mid].word)
                end = mid - 1;
        }
        return false;
    }

    bool SearchInSourseIndex(SearchNodeBT node)
    {
        //regex r(" (.*?)|");
        QRegExp rx("(\\d+)[|]\\d+");
        rx.setMinimal(true);
        /*int len = node.PageRank.length();
        char *str = new char[len + 1];
        str[len] = '\0';
        for (int i = 0; i < len; i++)
            str[i] = node.PageRank[i];*/
        QString str = node.PageRank;
        //qDebug() << node.word << node.PageRank;
        //cmatch cm;
        //regex_match(str, cm, r);
        //rx.indexIn(str);
        QStringList rx_list;
        int pos = 0;
        while ((pos = rx.indexIn(str, pos)) != -1) {
              rx_list << rx.cap(1);
              pos += rx.matchedLength();
        }
        //qDebug() << rx_list.size();

        for(int i = 0; i < rx_list.size();i++)
        {
            //qDebug() << rx_list[i];
            SearchNode *p = new SearchNode;
            p->word = node.word;
            //p->file = atoi(cm[i].str().c_str());
            p->file = rx_list[i].toInt();

            Result *q = new Result;
            q->file = p->file;
            //qDebug() << QString::number(q->file,10);
            if(resultHead == NULL)
            {
                if(a == 0)
                { a++; resultHead = q; resultTail = q; q->next = NULL; h = resultHead;}
                else
                {h->next= q; resultTail = q; q->next = NULL;}
            }
            else
            {
                resultTail->next = q; resultTail = q; resultTail->next = NULL;
            }
            //qDebug() << QString::number(p->file,10);
            //qDebug() << rx_list[i];
            if (head_ == NULL)
            {
                head_ = p; tail_ = p; head_->next = NULL;
            }
            else
            {
                tail_->next = p; tail_ = p; tail_->next = NULL;
            }
        }
        //qDebug() << (head_->next == NULL);
        SearchNode *head = head_;

        if(QFile::exists("C:/Users/kylin/Desktop/Search/out.txt"))
            QFile::remove("C:/Users/kylin/Desktop/Search/out.txt");
        while (head != NULL)
        {
            int begin = 0, end = 1430;
            while (begin <= end)
            {
                int mid = (begin + end) / 2;
                if (IndexList[mid].num == head->file)
                {
                    //qDebug() << IndexList[mid].file << node.word;
                    SearchInSource(IndexList[mid].file.replace(".new", ".txt"), node.word);
                    head = head->next;
                    if(head == NULL)
                        break;
                }
                else if (head->file > IndexList[mid].num)
                    begin = mid + 1;
                else if (head->file < IndexList[mid].num)
                    end = mid - 1;
            }
            //qDebug("xxxxx");
        }

        return true;
    }

    void SearchInSource(QString filename, QString word)
    {

        QString file = "C:\\Users\\kylin\\Desktop\\Search\\source\\" + filename;
        //ofstream outfile("SearchResult.txt");
        //ifstream infile(file.toStdString().c_str());
        QString buf;
        /*QFile outfile("C:\\Users\\kylin\\Desktop\\Search\\out.txt");
        outfile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream outfile_stream(&outfile);
        outfile_stream.setCodec("UTF-8");*/

        QFile infile(file);
        //qDebug() << file;

        if(!infile.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug() << "open fail " + file;


        QTextStream infile_stream(&infile);
        infile_stream.setCodec("UTF-8");
        //qDebug() << infile_stream.readLine();
        int count = 0;
        while(!infile_stream.atEnd())
        {
            count++;
            buf = infile_stream.readLine();
            //qDebug() << buf;
            Find A(buf, word);
            //qDebug() << buf;
            A.getnext();
            int a = A.fastfind();
            if (a != -1){
                //qDebug() << QString::number(count,10);
                h->line = count;
                h->word = word;
                h->content = buf;
                QRegExp rx("(\\d+) (\\d+) (.*) href = \"(.*)\" title = \"(.*)\" author = \"[{].*[}]\" content = \"(.*)\"");
                rx.setMinimal(true);
                rx.indexIn(buf);
                //qDebug() << buf;
                h->PageRank = rx.cap(3).toDouble();
                //qDebug() << rx.cap(3);
                if(h->next != NULL)
                    h = h->next;
                //qDebug() << QString::number(a,10);
                //outfile_stream << buf + "\n";
            }                           //读出来一条直接就输出到文件，最后做排序
        }
        infile.close();
    }
    void sort(){
        QFile outfile("C:\\Users\\kylin\\Desktop\\Search\\out.txt");
        outfile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream outfile_stream(&outfile);
        outfile_stream.setCodec("UTF-8");
        Result *t2 = resultHead;
        //Result *t2 = NULL;
         //Result *t2t = NULL;
        if(t2 == NULL)
           {  outfile.close();return;}
        Result *head2 = BoolSearch(t2, OR_INIT);
        //head1 = DivResultSort(head1);
        head2 = DivResultSort(head2);
        //qDebug() << t->content + "sfdsfds";
        /*while(head1)
        {
            //qDebug() << t->content;
            outfile_stream << head1->content + "\n";
            head1 = head1->next;
        }*/
        while(head2)
        {
            outfile_stream << head2->content + "\n";
            head2 = head2->next;
        }
        outfile.close();
    }

};
