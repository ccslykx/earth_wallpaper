#include "trayicon.h"
#include "config.h"
#include "thread.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QScreen>
#include <QStandardPaths>
#include <QMap>

TrayIcon::TrayIcon(QSystemTrayIcon *parent) : QSystemTrayIcon(parent)
{
    // 获取屏幕分辨率
    QScreen *desktop = QApplication::primaryScreen();
    QRect desktopRect = desktop->availableGeometry();
    this->height = desktopRect.height() + desktopRect.top();
    this->width = desktopRect.width() + desktopRect.left();
    initTrayIcon();
    initConnect();
    checkConfig();
}
TrayIcon::~TrayIcon()
{
    config->deleteLater();
    exit->deleteLater();
    trayIconMenu->deleteLater();
}
void TrayIcon::initConnect()
{
    // 退出程序
    connect(this->exit, &QAction::triggered, this, &TrayIcon::OnExit);
    // 打开设置页面
    connect(this->config, &QAction::triggered, this, &TrayIcon::showConfigPage);
    // 打开关于页面
    connect(this->about, &QAction::triggered, this, &TrayIcon::showAboutPage);
    // 刷新壁纸
    connect(this->refresh, &QAction::triggered, this, &TrayIcon::handle);
    // 保存壁纸
    connect(this->save, &QAction::triggered, this, &TrayIcon::saveCurrentImg);
    // 定时器
    connect(&this->timer, &QTimer::timeout, this, &TrayIcon::handle);
}
void TrayIcon::initTrayIcon()
{
    this->setIcon(QIcon(":/img/cn.huguoyang.earthwallpaper.png"));

    trayIconMenu = new QMenu();
    config = new QAction();
    exit = new QAction();
    refresh = new QAction();
    save = new QAction();
    about = new QAction();
    /* Todo
     *   Translation
     */
    config->setText("设置");
    refresh->setText("更新壁纸");
    save->setText("保存当前壁纸");
    exit->setText("退出");
    about->setText("关于");

    trayIconMenu->addAction(save);
    trayIconMenu->addAction(refresh);
    trayIconMenu->addAction(config);
    trayIconMenu->addAction(about);
    trayIconMenu->addAction(exit);

    this->setContextMenu(trayIconMenu);

    this->show();
}

void TrayIcon::OnExit()
{
    qApp->exit(0);
}

void TrayIcon::showConfigPage()
{
    if (!configPage) { // 没有if会创建多个实例
        this->configPage = new Config();
    }
    configPage->show();

    // 重载设置
    connect(this->configPage, &Config::configChanged, this, &TrayIcon::reloadSettings);
    connect(configPage, &Config::closed, this, [&](){
        disconnect(this->configPage, &Config::configChanged, this, &TrayIcon::reloadSettings);
        configPage->deleteLater();
        this->configPage = nullptr;
    });
}

void TrayIcon::showAboutPage()
{
    if (!aboutPage) { // 没有if会创建多个实例
        this->aboutPage = new About();
    }
    aboutPage->show();
    aboutPage->setAttribute(Qt::WA_DeleteOnClose);
    connect(aboutPage, &About::closed, this, [&](){
        aboutPage->deleteLater();
        this->aboutPage = nullptr;
    });
}

void TrayIcon::checkConfig()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    path += "/earth-wallpaper/config";
    QFile configFile = QFile(path);
    if (!configFile.exists())
    {
        qInfo() << "首次运行，打开设置页面";
        this->showConfigPage();
    }
    else
    {
        reloadSettings();
    }
}

void TrayIcon::reloadSettings()
{
    timer.stop();
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    configPath += "/earth-wallpaper/config";
    if (this->settings) {
        this->settings->deleteLater();
    }
    // 重新读取配置文件
    this->settings = new QSettings(configPath, QSettings::IniFormat);
    handle();
}
void TrayIcon::handle()
{
    qDebug() << "handling...";

    settings->beginGroup("APP");
    settings->setIniCodec("UTF8");
    QString earthSource = settings->value("earthSource").toString();
    QString earthSize = settings->value("earthSize").toString();
    QString wallpaperDir = settings->value("wallpaperDir").toString();
    QString wallpaperFile = settings->value("wallpaperFile").toString();
    settings->endGroup();

    settings->beginGroup("System");
    QMap<int,QString> proxyMap;
    proxyMap.insert(0,"None");
    proxyMap.insert(1,"http://"+settings->value("proxyAdd").toString()+":"+settings->value("proxyPort").toString());
    proxyMap.insert(2,"socks5://"+settings->value("proxyAdd").toString()+":"+settings->value("proxyPort").toString());
    settings->endGroup();

    QStringList command = QStringList();
    QString exePath = QCoreApplication::applicationDirPath();
    QDir scriptsDir(exePath + "/scripts");
    QStringList nameFilters;
    nameFilters << "*.py";
    scriptsDir.setNameFilters(nameFilters);
    QStringList files = scriptsDir.entryList(QDir::Files, QDir::Name);
    // qDebug() << earthSource;
    foreach (QString file, files)
    {
        QFile scriptFile(exePath + "/scripts/" + file);
        scriptFile.open(QIODevice::ReadOnly);
        QString info = scriptFile.readLine();
        if (info.split(" ")[1] == "source:")
        {

            // qDebug() << info.split(" ")[2].remove("\n");
            if (info.split(" ")[2].remove("\n") == earthSource)
            {
                command.append(exePath + "/scripts/" + file);
                command.append(QString::number(this->height));
                command.append(QString::number(this->width));
                command.append(earthSize);
                command.append(wallpaperDir);
                command.append(wallpaperFile);
                command.append(proxyMap[settings->value("System/proxy").toInt()]);
                if (file == "24h.py")
                {
                    settings->setValue("APP/updateTime", "10");
                }
            }
        }
    }

    // 根据设置下载、更新壁纸
    qDebug() << command;
    Thread *thread = new Thread(command);
    thread->start();
    connect(thread, &Thread::finished, thread, &Thread::deleteLater); // 自断经脉
    timer.start(60000 * settings->value("APP/updateTime").toInt());
}
void TrayIcon::saveCurrentImg()
{
    QString dirPath; // 缓存目录
    if (QSysInfo::productType() == "windows") { // 修复点击“保存当前壁纸”闪退的Bug
        dirPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/AppData/Roaming/earth-wallpaper/wallpaper/";
    } else {
        dirPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.cache/earth-wallpaper/wallpaper/";
    }
    QDir dir(dirPath);
    QStringList nameFilters;
    nameFilters << "[1-9]*";
    dir.setNameFilters(nameFilters);
    QStringList files = dir.entryList(QDir::Files, QDir::Name);
    QString picturePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/earth-wallpaper";
    if (!QDir(picturePath).exists())
    {
        QDir(picturePath).mkpath(picturePath);
    }

    QFile source = QFile(dirPath + files[0]); // changed from target to "source"
    QFile target = QFile(picturePath + "/" + files[files.count() - 1]);
    if (!target.exists()) {
        if (source.copy(target.fileName()))
        {
            QMessageBox::information(nullptr, "保存", "保存成功，已保存到用户Picture目录", QMessageBox::Yes);
        } else {
            QMessageBox::warning(nullptr, "保存", "保存失败，原因未知（懒）", QMessageBox::Yes);
        }
    } else {
        QMessageBox::information(nullptr, "保存", "当前壁纸已存在，未做任何处理", QMessageBox::Yes);
    }

}
