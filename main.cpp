#include <QtCore/QCoreApplication>
#include "DatabaseConnector.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString findStartDay;
    findStartDay = "SELECT USID ";
    findStartDay += "FROM NMBI_USER ";

    QSqlQuery result = DatabaseConnector::getInstance()->dbExecuteQuery(findStartDay);

    if (result.isActive()) {
        while (result.next()) {
            qDebug() << result.value(0).toString();
        }
    }
    
    return a.exec();
}
