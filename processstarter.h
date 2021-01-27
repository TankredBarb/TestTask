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
    ProcessStarter(QObject * parent = nullptr);

    void execute (const QString& command);

    QStringList getProcessOuput () const {return mOutput;}

private:
    QProcess mProcess;
    QStringList mOutput;

private slots:

    void onReadyReadStdOut();
};

#endif // PROCESSSTARTER_H
