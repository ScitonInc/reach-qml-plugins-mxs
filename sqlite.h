#ifndef SQLITE_H
#define SQLITE_H

#include <QDeclarativeItem>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlRecord>
#include <QFile>
#include <QDir>
#include <QCryptographicHash>

#define DATABASE_NAME "settings.sqlite"
#define DATABASE_BACKUP_NAME "settingsbak.sqlite"
#define DATABASE_PATH "/application/databases"

class sqlite : public QDeclarativeItem
{
    Q_OBJECT
    Q_DISABLE_COPY(sqlite)
    
public:
    sqlite(QDeclarativeItem *parent = 0);
    ~sqlite();

    Q_INVOKABLE bool openDB();
    Q_INVOKABLE void closeDB();
    Q_INVOKABLE QString lastError();
    Q_INVOKABLE QString getSetting(QString setting);
    Q_INVOKABLE bool setSetting(QString setting, QString value);
    Q_INVOKABLE int execSql(QString sql);
    Q_INVOKABLE int execSql(QString sql, QStringList arguments);
    Q_INVOKABLE QVariantList getRows(QString sql);

private:
    QSqlDatabase* db;
    bool createSettingsTable();
    void backupDB();
    bool restoreDB();
    bool backUpFlag = false;

};

QML_DECLARE_TYPE(sqlite)

#endif // SQLITE_H

