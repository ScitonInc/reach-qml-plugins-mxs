#include "sqlite.h"

#include <qdeclarative.h>

sqlite::sqlite(QDeclarativeItem *parent):
    QDeclarativeItem(parent)
{
    // By default, QDeclarativeItem does not draw anything. If you subclass
    // QDeclarativeItem to create a visual item, you will need to uncomment the
    // following line:
    
    // setFlag(ItemHasNoContents, false);
}

sqlite::~sqlite()
{

}

bool sqlite::openDB()
{
    bool ret = false;
    //Check if we need to restore the DB
    restoreDB();

    // Find QSLite driver
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "reach_connection"));
    QString path(DATABASE_PATH);

    //Check if path exists.  If not we have to create it
    QDir dir = QDir::root();
    if (!dir.exists(path))
    {
        dir.mkpath(path);
    }

    path.append(QDir::separator()).append(DATABASE_NAME);
    db->setDatabaseName(path);
    ret = db->open();

    //test if the database is not corrupted

    QSqlQuery* query = new QSqlQuery(db->database("reach_connection"));
    query->exec("select sqlite_version()");

    if (ret && query->next())
    {
        //Create settings table if it does not exist
        qDebug() << "Sqlite database open: " << path;
        //ret = createSettingsTable();
    }
    else
    {
        qDebug() << "Sqlite database could not be open."  << "  File may be corrupted: " << path;
    }
    query->finish();
    delete query;

    return ret;
}

bool sqlite::createSettingsTable()
{
    bool ret = false;
    if (db->isOpen())
    {
        {
            QSqlQuery* query = new QSqlQuery(db->database("reach_connection"));
            ret = query->exec("CREATE TABLE IF NOT EXISTS settings(setting TEXT UNIQUE, value TEXT)");
            query->finish();
            delete query;
        }
    }
    return ret;
}


bool sqlite::setSetting(QString setting, QString value)
{
    bool ret = false;
    if (db->isOpen())
    {
        {
            QSqlQuery* query = new QSqlQuery(db->database("reach_connection"));
            query->prepare("INSERT OR REPLACE INTO settings VALUES (?,?)");
            query->addBindValue(setting);
            query->addBindValue(value);
            ret = query->exec();
            delete query;
            backUpFlag = true;
        }
    }
    return ret;
}

QString sqlite::getSetting(QString setting)
{
    QString ret = "";
    if (db->isOpen())
    {
        {
            QSqlQuery* query = new QSqlQuery(db->database("reach_connection"));
            query->prepare("SELECT value FROM settings WHERE setting=?");
            query->addBindValue(setting);
            query->exec();
            if (query->next())
                ret = query->value(0).toString();
            delete query;
        }
    }
    return ret;
}

QVariantList sqlite::getRows(QString sql)
{
    QVariantList rows;
    if (db->isOpen())
    {
        {
            QSqlQuery* query = new QSqlQuery(db->database("reach_connection"));
            query->exec(sql);
            while (query->next())
            {
                QSqlRecord record = query->record();
                int cols = record.count();
                QStringList list;
                for( int c=0; c<cols; c++ )
                {
                    list.append(record.value(c).toString());
                }
                rows.append(list);
            }
            delete query;
        }
    }
    return rows;
}

int sqlite::execSql(QString sql)
{
    int ret = 0;
    if (db->isOpen())
    {
        {
            QSqlQuery* query = new QSqlQuery(db->database("reach_connection"));
            query->exec(sql);
            ret = query->numRowsAffected();
            if (ret > 0)
                backUpFlag = true;
            delete query;
        }
    }
    return ret;
}


int sqlite::execSql(QString sql, QStringList arguments)
{
    int ret = 0;
    if (db->isOpen())
    {
        {
            QSqlQuery* query = new QSqlQuery(db->database("reach_connection"));
            query->prepare(sql);
            foreach (const QString &str, arguments)
            {
                query->addBindValue(str);
            }

            query->exec();
            ret = query->numRowsAffected();
            if (ret > 0)
                backUpFlag = true;
            delete query;
        }
    }
    return ret;
}

QString sqlite::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db->lastError().text();
}

void sqlite::closeDB()
{
    if (db->isOpen())
    {
        // Close and backup database
        if (backUpFlag)
            backupDB();
        db->close();
        delete db;
        QStringList list = QSqlDatabase::connectionNames();
        for(int i = 0; i < list.count(); ++i) {
            QSqlDatabase::removeDatabase(list[i]);
            qDebug() << "database removed: " << list[i];
        }
    }
}

void sqlite::backupDB()
{
    if (db->isOpen())
    {
        QSqlQuery* query = new QSqlQuery(db->database("reach_connection"));
        query->exec("select sqlite_version()");
        if (query->next())
        {
            query->prepare( "BEGIN IMMEDIATE;");
            query->exec();

            QString source(DATABASE_PATH);
            source.append(QDir::separator()).append(DATABASE_NAME);

            QString destination(DATABASE_PATH);
            destination.append(QDir::separator()).append(DATABASE_BACKUP_NAME);

            if (QFile::exists(destination))
                QFile::remove(destination);

            if (QFile::copy(source, destination))
                qDebug() << "Sqlite database was backed up.";

            query->prepare( "ROLLBACK;");
            query->exec();
            delete query;
        }
    }
}

bool sqlite::restoreDB()
{
    bool ret = false;
    QString source(DATABASE_PATH);
    source.append(QDir::separator()).append(DATABASE_NAME);

    QString destination(DATABASE_PATH);
    destination.append(QDir::separator()).append(DATABASE_BACKUP_NAME);

    //Check if both files exist
    if (QFile::exists(source) && QFile::exists(destination))
    {
        QCryptographicHash sourceHash( QCryptographicHash::Sha1 );
        QFile sourceFile(source);
        if ( sourceFile.open( QIODevice::ReadOnly ) ) {
            sourceHash.addData( sourceFile.readAll() );
        } else {
            qDebug() << "Could not open db: " << source;
            return false;
        }
        QByteArray sourceSig = sourceHash.result();

        QCryptographicHash destinationHash( QCryptographicHash::Sha1 );
        QFile destinationFile(destination);
        if ( destinationFile.open( QIODevice::ReadOnly ) ) {
            destinationHash.addData( destinationFile.readAll() );
        } else {
            qDebug() << "Could not open db: " << destination;
            return false;
        }
        QByteArray destinationSig = destinationHash.result();

        if (memcmp(sourceSig, destinationSig, sizeof(sourceSig)) != 0)
        {
            QFile::remove(source);
            QFile::copy(destination, source);
            qDebug() << "Sqlite database: " << source << " was restored.";
            ret = true;
        }
        else
        {
            qDebug() << "Sqlite database: " << source << " did not need to be restored.";
            ret = false;
        }

    }

    return ret;
}
