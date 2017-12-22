#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>
#include "stdafx.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QMessageBox>

using namespace std;

class MainWindow : public QWidget
{
    Q_OBJECT
    QTextEdit *list;
    QPushButton *btn;
    QPushButton *b1;
    QGridLayout *grid;
    QLineEdit *line;

    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;

public:
    explicit MainWindow(QWidget *parent = 0);
    static int getCommandLine(const char *command, char* result);
signals:

public slots:
    void releaseList();
    void burn();
private slots:
    void changeEvent(QEvent*);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void trayActionExecute();
    void setTrayIconActions();
    void showTrayIcon();

};

#endif // MAINWINDOW_H
