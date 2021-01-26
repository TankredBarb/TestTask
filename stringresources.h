#ifndef STRINGRESOURCES_H
#define STRINGRESOURCES_H

#include <QString>
#include <QtGlobal>

class StringResources
{
public:

    // HTTP codes
    static const QString code200;
    static const QString code404;
    static const QString code500;

    // resources;
    static const QString welcomeString;
    static const QString notFoundString;
    static const QString processesList;
    static const QString listDir;
    static const QString dateString;
    static const QString internErrorString;
    static const QString htmlNewLine;

    // for platform independent command strings
    static QString getProcessesListCommand();
    static QString getListDirCommand();
    static QString getDateCommand();

};



#endif // STRINGRESOURCES_H
