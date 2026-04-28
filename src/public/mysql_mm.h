#pragma once

#include <functional>
#include <string>

#define MYSQLMM_INTERFACE "SQLInterface"

class IMySQLQuery;

typedef std::function<void(bool)> ConnectCallbackFunc;
typedef std::function<void(IMySQLQuery *)> QueryCallbackFunc;
typedef std::function<void(std::vector<IMySQLQuery *>)> TransactionSuccessCallbackFunc;
typedef std::function<void(std::string, int)> TransactionFailureCallbackFunc;

struct Transaction
{
    std::vector<std::string> queries;
};

class IMySQLRow
{
public:
};

class IMySQLResult
{
public:
    virtual int GetRowCount() = 0;
    virtual int GetFieldCount() = 0;
    virtual bool FieldNameToNum(const char *name, unsigned int *columnId) = 0;
    virtual const char *FieldNumToName(unsigned int colId) = 0;
    virtual bool MoreRows() = 0;
    virtual IMySQLRow *FetchRow() = 0;
    virtual IMySQLRow *CurrentRow() = 0;
    virtual bool Rewind() = 0;
    virtual int GetFieldType(unsigned int field) = 0;
    virtual const char *GetString(unsigned int columnId, size_t *length = nullptr) = 0;
    virtual size_t GetDataSize(unsigned int columnId) = 0;
    virtual float GetFloat(unsigned int columnId) = 0;
    virtual int GetInt(unsigned int columnId) = 0;
    virtual bool IsNull(unsigned int columnId) = 0;
    virtual int64_t GetInt64(unsigned int columnId) = 0;
};

class IMySQLQuery
{
public:
    virtual IMySQLResult *GetResultSet() = 0;
    virtual bool FetchMoreResults() = 0;
    virtual unsigned int GetInsertId() = 0;
    virtual unsigned int GetAffectedRows() = 0;
};

class IMySQLConnection
{
public:
    virtual void Connect(ConnectCallbackFunc callback) = 0;
    virtual void Query(char *query, QueryCallbackFunc callback) = 0;
    virtual void Query(const char *query, QueryCallbackFunc callback, ...) = 0;
    virtual void ExecuteTransaction(Transaction txn, TransactionSuccessCallbackFunc success, TransactionFailureCallbackFunc failure) = 0;
    virtual void Destroy() = 0;
    virtual std::string Escape(char *string) = 0;
    virtual std::string Escape(const char *string) = 0;
};

typedef enum EMySQLType
{
    MM_MYSQL_TYPE_DECIMAL,
    MM_MYSQL_TYPE_TINY,
    MM_MYSQL_TYPE_SHORT,
    MM_MYSQL_TYPE_LONG,
    MM_MYSQL_TYPE_FLOAT,
    MM_MYSQL_TYPE_DOUBLE,
    MM_MYSQL_TYPE_NULL,
    MM_MYSQL_TYPE_TIMESTAMP,
    MM_MYSQL_TYPE_LONGLONG,
    MM_MYSQL_TYPE_INT24,
    MM_MYSQL_TYPE_DATE,
    MM_MYSQL_TYPE_TIME,
    MM_MYSQL_TYPE_DATETIME,
    MM_MYSQL_TYPE_YEAR,
    MM_MYSQL_TYPE_NEWDATE,
    MM_MYSQL_TYPE_VARCHAR,
    MM_MYSQL_TYPE_BIT,
    MM_MYSQL_TYPE_TIMESTAMP2,
    MM_MYSQL_TYPE_DATETIME2,
    MM_MYSQL_TYPE_TIME2,
    MM_MYSQL_TYPE_UNKNOWN,
    MM_MYSQL_TYPE_JSON = 245,
    MM_MYSQL_TYPE_NEWDECIMAL = 246,
    MM_MYSQL_TYPE_ENUM = 247,
    MM_MYSQL_TYPE_SET = 248,
    MM_MYSQL_TYPE_TINY_BLOB = 249,
    MM_MYSQL_TYPE_MEDIUM_BLOB = 250,
    MM_MYSQL_TYPE_LONG_BLOB = 251,
    MM_MYSQL_TYPE_BLOB = 252,
    MM_MYSQL_TYPE_VAR_STRING = 253,
    MM_MYSQL_TYPE_STRING = 254,
    MM_MYSQL_TYPE_GEOMETRY = 255
} EMySQLType;

struct MySQLConnectionInfo
{
    const char *host;
    const char *user;
    const char *pass;
    const char *database;
    int port = 3306;
    int timeout = 60;
};

class IMySQLClient
{
public:
    virtual IMySQLConnection *CreateMySQLConnection(MySQLConnectionInfo info) = 0;
};

class IMySQLInterface
{
public:
    virtual IMySQLClient *GetMySQLClient() = 0;
};
