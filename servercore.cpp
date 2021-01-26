#include "servercore.h"

SimpleHttpServer::SimpleHttpServer(quint16 port, QObject * par):
    QTcpServer(par), mPort(port)

{
    allMethods.append("GET");  //We only support GET for now
}

bool SimpleHttpServer::startListenPort()
{
    return listen(QHostAddress::Any, mPort);
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
    // Getting request from client
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
                // Send 200 for correct requests
                switch (mCurrentCommand)
                {
                    case comRoot:
                        handleRoot(pClientSocket); break;

                    case comProcList:
                        handleProcessesList(pClientSocket); break;

                    case comListDir:
                        handleListDir(pClientSocket); break;

                    case comDate:
                        handleDate(pClientSocket); break;

                    default:;
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
            // We only know HTTP GET method, so sending 500 for another
            handleServerFailure(pClientSocket);
        }

        pClientSocket ->close();
        if (pClientSocket->state() == QTcpSocket::UnconnectedState)
        {
            pClientSocket->deleteLater();
        }

    }
}


void SimpleHttpServer::dropClientSocket()
{
    // We should clear socket that we create in incomingConnection
    QTcpSocket * pClientSocket = qobject_cast<QTcpSocket *>(sender());

    if (pClientSocket != nullptr)
    {
        pClientSocket->deleteLater();
    }
}

SupportedCommands SimpleHttpServer::StringToCommand(const QString &s)
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

// handle* methods is for sending server responses
void SimpleHttpServer::handleRoot(QTcpSocket *pSocket)
{
    QTextStream os(pSocket);
    os.setAutoDetectUnicode(true);

    os << StringResources::code200;
    os << StringResources::welcomeString;
    os << StringResources::processesList << " ";
    os << StringResources::listDir << " ";
    os << StringResources::dateString;
}

void SimpleHttpServer::handleNotFound(QTcpSocket *pSocket)
{
    QTextStream os(pSocket);
    os.setAutoDetectUnicode(true);

    os << StringResources::code404;
    os << StringResources::notFoundString;
}

void SimpleHttpServer::handleServerFailure(QTcpSocket *pSocket)
{
    QTextStream os(pSocket);
    os.setAutoDetectUnicode(true);

    os << StringResources::code500;
    os << StringResources::internErrorString;
}

void SimpleHttpServer::handleProcessesList(QTcpSocket *pSocket)
{

    ProcessStarter dateProcess(this);
    dateProcess.execute(StringResources::getProcessesListCommand());

    QStringList out = dateProcess.getProcessOuput();

    if (out.length() > 0)
    {
        QTextStream os(pSocket);
        os.setAutoDetectUnicode(true);

        os << StringResources::code200;

        for (QString &t : out)
        {
            os << t << StringResources::htmlNewLine;
        }
    }    
    else //something is wrong with process, sending 500
    {
        handleServerFailure(pSocket);
    }

}

void SimpleHttpServer::handleListDir(QTcpSocket *pSocket)
{   
    ProcessStarter dateProcess(this);
    dateProcess.execute(StringResources::getListDirCommand());

    QStringList out = dateProcess.getProcessOuput();

    if (out.length() > 0)
    {
        QTextStream os(pSocket);
        os.setAutoDetectUnicode(true);

        os << StringResources::code200;

        for (QString &t : out)
        {
            os << t << StringResources::htmlNewLine;
        }
    }
    else //something is wrong with process, sending 500
    {
        handleServerFailure(pSocket);
    }
}

void SimpleHttpServer::handleDate(QTcpSocket *pSocket)
{    
    ProcessStarter dateProcess(this);
    dateProcess.execute(StringResources::getDateCommand());

    QStringList out = dateProcess.getProcessOuput();

    if (out.length() > 0)
    {
        QTextStream os(pSocket);
        os.setAutoDetectUnicode(true);

        os << StringResources::code200;

        for (QString &t : out)
        {
            os << t << StringResources::htmlNewLine;
        }

    }
    else //something is wrong with process, sending 500
    {
        handleServerFailure(pSocket);
    }
}


