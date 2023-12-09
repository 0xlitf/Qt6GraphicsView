#include <iostream>

#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QTime>
#include <QDir>
#include <QMutex>
#include <QtGui>

#define STRINGX(x) #x
#define STRING(x) STRINGX(x)
#define MY_LOG(msg) __pragma(message(__FILE__"(" STRING(__LINE__) "): " "\033[0;30;42m " msg "\033[0m"))

void logger(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static QMutex mutex;
    static QFile file;

    mutex.lock();
    if (!file.isOpen()) {
        QString logPath{"."};
        // = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).append(QString::fromLocal8Bit(""));


        QDir logDir(logPath);

        if (!logDir.exists()) {
            logDir.mkpath(logPath);
        }

        file.setFileName(logPath + QString("/app_%1.log").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss")));
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        //text_stream.setDevice(&file);
        //text_stream.setCodec("UTF-8");
        //text_stream << "\r\n" << "\r\n";
        //file.flush();
    }

    QString text;

    switch (type)
    {
        case QtDebugMsg:
            text = QString("\033[32m""Debug""\033[0m");
            break;
        case QtWarningMsg:
            text = QString("\033[35;1m""Warning""\033[0m");
            break;
        case QtCriticalMsg:
            text = QString("Critical");
            break;
        case QtFatalMsg:
            text = QString("Fatal");
        case QtInfoMsg:;
            text = QString("Info");
    }

    QString message = QString("[File:%1:Line]: [%3] [%4] %5")
                          .arg(QString::fromLocal8Bit(context.file))
                          .arg(QString::number(context.line))
                          .arg(text)
                          .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                          .arg(msg);

    QTextStream text_stream(&file);
    // text_stream.setCodec("UTF-8");
    text_stream << message << "\r\n";
    file.flush();
    //file.close();
    mutex.unlock();
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    if (!qEnvironmentVariableIsEmpty("VSAPPIDDIR")) { // start direct in vs
        qSetMessagePattern("%{file}(%{line}): "
                           "[%{type}]:"
                           " [%{function}]:\n"
                           " [%{time yyyy-MM-dd h:mm:ss.zzz t}]"
                           " %{message}");
        qDebug() << "start in vs: " << qgetenv("VSAPPIDDIR");
    } else if (!qEnvironmentVariableIsEmpty("QTDIR")) { // start direct in QtCreator
        qSetMessagePattern("\033[1;37m [file://%{file}:%{line}]:\033[0m"
                           " %{if-debug}\033[1;36m%{endif}%{if-info}\033[1;44m%{endif}%{if-warning}\033[1;43m%{endif}%{if-critical}\033[1;41m%{endif}%{if-fatal}\033[1;41m%{endif}[%{type}]:\033[0m"
                           "\033[1;35m [%{function}]:\033[0m\n"
                           "\033[0;37m [%{time yyyy-MM-dd h:mm:ss.zzz t}]\033[0m"
                           "\033[1;91m %{message}\033[0m");
        qDebug() << "start in QtCreator: " << qgetenv("QTDIR");
    } else {
        qInstallMessageHandler(logger);
    }

    MY_LOG("This is a log message at compile");
    qDebug() << "qDebug";
    qInfo() << "qInfo";
    qWarning() << "qWarning";
    qCritical() << "qCritical";
    // qFatal() << "qFatal";

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString& locale : uiLanguages) {
        const QString baseName = "i18n_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.setMinimumSize(1280, 700);
    w.show();
    return a.exec();
}
