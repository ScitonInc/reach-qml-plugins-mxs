#include "upgrade.h"
#include <qdeclarative.h>

Upgrade::Upgrade(QDeclarativeItem *parent):
    QDeclarativeItem(parent)
{
    // By default, QDeclarativeItem does not draw anything. If you subclass
    // QDeclarativeItem to create a visual item, you will need to uncomment the
    // following line:
    
    //setFlag(ItemHasNoContents, false);
    setProgress(0);
}


void Upgrade::setProgress(int progress)
{
    m_progress = progress;
    emit progressChanged();
}

void Upgrade::setErrorMsg(QString errorMsg)
{
    m_errorMsg = errorMsg;
    emit errorMsgChanged();
}

int Upgrade::progress()
{
    return m_progress;
}

QString Upgrade::errorMsg()
{
    return m_errorMsg;
}

void Upgrade::setUpgradeSourcePath(QString upgradeSourcePath)
{
    m_upgradeSourcePath = upgradeSourcePath;
}

QString Upgrade::upgradeSourcePath() const
{
    return m_upgradeSourcePath;
}

void Upgrade::setApplicationSourcePath(QString applicationSourcePath)
{
    m_applicationSourcePath = applicationSourcePath;
}

QString Upgrade::applicationSourcePath() const
{
    return m_applicationSourcePath;
}

bool Upgrade::removeFolder(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    qDebug() << "removing files from " << dirName;

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeFolder(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
                setProgress(m_progress += 1);
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    return result;
}

void Upgrade::copyFolder(QString sourceFolder, QString destFolder)
{
    QDir sourceDir(sourceFolder);
    QDir destDir(destFolder);
    if(!destDir.exists())
    {
        destDir.mkdir(destFolder);
    }

    qDebug() << "copying files from " << sourceFolder << "\n";

    QStringList files = sourceDir.entryList(QDir::Files);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = sourceFolder + "/" + files[i];
        QString destName = destFolder + "/" + files[i];
        QFile::copy(srcName, destName);
        setProgress(m_progress += 1);
    }
    files.clear();
    files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = sourceFolder + "/" + files[i];
        QString destName = destFolder + "/" + files[i];
        copyFolder(srcName, destName);
    }
}

int Upgrade::countFiles(const QString &path){
    int fileCount = 0;

    QDirIterator it(path, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        it.next();
        QFileInfo fi = it.fileInfo();

        if (fi.isFile())
        {
            fileCount++;
        }
    }

    qDebug() << "countFiles(): " + QString::number(fileCount);
    return fileCount;
}

int Upgrade::getTotalFileCount()
{
    int total = 0;
    total = countFiles(m_applicationSourcePath) + countFiles(m_upgradeSourcePath);
    qDebug() << "getTotalFileCount(): " + QString::number(total);
    return total;
}

bool Upgrade::upgrade()
{
    bool result = true;
    if (m_applicationSourcePath.length() == 0)
        m_applicationSourcePath = "/application";

    QDir sourceDir(m_upgradeSourcePath);
    if (m_upgradeSourcePath.length() > 0 && sourceDir.exists())
    {
        Worker *worker = new Worker(this, m_applicationSourcePath, m_upgradeSourcePath);
        connect(worker, SIGNAL(valueChanged(int)), this, SLOT(setProgress(int)));
        connect(worker, SIGNAL(errorMsgChanged(QString)), this, SLOT(setErrorMsg(QString)));

        worker->start();
        connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    }
    else
    {
        if (m_upgradeSourcePath.length() == 0)
        {
            setErrorMsg("Error: Property upgradeSourcePath is required.");
            qDebug() << "Error: Property upgradeSourcePath is required.";
        }
        else
        {
            setErrorMsg("Error: Property upgradeSourcePath \n'" + m_upgradeSourcePath + "'\ndoes not exist.");
            qDebug() << "Error: properties upgradeSourcePath '" << m_upgradeSourcePath <<  "' does not exist.";
        }
        result = false;
    }

    return result;
}


QString Upgrade::execute(QString command, QStringList args)
{
    QProcess p(this);

    qDebug() << "executing " << command << " with args " << args << "\n";

    p.start(command, args);
    p.waitForFinished(1000);

    QByteArray data(p.readAll());

   return QString::fromLatin1(data.data());

}

Upgrade::~Upgrade()
{
}

