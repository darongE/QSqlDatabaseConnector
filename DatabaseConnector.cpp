/**
*****************************************************************************
**
** @file DatabaseConnector.cpp
** DatabaseConnector를 정의한 파일.
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

#include "DatabaseConnector.h"


DatabaseConnector * DatabaseConnector::_instance = 0;
const QString DatabaseConnector::ndatabaseType = "QOCI";
const QString DatabaseConnector::nhostName = "192.168.100.55";
const QString DatabaseConnector::ndatabaseName = "orcl";
const QString DatabaseConnector::nuserName = "nousco";
const QString DatabaseConnector::npassword = "nousco123";
const int DatabaseConnector::nport = 1521;



/**
 * Get DatabaseConnector Instance
 *
 * @return _instance :
 *
 * @par History
 * -# Dongwon.Choi : 2014.03.20 : 최초작성
 */
DatabaseConnector* DatabaseConnector::getInstance() {
    if (_instance == 0) {
        _instance = new DatabaseConnector;
    }
    return _instance;
}



/**
 * Destructor
 *
 * @par History
 * -# Dongwon.Choi : 2014.03.20 : 최초작성
 */
DatabaseConnector::~DatabaseConnector() {
    dbDisconnect();
}



/**
 * Constructor
 *
 * @par History
 * -# Dongwon.Choi : 2014.03.20 : 최초작성
 */
DatabaseConnector::DatabaseConnector() {
    // database driver check
    if (!QSqlDatabase::isDriverAvailable (ndatabaseType)) {
        exit(1);
    }

    // add to database information
    database = QSqlDatabase::addDatabase(ndatabaseType);
    database.setHostName(nhostName);
    database.setPort(nport);
    database.setDatabaseName(ndatabaseName);
    database.setUserName(nuserName);
    database.setPassword(npassword);

    dbConnect();
}


/**
 * Constructor
 *
 *
 * @par History
 * -# Dongwon.Choi : 2014.04.08 : 최초작성
 */
void DatabaseConnector::setDatabaseConnector(
        QString hostName,
        int port,
        QString databaseName,
        QString userName,
        QString password,
        DATABASE_TYPE type) {

    this->hostName = hostName;
    this->port = port;
    this->databaseName = databaseName;
    this->userName = userName;
    this->password = password;

    setDatabaseType(type);

    connect();
}



/**
 * 데이터베이스 인스턴스를 오픈하는 메소드
 *
 *
 * @par History
 * -# Dongwon.Choi : 2014.04.08 : 최초작성
 */
bool DatabaseConnector::connect() {
    if(database.isOpen()) {
        database.close();
        qDebug() << "database status : open, so execute to close";
    }

    // 다른 Database Solution도 else if처리 하여 접근가능하게 할 수 있다.
    if(QPSQL == databaseType() && database.contains("postgres") == 0) {
        database = QSqlDatabase::addDatabase("QPSQL", "postgres");
        qDebug() << "Database Vaildation :" <<(database.isValid() ? "Vaild" : "Failed");
    } else if (QMYSQL == databaseType() && database.contains("mysql") == 0) {
        database = QSqlDatabase::addDatabase("QMYSQL", "mysql");
        qDebug() << "Database Vaildation :" <<(database.isValid() ? "Vaild" : "Failed");
    } else if (QOCI == databaseType() && database.contains("oracle") == 0) {
        //database = QSqlDatabase::addDatabase("QODBC", "oracle");
        database = QSqlDatabase::addDatabase("QOCI");
        qDebug() << "Database Vaildation :" <<(database.isValid() ? "Vaild" : "Failed");
    } else {
        setLastError("Not Found DATABASE TYPE");
        return false;
    }

    if(hostName.isEmpty() && QPSQL == databaseType()) {
        setLastError("HostName was Empty!");
        return false;
    } else if(!hostName.isEmpty()) {
        database.setHostName(hostName);
    }

    if(port != 0) {
        database.setPort(port);
    }

    if(databaseName.isEmpty()) {
        setLastError("DatabaseName was Empty!");
        return false;
    } else {
        database.setDatabaseName(databaseName);
    }

    if(!userName.isEmpty()) {
        database.setUserName(userName);
    }

    if(!password.isEmpty()) {
        database.setPassword(password);
    }

    if(!database.open()) {
        setLastError(database.lastError().text());
        qDebug() << "Database State : not Open";
        qDebug() << database.lastError().text();
        return false;
    } else {
        qDebug() << "Database State : Open";
    }

    return true;
}



/**
 * Database Connection
 *
 * @return Database Open Status
 *  true : Success to connect
 *  false : Failed to connect
 *
 * @par History
 * -# Dongwon.Choi : 2014.03.20 : 최초작성
 */
bool DatabaseConnector::dbConnect() {
    if (!database.open()) {
        return false;
    }

    return true;
}



/**
 * Database Disconnection
 *
 * @return Database Close Status
 *  true : Database is close
 *  false : Database is open
 *
 * @par History
 * -# Dongwon.Choi : 2014.03.20 : 최초작성
 */
bool DatabaseConnector::dbDisconnect() {

    database.close();
    if (!database.isOpen()) {
        return true;
    }
    else {
        return false;
    }
}



/**
 * Execute Query
 *
 * @param queryStatement : 질의를 수행 할 문장
 *
 * @return result : Query Result
 *
 * @par History
 * -# Dongwon.Choi : 2014.03.20 : 최초작성
 */
QSqlQuery DatabaseConnector::dbExecuteQuery(QString queryStatement) {
    QSqlQuery result;

    if (database.isOpen()) {
        result = database.exec(queryStatement);
    }
    return result;
}



/**
 * Database Connection Check
 *
 * @return Database Open Status
 *  true : Database is open
 *  false : Database is close
 *
 * @par History
 * -# Dongwon.Choi : 2014.03.20 : 최초작성
 */
bool DatabaseConnector::isConnected() {
   return database.isOpen();
}



/**
 * 데이터베이스 타입을 설정하는 메소드
 *
 *
 * @par History
 * -# Dongwon.Choi : 2014.04.08 : 최초작성
 */
void DatabaseConnector::setDatabaseType(DATABASE_TYPE connectionType) {
    DatabaseType = connectionType;
}



/**
 * return Database
 *
 * @return Database instance
 *
 * @par History
 * -# Dongwon.Choi : 2014.04.02 : 최초작성
 */
QSqlDatabase DatabaseConnector::getDatabase() {
    return database;
}



/**
 * 에러를 담고 있는 변수를 반환하는 메소드
 *
 *
 * @par History
 * -# Dongwon.Choi : 2014.04.08 : 최초작성
 */
QString DatabaseConnector::error() {
    return lastError;
}



/**
 * 에러를 저장하는 메소드
 *
 *
 * @par History
 * -# Dongwon.Choi : 2014.04.08 : 최초작성
 */
void DatabaseConnector::setLastError(QString error) {
    lastError = error;
}



void DatabaseConnector::fitPerfect() {}
void DatabaseConnector::saveTemplate() {}
void DatabaseConnector::loadLatestDocument() {}
void DatabaseConnector::clearDocument() {}

