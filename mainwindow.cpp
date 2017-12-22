#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(300, 300);
    list = new QTextEdit(this);

    btn = new QPushButton(this);
    btn->setText("Add");
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(releaseList()));

    b1 = new QPushButton(this);
    b1->setText("Burn");
    connect(b1, SIGNAL(clicked(bool)), this, SLOT(burn()));

    line = new QLineEdit(this);

    grid = new QGridLayout(this);
    grid->addWidget(line, 0, 0);
    grid->addWidget(list, 1, 0);
    grid->addWidget(btn, 2, 0);
    grid->addWidget(b1, 3, 0);


    this -> setTrayIconActions();
    this -> showTrayIcon();
}

void MainWindow::releaseList()
{
    //
    char *command = new char[1024];
    strcpy(command, "cp ");
    strcat(command, line->text().toStdString().c_str());
    strcat(command, " /home/vladiislav/Документы/iipu7/list");
    system(command);
    getCommandLine("cd /home/vladiislav/Документы/iipu7/list; dir", command);
    list->clear();
    list->setText(QString(command));
    delete(command);
}


void MainWindow::burn()
{
    this->setVisible(false);
    system("mkisofs -V volume_ID -D -l -L -N -J -R -v -o cdrom.iso /home/vladiislav/Документы/iipu7/list");
    system("umount /dev/cdrom");
    system("cdrecord -dev=/dev/cdrom -v blank=fast");
    system("cdrecord -dev=/dev/cdrom -speed=16 -eject -v cdrom.iso");
    system("rm -Rf /home/vladiislav/Документы/iipu7/list");
    system("mkdir /home/vladiislav/Документы/iipu7/list");
    trayIcon->showMessage("SUCCESS", "Burn finished");
}


void MainWindow::showTrayIcon()
{
    // Создаём экземпляр класса и задаём его свойства...
    trayIcon = new QSystemTrayIcon(this);
    QIcon trayImage("/home/vladiislav/Pictures/12.png");
    trayIcon -> setIcon(trayImage);
    trayIcon -> setContextMenu(trayIconMenu);

    // Подключаем обработчик клика по иконке...
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    // Выводим значок...
    trayIcon -> show();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            this -> setVisible(true);
            break;
        default:
            break;
    }
}

void MainWindow::setTrayIconActions()
{
    // Setting actions...
    minimizeAction = new QAction("Свернуть", this);
    restoreAction = new QAction("Восстановить", this);
    quitAction = new QAction("Выход", this);

    // Connecting actions to slots...
    connect (minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    connect (restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    connect (quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Setting system tray's icon menu...
    trayIconMenu = new QMenu(this);
    trayIconMenu -> addAction (minimizeAction);
    trayIconMenu -> addAction (restoreAction);
    trayIconMenu -> addAction (quitAction);
}

void MainWindow::changeEvent(QEvent *event)
{
    //QMainWindow::changeEvent(event);
    if (event -> type() == QEvent::WindowStateChange)
    {
        if (isMinimized())
        {
            this -> hide();
        }
    }
}

int MainWindow::getCommandLine(const char *command, char* result)
{
    result[0] = '\0';
    char* tmp = new char[256];
    FILE *file = popen(command, "r");
    if(!file)
    {
        cout << "Open error" << endl;
        return 0;
    }
    int i = 0;
    for(i = 0; fgets(tmp, 200, file); i++)
        strcat(result, tmp);
    pclose(file);
    delete(tmp);
    return i;
}

