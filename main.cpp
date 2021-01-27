#include "simpleapp.h"
#include "servercore.h"

int main(int argc, char *argv[])
{
    SimpleHttpServerApplication app(8081,argc,argv);

    if (!app.startServer())
    {
        qDebug() << "Unable to listen port. Terminating";
        return -1;
    }

    qDebug() << "Started server at port 8081. Enter 'q' to quit";
    return app.exec();
}
