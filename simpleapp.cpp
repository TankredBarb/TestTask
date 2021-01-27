#include "simpleapp.h"

StdinListener::StdinListener()
{
    this->connect(this, SIGNAL(finishedGetLine(const QString &)),
                     this, SLOT(onFinishedGetLine(const QString &)),
                     Qt::QueuedConnection);

#ifdef Q_OS_WIN
    mNotifier = new QWinEventNotifier(GetStdHandle(STD_INPUT_HANDLE));
#else
    mNotifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read);
#endif

    // Moving to helper thread to not get blocked by std::getline
    mNotifier->moveToThread(&mHelperThread);
    this->connect( &mHelperThread , SIGNAL(finished()),
                   mNotifier, SLOT(deleteLater()));


#ifdef Q_OS_WIN
    QObject::connect(mNotifier, &QWinEventNotifier::activated,
#else
    QObject::connect(mNotifier, &QSocketNotifier::activated,
#endif
    [this]() {
        std::string line;
        std::getline(std::cin, line);
        QString strLine = QString::fromStdString(line);
       emit this->finishedGetLine(strLine);
    });

    mHelperThread.start();
}

void StdinListener::onFinishedGetLine(const QString &strNewLine)
{
    emit newLine(strNewLine);
}

StdinListener::~StdinListener()
{
    mHelperThread.quit();
    mHelperThread.wait();
}


SimpleHttpServerApplication::SimpleHttpServerApplication(quint16 port,int argc, char **argv):
    QCoreApplication(argc, argv)
{
    mServer = new SimpleHttpServer(port, this);
    this->connect(this, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()));
    this->connect(&mStdinListener, SIGNAL(newLine(const QString &)), this, SLOT(onStdinNewLine(const QString &)));
}

bool SimpleHttpServerApplication::startServer()
{
    return mServer->startListenPort();
}

void SimpleHttpServerApplication::onAboutToQuit()
{
    qDebug() << "Quitting..";
    mServer->stopListenPort();
}

void SimpleHttpServerApplication::onStdinNewLine(const QString &newLine)
{    
    if (newLine.compare("q", Qt::CaseInsensitive) == 0)
    {
        this->quit();
    }
}
