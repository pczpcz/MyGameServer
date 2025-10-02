#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <vector>
#include <memory>
#include "User.h"
#include "Database.h"

class UserService {
private:
    Database* database;

public:
    UserService(Database* db);
    
    // User operations
    std::vector<User> getAllUsers();
    User getUserById(int id);
    bool createUser(const User& user);
    bool updateUser(int id, const User& user);
    bool deleteUser(int id);
};

#endif // USERSERVICE_H
