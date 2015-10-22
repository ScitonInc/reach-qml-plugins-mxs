#ifndef SQLITE_H
#define SQLITE_H

#include <QDeclarativeItem>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlRecord>
#include <QFile>
#include <QDir>
#include <QCryptographicHash>
#include <QDebug>
#include <QProcess>

#define DATABASE_NAME "settings.sqlite"
#define DATABASE_BACKUP_NAME "settingsbak.sqlite"
#define DATABASE_PATH "/application/databases"
#define SYNC_CMD "/bin/sync"

class SqLite : public QDeclarativeItem
{
    Q_OBJECT
    Q_DISABLE_COPY(SqLite)
    
public:
    SqLite(QDeclarativeItem *parent = 0);
    ~SqLite();

    Q_INVOKABLE bool openDB();
    Q_INVOKABLE void closeDB();
    Q_INVOKABLE QString lastError();
    Q_INVOKABLE int execSql(QString sql);
    Q_INVOKABLE int execSql(QString sql, QStringList arguments);
    Q_INVOKABLE QVariantList getRows(QString sql);

private:
    QSqlDatabase* db;
    void backupDB();
    bool restoreDB();
    bool backUpFlag;

};

#endif // SQLITE_H

