#include "imgprocessor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImgProcessor w;
    QFont f("宋体",12);
    w.setFont(f);
    w.show();
    return a.exec();
}
