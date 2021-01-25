#ifndef PROCESSSTARTER_H
#define PROCESSSTARTER_H

#include <QProcess>
#include <QStringList>
#include <QDebug>
#include <QTextCodec>

class ProcessStarter : public QObject
{
    Q_OBJECT


public:
    ProcessStarter(QObject * parent = 0);

    void execute (QString command);

    QStringList getProcessOuput () const {return mOutput;}

private:
    QProcess mProcess;
    QString mProcessOutput;
    QStringList mOutput;

private slots:

    void onReadyReadStdOut();

};

#endif // PROCESSSTARTER_H
