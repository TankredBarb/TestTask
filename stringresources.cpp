#include "stringresources.h"

const QString StringResources::code200 = QString("HTTP/1.0 200 Ok\r\n"
                                                 "Content-Type: text/html; charset=\"utf-8\"\r\n"
                                                 "\r\n");

const QString StringResources::code404 = QString("HTTP/1.1 404 Not Found \r\n"
                                                 "Content-Type: text/html; charset=\"utf-8\"\r\n"
                                                 "\r\n");


const QString StringResources::welcomeString = QString("<h1>Available commands</h1>\n");

const QString StringResources::notFoundString = QString("<h1>Unknown command</h1>\n");


const QString StringResources::processesList = QString("<a href=\"/ProcList\">Processes List</a>");

const QString StringResources::listDir = QString("<a href=\"/ListDir\">List Dir</a>");

const QString StringResources::dateString = QString("<a href=\"/Date\">Date</a>");