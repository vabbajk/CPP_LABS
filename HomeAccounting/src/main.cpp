#include <QApplication>
#include <QFont>
#include <QStyle>
#include "../include/MainWindow.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QTextCodec>
#endif

#ifdef _WIN32
#include <Windows.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;
    
    int argc = 0;
    char** argv = nullptr;
    QApplication app(argc, argv);

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    #endif

    QApplication::setStyle("Fusion");
    QFont font("Segoe UI", 10);
    QApplication::setFont(font);

    MainWindow window;
    window.show();

    return QApplication::exec();
}

#else
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    #endif

    app.setStyle("Fusion");
    QFont font("Segoe UI", 10);
    QApplication::setFont(font);

    MainWindow window;
    window.show();

    return QApplication::exec();
}
#endif