#ifndef DATABASE_H
#define DATABASE_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <memory>
#include <string>

class Database {
private:
    std::unique_ptr<sql::mysql::MySQL_Driver> driver;
    std::unique_ptr<sql::Connection> connection;
    
    std::string host;
    std::string port;
    std::string database;
    std::string username;
    std::string password;

public:
    Database();
    ~Database();
    
    bool connect();
    void disconnect();
    bool isConnected() const;
    
    // User operations
    bool createUserTable();
    std::unique_ptr<sql::ResultSet> executeQuery(const std::string& query);
    bool executeUpdate(const std::string& query);
    
    // Prepared statements
    std::unique_ptr<sql::PreparedStatement> prepareStatement(const std::string& query);
};

#endif // DATABASE_H
