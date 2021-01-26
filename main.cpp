#include "simpleapp.h"
#include "servercore.h"
#include "processstarter.h"


int main(int argc, char *argv[])
{
    SimpleHttpServerApplication app(argc, argv);

    if (!app.startServer())
    {
        qDebug() << "Unable to listen port. Terminating";
        return -1;
    }

    qDebug() << "Started server. Enter 'q' to quit";
    return app.exec();

}
