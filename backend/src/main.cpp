#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "Database.h"
#include "UserController.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class Application {
private:
    std::unique_ptr<Database> database;
    std::unique_ptr<UserController> userController;
    http_listener listener;

public:
    Application() 
        : listener("http://0.0.0.0:8080/api") {
        
        // Initialize database connection
        //database = std::make_unique<Database>();
        
        // Initialize controllers
        //userController = std::make_unique<UserController>(database.get());
        
        // Setup routes
        //setupRoutes();
    }

    /**
     * 设置路由配置方法
     * 该方法用于配置HTTP监听器的各种路由处理
     * 包括健康检查端点和用户相关的API路由
     */
    void setupRoutes() {
        // Health check endpoint
        listener.support(methods::GET, [](http_request request) {
            json::value response;
            response["status"] = json::value::string("OK");
            response["message"] = json::value::string("C++ Backend API is running");
            request.reply(status_codes::OK, response);
        });

        // User routes
        // 修改前（错误的方式）：
        // listener.support(methods::GET, U("/users"), 
        //     [this](http_request request) { userController->getUsers(request); });

        // 修改后（正确的方式）：
        listener.support(methods::GET, [this](http_request request) {
            auto path = request.request_uri().path();
            
            if (path == U("/users")) {
                userController->getUsers(request);
            } else if (path.find(U("/users/")) == 0) {
                // 处理 /users/{id} 模式
                userController->getUser(request);
            } else {
                // 其他未匹配的 GET 请求
                request.reply(status_codes::NotFound, U("Route not found"));
            }
        });

        listener.support(methods::POST, [this](http_request request) {
            auto path = request.request_uri().path();
            
            if (path == U("/users")) {
                userController->createUser(request);
            } else {
                request.reply(status_codes::NotFound, U("Route not found"));
            }
        });

        listener.support(methods::PUT, [this](http_request request) {
            auto path = request.request_uri().path();
            
            if (path.find(U("/users/")) == 0) {
                userController->updateUser(request);
            } else {
                request.reply(status_codes::NotFound, U("Route not found"));
            }
        });

        listener.support(methods::DEL, [this](http_request request) {
            auto path = request.request_uri().path();
            
            if (path.find(U("/users/")) == 0) {
                userController->deleteUser(request);
            } else {
                request.reply(status_codes::NotFound, U("Route not found"));
            }
        });
    }

    void run() {
        try {
            // Initialize database connection
            database = std::make_unique<Database>();
            
            // Initialize controllers
            userController = std::make_unique<UserController>(database.get());
            
            // Setup routes
            setupRoutes();
            
            listener.open().wait();
            std::cout << "C++ Backend API server listening on http://0.0.0.0:8080" << std::endl;
            std::cout << "Server is running. Use Ctrl+C to stop." << std::endl;
            
            // Keep the server running
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(60));
            }
            
            listener.close().wait();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
};

int main() {
    std::cout << "Starting C++ Backend API Server..." << std::endl;
    
    try {
        Application app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
