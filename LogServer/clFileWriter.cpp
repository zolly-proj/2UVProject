#include "clFileWriter.h"
clFileWriter::clFileWriter(QString paFilePath,QString paFileSize,QString paDiskUsage)
{
    try
    {
        setCurrentFileName(QString(""));
        setPath(paFilePath);
        setSize(paFileSize);
        setDiskUsage(paDiskUsage);
    }
    catch(...)
    {
        cout << "clFileWriter::clFileWriter(QString paConfigFile) -> error ...";
    }
}

clFileWriter::~clFileWriter ()
{
}

bool clFileWriter::writeMessageToLogFile(QString paLogNumber,QString paComputerName, QString paApplicationName,QString paApplicationText)
{
    try
    {
        //Check the file name, if not there create file
        if (getCurrentFileName().compare(QString("")) == 0)
        {
            if (!createNewFile(meCurrentFile))
            {
                cout << "clFileWriter::createNewFile() -> error ...";
                return false;
            }
        }
        //Check file size, if nok create new file //TODO
        bool loFreeSize;
        if (!checkFileSize(loFreeSize))
        {
            cout << "clFileWriter::checkFileSize() -> error ...";
            return false;
        }
        if(!loFreeSize)
        {
            if (!createNewFile(meCurrentFile))
            {
                cout << "clFileWriter::createNewFile() -> error ...";
                return false;
            }
        }
        //Check disk size, if nok do not write any more //TODO
        //bool loFreeSpace;
        //if(!checkDiskSpace(loFreeSpace))
        //{
        //    cout << "clFileWriter::checkDiskSpace() -> error ...";
        //    return false;
        //}
        //if (!loFreeSpace)
        //{
        //    cout << "clFileWriter::writeMessageToLogFile() -> not enough space ...";
        //    return false;
        //}

        //Get the time stamp
        QDateTime loDateTimeNow = QDateTime::currentDateTime();
        QString loTimeStamp(loDateTimeNow.toString("dd.MM.yyyy hh:mm:ss:zzz"));

        //Write to the log file
        QFile loFile(QString(getPath() + meCurrentFile));
        if (loFile.open(QFile::Append )) {
            QTextStream out(&loFile);
            out << QString("BBEEBBEE") + loTimeStamp + QString("BBEE") + paLogNumber + QString("BBEE") + paComputerName + QString("BBEE") + paApplicationName + QString("BBEE") + paApplicationText + QString("BBEEBBEE") << "\n";
            return true;
        }
        else
        {
            cout << "clFileWriter::writeMessageToLogFile() -> Could not open the file ...";
            return false;
        }
    }
    catch(...)
    {
        cout << "clFileWriter::writeMessageToLogFile(QString paLogNumber,QString paApplicationName,QString paApplicationText) -> error ...";
        return false;
    }
}
bool clFileWriter::checkFileSize(bool &paFreeSize)
{
    try
    {
        QFile loFile(mePath + meCurrentFile);
        //CHECK WICH VALUE SHOULD BE INTERED
        qint64 loTemp;
        loTemp = loFile.size();
        QString loSize = QString::number(loTemp);
        /*
            QByteArray loBa = loSize.toLatin1();
            char *loString = loBa.data();

            cout << "This is the string '";
            cout << loString << endl;
        */
        if (loSize.toDouble() > getSize().toDouble())
        {
            paFreeSize = false;
        }
        else
        {
            paFreeSize = true;
        }
        return true;
    }
    catch(...)
    {
        cout << "clFileWriter::checkFileSize() -> error ...";
        return false;
    }
}
bool clFileWriter::createNewFile(QString &paFileName)
{
    try
    {
        QDateTime loDateTimeNow = QDateTime::currentDateTime();
        paFileName = loDateTimeNow.toString("ddMMyyyyhhmmsszzz");
        return true;
    }
    catch(...)
    {
        cout << "clFileWriter::createNewFile() -> error ...";
        return false;
    }
}
bool clFileWriter::checkDiskSpace(bool &paFreeSpace)
{
    try
    {
        QStringList loStringList = getPath().split ( "/");

        QFileInfo sizeInfo(loStringList.at(0) + "/");
        int size = sizeInfo.size();

        QString temp(size);

        QByteArray loBa = temp.toLatin1();
        char *loString = loBa.data();

        cout << "The size is '";
        cout << loString;

        paFreeSpace = true;
        return true;
    }
    catch(...)
    {
        cout << "clFileWriter::checkDiskSpace() -> error ...";
        return false;
    }
}
bool clFileWriter::writeMessage(QString paMessage)
{
    try
    {
        return true;
    }
    catch(...)
    {
        cout << "clFileWriter::writeMessage() -> error ...";
        return false;
    }
}

//Getters and setters
void clFileWriter::setPath(QString paPath){ mePath = paPath;}
QString clFileWriter::getPath(){return mePath;}
void clFileWriter::setSize(QString paSize){meSize = paSize;}
QString clFileWriter::getSize(){return meSize;}
void clFileWriter::setDiskUsage(QString paDiskUsage){meDiskUsage = paDiskUsage;}
QString clFileWriter::getDiskUsage(){return meDiskUsage;}
void clFileWriter::setCurrentFileName(QString paCurrentFile){meCurrentFile = paCurrentFile;}
QString clFileWriter::getCurrentFileName(){return meCurrentFile;}

