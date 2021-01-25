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
     SimpleHttpServer(quint16 port, QObject* par = 0);

     void startListenPort();
     void stopListenPort();     

     void incomingConnection(qintptr socketId);


private slots:
     void readFromClientSocket();
     void dropClientSocket();

 private:
     bool disabled;

     quint16 mPort;

     // List of supported HTTP methods
     QStringList allMethods;

     SupportedCommands mCurrentCommand;

     SupportedCommands StringToCommand(QString s);

     // helper methods for sending http responses
     void handleRoot(QTcpSocket * pSocket);

     void handleNotFound(QTcpSocket * pSocket);

     void handleProcessesList(QTcpSocket * pSocket);

     void handleListDir(QTcpSocket * pSocket);

     void handleDate(QTcpSocket * pSocket);
 };



#endif // SERVERCORE_H