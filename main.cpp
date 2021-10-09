#include <QTextStream>
#include "CompareFolders.h"


int main(int argc, char *argv[])
{
    QTextStream out(stdout);
    if (argc != 3) {
      qWarning("The number of arguments should be 3");
      return 1;
    }

    QString path1 = argv[1];
    QString path2 = argv[2];
    CompareFolders c(path1, path2);
    c.compareTwoFolders();
    c.printSimilar();
    return 0;
}
