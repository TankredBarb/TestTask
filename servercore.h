#ifndef SERVERCORE_H
#define SERVERCORE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>

#include "stringresources.h"
#include "processstarter.h"

enum SupportedCommands {comDate, comListDir, comProcList, comRoot, comDefault};

class SimpleHttpServer : public QTcpServer
{
     Q_OBJECT
 public:
     SimpleHttpServer(quint16 port, QObject* par = nullptr);

     bool startListenPort();
     void stopListenPort();     

     // Handle a new client connection.
     void incomingConnection(qintptr socketId);

private slots:
     void readFromClientSocket();
     void dropClientSocket();

private:

     quint16 mPort;

     // List of supported HTTP methods
     QStringList allMethods;

     SupportedCommands mCurrentCommand;

     // method to parse string data into the current command
     SupportedCommands StringToCommand(const QString& s);

     // helper methods for sending http responses
     void handleRoot(QTcpSocket * pSocket);
     void handleNotFound(QTcpSocket * pSocket);
     void handleProcessesList(QTcpSocket * pSocket);
     void handleListDir(QTcpSocket * pSocket);
     void handleDate(QTcpSocket * pSocket);
     void handleServerFailure(QTcpSocket * pSocket);
 };


#endif // SERVERCORE_H
