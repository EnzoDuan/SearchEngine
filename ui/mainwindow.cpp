#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <qdebug.h>
#include <globel.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    page = 0;
    page_max = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exec_line(){
    //QString patternStr = "href = \"(.*)\" title = \"(.*)\" author = \"[{]\'intro\': \'\\\\n(.*)\\\\n\', \'author\': \'(.*)\', \'href\': \'(.*)\'[}]\" content = \"(.*)\"";
    QString patternStr = "(\\d+) (\\d+) (.*) href = \"(.*)\" title = \"(.*)\" author = \"[{].*[}]\" content = \"(.*)\"";
    QRegExp rx(patternStr);
    QRegExp href("\'href\': \'(.*)\'");
    QRegExp intro("\'intro\': \'\\\\n(.*)\\\\n\'");
    QRegExp author("\'author\': \'(.*)\'");

    rx.setMinimal(true);
    href.setMinimal(true);
    intro.setMinimal(true);
    author.setMinimal(true);
    ui->textBrowser->clear();
    ui->textBrowser->setOpenExternalLinks(true);
    for(int i = 0; i < 5; i++){
        if(page+i>=page_max)
            break;
        rx.indexIn(content_list[page+i]);
        href.indexIn(content_list[page+i]);
        intro.indexIn(content_list[page+i]);
        author.indexIn(content_list[page+i]);
        ui->textBrowser->append("<hr><a href=\""+rx.cap(4)+"/\">"+
                                "<span style=\"color:#003399;font-size:25px;\">"+rx.cap(5)+"</span>"+    //title
                                "</a><br>"+
                                "<a href=\""+href.cap(1)+"/\">"+
                                "<span style=\"color:#000000;\"><strong>"+author.cap(1)+"</strong></span>"+    //author page
                                "</a> "+
                                "<span>, </span>"
                                "<a href=\""+href.cap(1)+"/\">"+
                                "<span style=\"color:#999999;\">"+intro.cap(1)+"</span>"+
                                "</a> "+
                                "<br>"+
                                rx.cap(6)+                //content
                                "<br>"+
                                "<span style=\"font-size:12px;color:#EE33EE;\">"+"in-degree:"+rx.cap(2)+"</span>"+" "
                                "<span style=\"font-size:12px;color:#EE33EE;\">"+"pagerank-value:"+rx.cap(3)+"</span>"
                           );
    }
    ui->textBrowser->moveCursor(QTextCursor::Start);
    ui->label_2->setText(QString::number(page+1,10)+'/'+QString::number(page_max,10));
}

void MainWindow::on_searchButton_clicked()
{
    content_list.clear();
    QString searchText =  ui->lineEdit->text();
    QFile file("C:\\Users\\kylin\\Desktop\\Search\\pre_in.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        exit(0);//打开写入文件失败
    }
    QTextStream outdata(&file);
    outdata.setCodec("UTF-8");
    outdata << searchText;
    file.close();
    QProcess p(0);
    p.start("python C:\\Users\\kylin\\Desktop\\Search\\in_jieba.py");
    p.waitForStarted();
    p.waitForFinished();

    BooleanRetrieval BR;
    //qDebug() << "br";
    BR.Search();
    //qDebug() << BR.resultHead->content;
    BR.sort();

    QFile file2("C:\\Users\\kylin\\Desktop\\Search\\out.txt");
    //QFile file2("C:\\Users\\kylin\\Desktop\\build-qt-Qt-Debug\\SearchResult.txt");
    if (!file2.open(QIODevice::ReadOnly | QIODevice::Text)){
        exit(0);//打开读入文件失败
    }
    QTextStream in(&file2);
    in.setCodec("UTF-8");
    while(!in.atEnd()){
        QString contents = in.readLine();
        //qDebug() << contents;
        content_list.append(contents);
    }
    page_max = content_list.size();
    if(content_list[0] == ""){
        QString error_msg = "No result....";
        ui->textBrowser->setText(error_msg);
    }
    else{
        ui->label_2->setText(QString::number(page_max, 10));
        page = 0;
        exec_line();
    }
}



void MainWindow::on_up_page_button_clicked()
{
    if(page >= 5)
        page -=5;
    else
        page = 0;
    ui->label_2->setText(QString::number(page,10));
    exec_line();

}

void MainWindow::on_next_page_button_clicked()
{
    if(page + 5 < page_max)
        page += 5;
    exec_line();
}

/*void MainWindow::on_pushButton_clicked()
{
    QString searchText =  ui->lineEdit->text();
    QFile file("C:\\Users\\kylin\\Desktop\\qt_src\\BTfileIndex.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        exit(0);
    }
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString lineText;
    //一行行读取文件
    do{
      lineText = in.readLine();
      //匹配正则表达式
      QString patternStr = "(\\d+) first = [{]word = \"(.*)\" PageRank = \"(.*)\"[}] last = [{]word = \"(.*)\" PageRank = \"(.*)\"[}]";
      QRegExp rx(patternStr);
      rx.setMinimal(true);
      if(rx.indexIn(lineText) == -1){
          ui->label_3->setText("error");
      }
      if(searchText >= rx.cap(2) && searchText <= rx.cap(4)){
        //ui->label->setText(rx.cap(1)+'\n'+rx.cap(2)+'\n'+rx.cap(3)+'\n'+rx.cap(4)+'\n'+rx.cap(5));
        QString filepath = "C:\\Users\\kylin\\Desktop\\qt_src\\BTfile\\BTfile"+rx.cap(1)+".txt";
        inFileSearch(filepath, searchText);
      }
    }while(!in.atEnd());
    //ui->label->setText(text);
}

void MainWindow::inFileSearch(QString filepath, QString searchText)
{
    QFile file2(filepath);
    if (!file2.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->label_2->setText("error!");
    }
    QTextStream in2(&file2);
    in2.setCodec("UTF-8");
    //ui->label_2->setText("OK"+filepath);
    QString lineText;
    do{
        lineText = in2.readLine();
        QString patternStr = "word = \"(.*)\" PageRank = \"(.*)\"";
        QRegExp rx(patternStr);
        rx.setMinimal(true);
        if(rx.indexIn(lineText) == -1){
            ui->label_3->setText("error2");
        }
        if(searchText == rx.cap(1)){
            QString pageRanks_str = rx.cap(2);
            ui->label->setText(rx.cap(1) + '\n');
            QStringList pageRanks_list = pageRanks_str.split(" ");
            for(int i = 0; i < pageRanks_list.size(); i++){

            }
        }
    }while(!in2.atEnd());

}*/




