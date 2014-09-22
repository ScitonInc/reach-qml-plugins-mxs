#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <QDirIterator>
#include <QFile>
#include <QDebug>

class Worker : public QThread
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0, QString applicationSourcePath = "", QString upgradeSourcePath = "");
    void stop();
    void setProgress(int progress);
    void setApplicationSourcePath(QString applicationSourcePath);
    void setUpgradeSourcePath(QString upgradeSourcePath);

signals:
    void valueChanged(int);
    void errorMsgChanged(QString);
    
public slots:

private:
    bool removeFolder(const QString & dirName);
    bool copyFolder(QString sourceFolder, QString destFolder);
    volatile bool stopped;
    int m_progress;
    QString m_errorMsg;
    QString m_applicationSourcePath;
    QString m_upgradeSourcePath;

protected:
    void run(void);
};

#endif // WORKER_H
