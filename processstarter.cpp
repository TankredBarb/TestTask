#include "processstarter.h"

ProcessStarter::ProcessStarter(QObject *parent):
    QObject(parent)
{
    connect(&mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyReadStdOut()));
    mProcess.setProcessChannelMode(QProcess::MergedChannels);
}

void ProcessStarter::execute(QString command)
{
    mProcessOutput = "";
    mOutput.clear();
    mProcess.start(command);
    mProcess.waitForFinished();
    mProcess.close();
}

void ProcessStarter::onReadyReadStdOut()
{

    while (!mProcess.atEnd())
    {


        mOutput.append((mProcess.readLine()) );
    }
}
