#include "Database.h"
#include <iostream>
#include <cstdlib>

Database::Database() {
    // Get database configuration from environment variables
    host = std::getenv("DB_HOST") ? std::getenv("DB_HOST") : "localhost";
    port = std::getenv("DB_PORT") ? std::getenv("DB_PORT") : "3306";
    database = std::getenv("DB_NAME") ? std::getenv("DB_NAME") : "myapp";
    username = std::getenv("DB_USER") ? std::getenv("DB_USER") : "appuser";
    password = std::getenv("DB_PASSWORD") ? std::getenv("DB_PASSWORD") : "apppassword";
    
    connect();
}

Database::~Database() {
    disconnect();
}

bool Database::connect() {
    try {
        driver.reset(sql::mysql::get_mysql_driver_instance());
        std::string connectionString = "tcp://" + host + ":" + port;
        connection.reset(driver->connect(connectionString, username, password));
        connection->setSchema(database);
        
        // Create user table if it doesn't exist
        createUserTable();
        
        std::cout << "Connected to MySQL database successfully" << std::endl;
        return true;
    } catch (sql::SQLException& e) {
        std::cerr << "MySQL Connection Error: " << e.what() << std::endl;
        std::cerr << "MySQL Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
        return false;
    }
}

void Database::disconnect() {
    if (connection) {
        connection->close();
        connection.reset();
    }
}

bool Database::isConnected() const {
    return connection && !connection->isClosed();
}

bool Database::createUserTable() {
    try {
        std::string createTableSQL = R"(
            CREATE TABLE IF NOT EXISTS users (
                id INT AUTO_INCREMENT PRIMARY KEY,
                name VARCHAR(100) NOT NULL,
                email VARCHAR(100) UNIQUE NOT NULL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
            )
        )";
        
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        stmt->execute(createTableSQL);
        std::cout << "User table created or already exists" << std::endl;
        return true;
    } catch (sql::SQLException& e) {
        std::cerr << "Error creating user table: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<sql::ResultSet> Database::executeQuery(const std::string& query) {
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        return std::unique_ptr<sql::ResultSet>(stmt->executeQuery(query));
    } catch (sql::SQLException& e) {
        std::cerr << "Query Error: " << e.what() << std::endl;
        return nullptr;
    }
}

bool Database::executeUpdate(const std::string& query) {
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        stmt->executeUpdate(query);
        return true;
    } catch (sql::SQLException& e) {
        std::cerr << "Update Error: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<sql::PreparedStatement> Database::prepareStatement(const std::string& query) {
    try {
        return std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement(query));
    } catch (sql::SQLException& e) {
        std::cerr << "Prepare Statement Error: " << e.what() << std::endl;
        return nullptr;
    }
}
