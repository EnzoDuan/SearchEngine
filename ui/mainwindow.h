#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_up_page_button_clicked();

    void on_searchButton_clicked();

    void on_next_page_button_clicked();

private:
    Ui::MainWindow *ui;
    int page;
    int page_max;
    QStringList content_list;
    void exec_line();
};

#endif // MAINWINDOW_H
