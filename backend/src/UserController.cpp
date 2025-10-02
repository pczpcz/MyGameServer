#include "UserController.h"
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>
#include <iostream>

using namespace web;
using namespace web::http;

UserController::UserController(Database* db) 
    : database(db), userService(db) {}

void UserController::getUsers(http_request request) {
    try {
        auto users = userService.getAllUsers();
        json::value response = json::value::array();
        
        for (size_t i = 0; i < users.size(); ++i) {
            response[i] = users[i].toJson();
        }
        
        request.reply(status_codes::OK, response);
    } catch (const std::exception& e) {
        std::cerr << "Error in getUsers: " << e.what() << std::endl;
        json::value error;
        error["error"] = json::value::string("Failed to get users");
        request.reply(status_codes::InternalError, error);
    }
}

void UserController::getUser(http_request request) {
    try {
        int userId = extractUserIdFromPath(request);
        
        if (userId <= 0) {
            json::value error;
            error["error"] = json::value::string("Invalid user ID");
            request.reply(status_codes::BadRequest, error);
            return;
        }
        
        auto user = userService.getUserById(userId);
        
        if (user.getId() == 0) {
            json::value error;
            error["error"] = json::value::string("User not found");
            request.reply(status_codes::NotFound, error);
            return;
        }
        
        request.reply(status_codes::OK, user.toJson());
    } catch (const std::exception& e) {
        std::cerr << "Error in getUser: " << e.what() << std::endl;
        json::value error;
        error["error"] = json::value::string("Failed to get user");
        request.reply(status_codes::InternalError, error);
    }
}

void UserController::createUser(http_request request) {
    try {
        request.extract_json().then([this, request](pplx::task<json::value> task) {
            try {
                auto json = task.get();
                auto user = User::fromJson(json);
                
                if (user.getName().empty() || user.getEmail().empty()) {
                    json::value error;
                    error["error"] = json::value::string("Name and email are required");
                    request.reply(status_codes::BadRequest, error);
                    return;
                }
                
                if (userService.createUser(user)) {
                    json::value response;
                    response["message"] = json::value::string("User created successfully");
                    request.reply(status_codes::Created, response);
                } else {
                    json::value error;
                    error["error"] = json::value::string("Failed to create user");
                    request.reply(status_codes::InternalError, error);
                }
            } catch (const std::exception& e) {
                std::cerr << "Error in createUser: " << e.what() << std::endl;
                json::value error;
                error["error"] = json::value::string("Invalid JSON or missing fields");
                request.reply(status_codes::BadRequest, error);
            }
        }).wait();
    } catch (const std::exception& e) {
        std::cerr << "Error in createUser: " << e.what() << std::endl;
        json::value error;
        error["error"] = json::value::string("Failed to create user");
        request.reply(status_codes::InternalError, error);
    }
}

void UserController::updateUser(http_request request) {
    try {
        int userId = extractUserIdFromPath(request);
        
        if (userId <= 0) {
            json::value error;
            error["error"] = json::value::string("Invalid user ID");
            request.reply(status_codes::BadRequest, error);
            return;
        }
        
        request.extract_json().then([this, request, userId](pplx::task<json::value> task) {
            try {
                auto json = task.get();
                auto user = User::fromJson(json);
                
                if (user.getName().empty() || user.getEmail().empty()) {
                    json::value error;
                    error["error"] = json::value::string("Name and email are required");
                    request.reply(status_codes::BadRequest, error);
                    return;
                }
                
                if (userService.updateUser(userId, user)) {
                    json::value response;
                    response["message"] = json::value::string("User updated successfully");
                    request.reply(status_codes::OK, response);
                } else {
                    json::value error;
                    error["error"] = json::value::string("Failed to update user");
                    request.reply(status_codes::InternalError, error);
                }
            } catch (const std::exception& e) {
                std::cerr << "Error in updateUser: " << e.what() << std::endl;
                json::value error;
                error["error"] = json::value::string("Invalid JSON or missing fields");
                request.reply(status_codes::BadRequest, error);
            }
        }).wait();
    } catch (const std::exception& e) {
        std::cerr << "Error in updateUser: " << e.what() << std::endl;
        json::value error;
        error["error"] = json::value::string("Failed to update user");
        request.reply(status_codes::InternalError, error);
    }
}

void UserController::deleteUser(http_request request) {
    try {
        int userId = extractUserIdFromPath(request);
        
        if (userId <= 0) {
            json::value error;
            error["error"] = json::value::string("Invalid user ID");
            request.reply(status_codes::BadRequest, error);
            return;
        }
        
        if (userService.deleteUser(userId)) {
            json::value response;
            response["message"] = json::value::string("User deleted successfully");
            request.reply(status_codes::OK, response);
        } else {
            json::value error;
            error["error"] = json::value::string("Failed to delete user");
            request.reply(status_codes::InternalError, error);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in deleteUser: " << e.what() << std::endl;
        json::value error;
        error["error"] = json::value::string("Failed to delete user");
        request.reply(status_codes::InternalError, error);
    }
}

int UserController::extractUserIdFromPath(const http_request& request) {
    try {
        auto path = request.relative_uri().path();
        auto segments = uri::split_path(path);
        
        if (segments.size() >= 2) {
            std::string idStr = segments[1];
            return std::stoi(idStr);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error extracting user ID: " << e.what() << std::endl;
    }
    
    return -1;
}
