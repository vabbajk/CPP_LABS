#include <QApplication>
#include <QFont>
#include <QStyle>
#include "../include/MainWindow.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QTextCodec>
#endif

#ifdef _WIN32
#include <windows.h>

// Собственная точка входа WinMain для Windows GUI приложения
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Q_UNUSED(hInstance);
    Q_UNUSED(hPrevInstance);
    Q_UNUSED(lpCmdLine);
    Q_UNUSED(nCmdShow);
    
    int argc = 0;
    char** argv = nullptr;
    QApplication app(argc, argv);

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    #endif

    app.setStyle("Fusion");
    QFont font("Segoe UI", 10);
    app.setFont(font);

    MainWindow window;
    window.show();

    return app.exec();
}

#else
// Стандартная точка входа для других платформ
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    #endif

    app.setStyle("Fusion");
    QFont font("Segoe UI", 10);
    app.setFont(font);

    MainWindow window;
    window.show();

    return app.exec();
}
#endif