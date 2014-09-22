#include "worker.h"

Worker::Worker(QObject *parent, QString applicationSourcePath, QString upgradeSourcePath) : QThread(parent)
{
    setApplicationSourcePath(applicationSourcePath);
    setUpgradeSourcePath(upgradeSourcePath);
    stopped = false;
    setProgress(0);
}

void Worker::setApplicationSourcePath(QString applicationSourcePath)
{
   m_applicationSourcePath = applicationSourcePath;
}

void Worker::setUpgradeSourcePath(QString upgradeSourcePath)
{
   m_upgradeSourcePath = upgradeSourcePath;
}

void Worker::run(void)
{
    QDir dir(m_upgradeSourcePath);
    if (!dir.exists())
    {
        m_errorMsg.append("Error the upgrade source folder does not exist:").append("\n").append(m_upgradeSourcePath);
        emit errorMsgChanged(m_errorMsg);
        stop();
    }
    else
    {
        if (removeFolder(m_applicationSourcePath) && copyFolder(m_upgradeSourcePath, m_applicationSourcePath))
        {
            stop();
        }
        else
        {
            emit errorMsgChanged(m_errorMsg);
            stop();
        }
    }
}

void Worker::stop()
{
    stopped = true;
}


void Worker::setProgress(int progress)
{
    m_progress = progress;
    emit valueChanged(progress);
}

bool Worker::removeFolder(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    qDebug() << "removing files from " << dirName << "\n";

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
                m_errorMsg = "Error removing file: " +  info.absoluteFilePath();
                return result;
            }
        }
        result = dir.rmdir(dirName);
        if (!result) {
            m_errorMsg = "Error removing folder: " + dirName;
            return result;
        }
    }

    return result;
}

bool Worker::copyFolder(QString sourceFolder, QString destFolder)
{
    bool result = true;

    QDir sourceDir(sourceFolder);
    if(!sourceDir.exists())
    {
        m_errorMsg = "Error the source folder does not exist: " + sourceFolder;
        return false;
    }

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
        result = QFile::copy(srcName, destName);
        if (!result)
        {
            m_errorMsg = "Error copying file: " + srcName;
            return result;
        }
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

    return result;
}



