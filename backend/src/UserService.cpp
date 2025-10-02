#include "UserService.h"
#include <iostream>

UserService::UserService(Database* db) : database(db) {}

std::vector<User> UserService::getAllUsers() {
    std::vector<User> users;
    
    try {
        std::string query = "SELECT id, name, email, created_at, updated_at FROM users ORDER BY id";
        auto result = database->executeQuery(query);
        
        if (result) {
            while (result->next()) {
                User user;
                user.setId(result->getInt("id"));
                user.setName(result->getString("name"));
                user.setEmail(result->getString("email"));
                user.setCreatedAt(result->getString("created_at"));
                user.setUpdatedAt(result->getString("updated_at"));
                users.push_back(user);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting all users: " << e.what() << std::endl;
    }
    
    return users;
}

User UserService::getUserById(int id) {
    User user;
    
    try {
        std::string query = "SELECT id, name, email, created_at, updated_at FROM users WHERE id = ?";
        auto stmt = database->prepareStatement(query);
        stmt->setInt(1, id);
        auto result = std::unique_ptr<sql::ResultSet>(stmt->executeQuery());
        
        if (result && result->next()) {
            user.setId(result->getInt("id"));
            user.setName(result->getString("name"));
            user.setEmail(result->getString("email"));
            user.setCreatedAt(result->getString("created_at"));
            user.setUpdatedAt(result->getString("updated_at"));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting user by id: " << e.what() << std::endl;
    }
    
    return user;
}

bool UserService::createUser(const User& user) {
    try {
        std::string query = "INSERT INTO users (name, email) VALUES (?, ?)";
        auto stmt = database->prepareStatement(query);
        stmt->setString(1, user.getName());
        stmt->setString(2, user.getEmail());
        
        return stmt->executeUpdate() > 0;
    } catch (const std::exception& e) {
        std::cerr << "Error creating user: " << e.what() << std::endl;
        return false;
    }
}

bool UserService::updateUser(int id, const User& user) {
    try {
        std::string query = "UPDATE users SET name = ?, email = ? WHERE id = ?";
        auto stmt = database->prepareStatement(query);
        stmt->setString(1, user.getName());
        stmt->setString(2, user.getEmail());
        stmt->setInt(3, id);
        
        return stmt->executeUpdate() > 0;
    } catch (const std::exception& e) {
        std::cerr << "Error updating user: " << e.what() << std::endl;
        return false;
    }
}

bool UserService::deleteUser(int id) {
    try {
        std::string query = "DELETE FROM users WHERE id = ?";
        auto stmt = database->prepareStatement(query);
        stmt->setInt(1, id);
        
        return stmt->executeUpdate() > 0;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting user: " << e.what() << std::endl;
        return false;
    }
}
