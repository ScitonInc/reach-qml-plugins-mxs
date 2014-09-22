#ifndef UPGRADE_H
#define UPGRADE_H

#include <QDeclarativeItem>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QProcess>
#include "worker.h"

class Upgrade : public QDeclarativeItem
{
    Q_OBJECT
    Q_DISABLE_COPY(Upgrade)
    
public:
    Upgrade(QDeclarativeItem *parent = 0);
    ~Upgrade();
    Q_INVOKABLE bool removeFolder(const QString & dirName);
    Q_INVOKABLE void copyFolder(QString sourceFolder, QString destFolder);
    Q_INVOKABLE int getTotalFileCount();
    Q_INVOKABLE int progress();
    Q_INVOKABLE QString errorMsg();
    Q_INVOKABLE bool upgrade();
    Q_INVOKABLE QString execute(QString command, QStringList args);
    Q_PROPERTY (QString upgradeSourcePath READ upgradeSourcePath WRITE setUpgradeSourcePath() NOTIFY upgradeSourcePathChanged)
    Q_PROPERTY (QString applicationSourcePath READ applicationSourcePath WRITE setApplicationSourcePath() NOTIFY applicationSourcePathChanged)

    QString upgradeSourcePath() const;
    void setUpgradeSourcePath(QString upgradeSourcePath);
    QString applicationSourcePath() const;
    void setApplicationSourcePath(QString applicationSourcePath);

signals:
    void progressChanged();
    void errorMsgChanged();
    void upgradeSourcePathChanged();
    void applicationSourcePathChanged();

private:
    int countFiles(const QString &path);
    int m_progress;
    QString m_errorMsg;
    QString m_upgradeSourcePath;
    QString m_applicationSourcePath;

private slots:
    void setProgress(int);
    void setErrorMsg(QString);
};

QML_DECLARE_TYPE(Upgrade)

#endif // UPGRADE_H

