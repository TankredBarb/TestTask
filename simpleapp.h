#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <iostream>

#include "servercore.h"

#ifdef Q_OS_WIN

#include <QWinEventNotifier>
#include <windows.h>

#else

#include <QSocketNotifier>

#endif


// This class is needed to correctly integrate reading from stdin to QCoreApplication event loop
class StdinListener : public QObject
{
    Q_OBJECT

public:
    StdinListener();
    ~StdinListener();

signals:
    void newLine(const QString &strNewLine);

    // helper signal for eternal use
    void finishedGetLine(const QString &strNewLine);

private:
#ifdef Q_OS_WIN
    QWinEventNotifier *mNotifier;
#else
    QSocketNotifier *mNotifier;
#endif

private slots:
    void onFinishedGetLine(const QString &strNewLine);

private:
    QThread mHelperThread;
};

class SimpleHttpServerApplication : public QCoreApplication
{
    Q_OBJECT
public:
    SimpleHttpServerApplication(quint16 port,int argc, char **argv);

    bool startServer();

private slots:
    void onAboutToQuit();
    void onStdinNewLine(const QString & newLine);

private:
    SimpleHttpServer * mServer;
    StdinListener mStdinListener;
};

#endif // SIMPLEAPP_H
