#include <QCoreApplication>
#include "servercore.h"
#include "processstarter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SimpleHttpServer server(8080);
    server.startListenPort();


    return a.exec();
}
