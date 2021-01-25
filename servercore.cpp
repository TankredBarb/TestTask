#include "servercore.h"


SimpleHttpServer::SimpleHttpServer(quint16 port, QObject * par):
    QTcpServer(par), mPort(port)

{
    allMethods.append("GET");  //We only support GET for now
}

void SimpleHttpServer::startListenPort()
{
    this->listen(QHostAddress::Any, mPort);
}

void SimpleHttpServer::stopListenPort()
{
    if (this->isListening())
    {
        this->close();
    }
}

void SimpleHttpServer::incomingConnection(qintptr socketId)
{
    QTcpSocket* clientSocket = new QTcpSocket(this);

    clientSocket->setSocketDescriptor(socketId);
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readFromClientSocket()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(dropClientSocket()));
}

void SimpleHttpServer::readFromClientSocket()
{

    // This slot is called when the client sent data to the server. The
    // server looks if it was a get request and sends a very simple HTML
    // document back.
    QTcpSocket * pClientSocket = qobject_cast<QTcpSocket *>(sender());

    if (pClientSocket != nullptr && pClientSocket->canReadLine())
    {
        QString buff = pClientSocket->readLine();
        QStringList tokens = QString(buff).split(QRegExp("[ \r\n][ \r\n]*"));

        // Considering this regexp tokens[0] will have the type of request(GET)
        // tokens[1] will have the path

        if (tokens.size() >=2 && tokens[0] == allMethods[0])  //GET
        {
            mCurrentCommand = StringToCommand(tokens[1]);

            if (mCurrentCommand != comDefault)
            {

                switch (mCurrentCommand)
                {
                    case comRoot:
                        handleRoot(pClientSocket); break;

                    case comProcList:
                        handleProcessesList(pClientSocket);  break;

                    case comListDir:
                        handleListDir(pClientSocket); break;

                    case comDate:
                        handleDate(pClientSocket); break;

                    default: ;
                }



                if (pClientSocket->state() == QTcpSocket::UnconnectedState)
                {
                    delete pClientSocket;

                //Connection closed
                }
            }

            else
            {
                // Send 404 for unknown command

                handleNotFound(pClientSocket);
            }


        }

        else
        {
            //TODO: Send 500 for non-GET methods
        }


    }
}

void SimpleHttpServer::dropClientSocket()
{
    QTcpSocket * pClientSocket = qobject_cast<QTcpSocket *>(sender());

    if (pClientSocket != nullptr)
    {
        pClientSocket->deleteLater();
    }
}

SupportedCommands SimpleHttpServer::StringToCommand(QString s)
{
    SupportedCommands command = comDefault;

    if (s == "/")
    {
       command = comRoot;
    }
    else if (s == "/Date")
    {
        command = comDate;
    }
    else if (s == "/ListDir")
    {
        command = comListDir;
    }
    else if (s == "/ProcList")
    {
        command = comProcList;
    }

    return command;
}

void SimpleHttpServer::handleRoot(QTcpSocket *pSocket)
{
    QTextStream os(pSocket);
    os.setAutoDetectUnicode(true);

    os << StringResources::code200;
    os << StringResources::welcomeString;
    os << StringResources::processesList << " ";
    os << StringResources::listDir << " ";
    os << StringResources::dateString;

    pSocket->close();
}

void SimpleHttpServer::handleNotFound(QTcpSocket *pSocket)
{
    QTextStream os(pSocket);
    os.setAutoDetectUnicode(true);

    os << StringResources::code404;
    os << StringResources::notFoundString;

    pSocket->close();

}

void SimpleHttpServer::handleProcessesList(QTcpSocket *pSocket)
{

    ProcessStarter dateProcess(this);
    dateProcess.execute("cmd.exe /C tasklist");

    QStringList out = dateProcess.getProcessOuput();

    if (out.length() > 0)
    {
        QTextStream os(pSocket);
        os.setAutoDetectUnicode(true);

        os << StringResources::code200;

        for (QString &t : out)
        {
            qDebug() << t;
            os << t ;
            os << "<br>";
        }

        pSocket->close();
    }

    //TODO: if no -> throw 500

}

void SimpleHttpServer::handleListDir(QTcpSocket *pSocket)
{
    //QTextStream os(pSocket);
    //os.setAutoDetectUnicode(true);

    //os << StringResources::code200;
    //os << "This is list dir";

    //pSocket->close();

    ProcessStarter dateProcess(this);
    dateProcess.execute("cmd.exe /C dir");

    QStringList out = dateProcess.getProcessOuput();

    if (out.length() > 0)
    {
        QTextStream os(pSocket);
        os.setAutoDetectUnicode(true);

        os << StringResources::code200;

        for (QString &t : out)
        {
            qDebug() << t;
            os << t ;
            os << "<br>";
        }

        pSocket->close();
    }
}

void SimpleHttpServer::handleDate(QTcpSocket *pSocket)
{
    /*QTextStream os(pSocket);
    os.setAutoDetectUnicode(true);

    os << StringResources::code200;
    os << "This is date"; */;

    ProcessStarter dateProcess(this);
    dateProcess.execute("cmd.exe /C date /T");

    QStringList out = dateProcess.getProcessOuput();

    if (out.length() > 0)
    {
        QTextStream os(pSocket);
        os.setAutoDetectUnicode(true);

        os << StringResources::code200;
        os << out[0]; //TODO: loop

        pSocket->close();
    }

    //TODO: if no -> throw 500


}

