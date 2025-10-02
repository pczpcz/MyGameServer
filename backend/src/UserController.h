#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <cpprest/http_msg.h>
#include "Database.h"
#include "UserService.h"

class UserController {
private:
    Database* database;
    UserService userService;

public:
    UserController(Database* db);
    
    // HTTP request handlers
    void getUsers(web::http::http_request request);
    void getUser(web::http::http_request request);
    void createUser(web::http::http_request request);
    void updateUser(web::http::http_request request);
    void deleteUser(web::http::http_request request);
    
private:
    int extractUserIdFromPath(const web::http::http_request& request);
};

#endif // USERCONTROLLER_H
