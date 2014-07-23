/**
*****************************************************************************
**
** @file DatabaseConnector.h
** DatabaseConnector class를 선언한 파일.
**
** Copyright (C) 2009 Nousco Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nousco Corporation (ehr-info@nousco.com)
**
** @par History
** -# Dongwon.Choi : 2014.03.20 : 최초작성
**
****************************************************************************/
/* vi: set ts=4 sw=4 sts=4 expandtab: */

#ifndef __DATABASE_CONNECTOR_H__
#define __DATABASE_CONNECTOR_H__

#include <QtCore>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>



/**
 * DatabaseConnector class 선언
 *
 * @par History
 * -# Dongwon.Choi : 2014.03.20 : 최초작성
 */
class DatabaseConnector {
public:
    enum DATABASE_TYPE {
        UNDEFINED,
        MSACCESS,
        QMYSQL,
        QPSQL,
        QOCI,
        QSQLITE
    } DatabaseType;

    void setDatabaseConnector(
        QString hostName,
        int port,
        QString databaseName,
        QString userName,
        QString password,
        DATABASE_TYPE type);
    void setDatabaseType(DATABASE_TYPE connectionType);
    static DatabaseConnector* getInstance();
    QSqlDatabase getDatabase();
    bool connect();
    bool dbConnect();
    bool isConnected();
    bool dbDisconnect();
    QString error();
    QSqlQuery dbExecuteQuery(QString);
    DATABASE_TYPE databaseType() {
                return DatabaseType;
            }
    ~DatabaseConnector();

    void fitPerfect();
    void saveTemplate();
    void loadLatestDocument();
    void clearDocument();


protected:
    DatabaseConnector();

    static const QString ndatabaseType;
    static const QString nhostName;
    static const QString ndatabaseName;
    static const QString nuserName;
    static const QString npassword;
    static const int nport;


private:
    void setLastError(QString error);
    static DatabaseConnector* _instance;
    QString lastError;
    /// database instance
    QSqlDatabase database;
    QString hostName;
    int port;
    QString databaseName;
    QString userName;
    QString password;
    DATABASE_TYPE type;
};

#endif // __DATABASE_CONNECTOR_H__

