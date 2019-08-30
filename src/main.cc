#include <QApplication>
#include <QVBoxLayout>
#include <QWebEngineView>
#include "pdfappwidget.h"

int main(int argc, char *argv[]) {
    int newArgc = argc + 1;
    char **newArgv = new char*[newArgc];
    for (int i = 0; i < newArgc; i++)
        newArgv[i] = argv[i];
    newArgv[newArgc] = (char*)"--disable-web-security";
    QApplication app(newArgc, newArgv);
    auto w = new QWidget();
    auto v = new QVBoxLayout(w);
    const QString path = "/home/burgess/example.pdf";
    auto a = new PdfAppWidget();
    bool res = a->loadFile(path);
    v->addWidget(a);
    w->show();
    return QApplication::exec();
}
