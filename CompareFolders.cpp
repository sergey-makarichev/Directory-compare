#include "CompareFolders.h"
#include <thread>

CompareFolders::CompareFolders(QString& p1, QString& p2) {
    std::vector<std::thread> threads;
    for (int i = 0; i < 2; i++) {
        std::thread th([&p1, &p2, this](int i) {
            if (i == 0) {
                searchAllFiles(p1, listOfFiles1);
            } else {
                searchAllFiles(p2, listOfFiles2);
            }
            }, i);
        threads.emplace_back(std::move(th));
    }

    for (int i = 0; i < 2; i++)
        threads[i].join();
}

void CompareFolders::searchAllFiles(const QString& path, QFileInfoList& list) {
   QDirIterator it(path, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = it.fileInfo();
        list.push_back(info);
    }
}

void CompareFolders::compareTwoFolders() {
    int similarFilesId = 0;
    bool isSimilar = false;
    for (int i = 0; i < listOfFiles1.size(); ++i) {
        QFileInfo fileInfo1 = listOfFiles1.at(i);
        //similarFiles.at(i).push_back(fileInfo1);
        QString str1 = fileInfo1.absoluteFilePath();
        for (int j = 0; j < listOfFiles2.size(); ++j) {
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
                while (!(file1.atEnd() || file2.atEnd())) {
                    QByteArray data1 = file1.read(BUFSIZ);
                    QByteArray data2 = file2.read(BUFSIZ);
                    if(parallelCompareFiles(data1, data2)) {
                        isSimilar = true;
                    } else {
                        isSimilar = false;
                        break;
                    }
                }
                if(isSimilar) {
                    similarFiles.resize(similarFiles.size()+1);
                    similarFiles.at(similarFilesId).push_back(fileInfo1);
                    similarFiles.at(similarFilesId).push_back(fileInfo2);
                    similarFilesId++;
                    isSimilar = false;
                }
            }
        }
    }
}

bool CompareFolders::compareFiles(const QByteArray& data1,
    const QByteArray& data2, int leftIndex, int rightIndex) {
    for (int i = leftIndex; i <= rightIndex; ++i) {
        if(data1.at(i) != data2.at(i))
            return false;
    }
    return true;
}

bool CompareFolders::parallelCompareFiles(const QByteArray& data1, const QByteArray& data2) {
       qsizetype numberThreads = 4;
       qsizetype sizeVec = data1.size();
       qsizetype sizePartVec = sizeVec / numberThreads;
       QVector<bool> flags(numberThreads);
       std::vector<std::thread> threads;
       qsizetype remainder = sizeVec % numberThreads;
       for (int i = 0; i < numberThreads; i++) {
           std::thread th([&data1, &data2, &sizePartVec,
               &remainder, &flags, this](int i) {
               if (i == 0) {
                   flags[i] = compareFiles(data1, data2, i,
                       (i + 1) * sizePartVec + remainder - 1);
               } else {
                   flags[i] = compareFiles(data1, data2, i * sizePartVec + remainder,
                       (i + 1) * sizePartVec + remainder - 1);
               }
               }, i);
           threads.emplace_back(std::move(th));
       }

       for (int i = 0; i < numberThreads; i++)
           threads[i].join();
       for (int i = 0; i < numberThreads; i++) {
           if(!flags[i])
               return false;
       }
       return true;
}

void CompareFolders::printSimilar() {
    QTextStream out(stdout);
    out << "similar files:" << Qt::endl;
    for(size_t i = 0; i < similarFiles.size(); i++) {
        out << similarFiles.at(i).at(0).fileName()<<" - ";
        out << similarFiles.at(i).at(1).fileName()<<Qt::endl;
    }
}
