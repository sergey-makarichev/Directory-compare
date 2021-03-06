#ifndef COMPAREFOLDERS_H
#define COMPAREFOLDERS_H
#include <QTextStream>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QList>
#include <QDirIterator>

class CompareFolders
{
private:
    QString path1, path2;
    QFileInfoList listOfFiles1, listOfFiles2;
    std::vector<QFileInfoList> similarFiles;
    static void searchAllFiles(const QString& path, QFileInfoList& list);
public:
    CompareFolders(QString& p1, QString& p2);
    void compareTwoFolders();
    bool compareFiles(const QByteArray& data1, const QByteArray& data2, int leftIndex, int rightIndex);
    bool parallelCompareFiles(const QByteArray& data1, const QByteArray& data2);
    void printSimilar();
};

#endif // COMPAREFOLDERS_H
