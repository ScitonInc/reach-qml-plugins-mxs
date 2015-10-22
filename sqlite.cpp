#include "sqlite.h"

SqLite::SqLite(QDeclarativeItem *parent):
    QDeclarativeItem(parent)
{
    // By default, QDeclarativeItem does not draw anything. If you subclass
    // QDeclarativeItem to create a visual item, you will need to uncomment the
    // following line:
    
    // setFlag(ItemHasNoContents, false);
    backUpFlag = false;
}

SqLite::~SqLite()
{

}

bool SqLite::openDB()
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
        qDebug() << "Sqlite database open: " << path;
    }
    else
    {
        qDebug() << "Sqlite database could not be open."  << "  File may be corrupted: " << path;
    }
    query->finish();
    delete query;

    return ret;
}

QVariantList SqLite::getRows(QString sql)
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

int SqLite::execSql(QString sql)
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


int SqLite::execSql(QString sql, QStringList arguments)
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

QString SqLite::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db->lastError().text();
}

void SqLite::closeDB()
{
    QProcess p(this);
    QString cmd(SYNC_CMD);

    if (db->isOpen())
    {
        // Close and backup database
        if (backUpFlag)
            backupDB();
        db->close();

        /* call sync to flush disk cache */
        p.start(cmd);
        p.waitForFinished(1000);
        delete db;
        QStringList list = QSqlDatabase::connectionNames();
        for(int i = 0; i < list.count(); ++i) {
            QSqlDatabase::removeDatabase(list[i]);
            qDebug() << "database removed: " << list[i];
        }
    }
}

void SqLite::backupDB()
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

bool SqLite::restoreDB()
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
