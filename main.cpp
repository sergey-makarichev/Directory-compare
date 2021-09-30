#include <QTextStream>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QDataStream>

bool compareFiles(const QByteArray& data1, const QByteArray& data2) {
    for (int i = 0; i < data1.size(); ++i) {
        if(data1.at(i) != data2.at(i))
            return false;
    }
    return true;
}

void compareFolders(const QFileInfoList& listOfFiles1, const QFileInfoList& listOfFiles2) {
    QTextStream out(stdout);
    out << "duplicates:" << Qt::endl;
    for (int i = 0; i < listOfFiles1.size(); ++i) {
        QFileInfo fileInfo1 = listOfFiles1.at(i);
        QString str1 = fileInfo1.absoluteFilePath();
        for  (int j = 0; j < listOfFiles2.size(); ++j) {
            QFileInfo fileInfo2 = listOfFiles2.at(j);
            QString str2 = fileInfo2.absoluteFilePath();
            if(fileInfo1.size() == fileInfo2.size()) {
                QFile file1(str1);
                if (!file1.open(QIODevice::ReadOnly)) {
                    qWarning("Cannot open file for reading");
                    exit(1);
                }
                QFile file2(str2);
                if (!file2.open(QIODevice::ReadOnly)) {
                    qWarning("Cannot open file for reading");
                    exit(1);
                }
                QByteArray data1 = file1.readAll();
                QByteArray data2 = file2.readAll();
                if(compareFiles(data1, data2)) {
                    out << fileInfo1.fileName()<<" - "<<fileInfo2.fileName()<<Qt::endl;
                }
            }
        }
    }
}
int main(int argc, char *argv[])
{
    if (argc != 3) {
      qWarning("The number of arguments should be 3");
      return 1;
    }
    QString path1 = argv[1];
    QString path2 = argv[2];
    QDir directory1(path1);
    QDir directory2(path2);
    if (!(directory1.exists() && directory2.exists())) {
        qWarning("The directory does not exist");
        return 1;
    }
    directory1.setFilter(QDir::Files);
    directory2.setFilter(QDir::Files);
    QFileInfoList listOfFiles1 = directory1.entryInfoList();
    QFileInfoList listOfFiles2 = directory2.entryInfoList();
    compareFolders(listOfFiles1, listOfFiles2);

    return 0;
}
