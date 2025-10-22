#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include "udp/UdpPacket.h"
#include "udp/UdpPacketQueue.h"
#include "udp/UdpReceiver.h"
#include "PacketProcessor.h"
#include "handler/InputHandler.h"
#include "Database.h"
#include "UserController.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class Application
{
private:
    std::unique_ptr<Database> database;
    std::unique_ptr<UserController> userController;
    http_listener listener;

    // UDP相关
    boost::asio::io_context io_context_;
    PacketQueue packet_queue_;
    std::unique_ptr<UdpReceiver> udp_receiver_;
    std::thread io_thread_;
    bool running_;

public:
    Application()
        : listener("http://0.0.0.0:8080/api"), running_(false)
    {
        // Initialize database connection
        // database = std::make_unique<Database>();

        // Initialize controllers
        // userController = std::make_unique<UserController>(database.get());

        // Setup routes
        // setupRoutes();
    }

    void setupRoutes()
    {
        // Health check endpoint
        listener.support(methods::GET, [](http_request request)
                         {
            json::value response;
            response["status"] = json::value::string("OK");
            response["message"] = json::value::string("C++ Backend API is running");
            request.reply(status_codes::OK, response); });

        // User routes
        listener.support(methods::GET, [this](http_request request)
                         {
            auto path = request.request_uri().path();
            
            if (path == U("/users")) {
                // userController->getUsers(request);
                request.reply(status_codes::OK, U("Users endpoint"));
            } else if (path.find(U("/users/")) == 0) {
                // userController->getUser(request);
                request.reply(status_codes::OK, U("User detail endpoint"));
            } else {
                request.reply(status_codes::NotFound, U("Route not found"));
            } });

        listener.support(methods::POST, [this](http_request request)
                         {
            auto path = request.request_uri().path();
            
            if (path == U("/users")) {
                // userController->createUser(request);
                request.reply(status_codes::OK, U("User created"));
            } else {
                request.reply(status_codes::NotFound, U("Route not found"));
            } });
    }

    void run()
    {
        try
        {
            running_ = true;

            // Initialize database connection
            // database = std::make_unique<Database>();

            // Initialize controllers
            // userController = std::make_unique<UserController>(database.get());

            // Setup routes
            setupRoutes();

            // Start UDP receiver
            startUdpReceiver();

            listener.open().wait();
            std::cout << "C++ Backend API server listening on http://0.0.0.0:8080" << std::endl;
            std::cout << "UDP receiver listening on port 8888" << std::endl;
            std::cout << "Registered handlers: " << PacketProcessor::getInstance().getHandlerCount() << std::endl;
            std::cout << "Server is running. Use Ctrl+C to stop." << std::endl;

            // 主循环：处理数据包队列
            processPacketQueue();

            stopUdpReceiver();
            listener.close().wait();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

private:
    void startUdpReceiver()
    {
        // 创建UDP接收器，监听端口8888
        udp_receiver_ = std::make_unique<UdpReceiver>(io_context_, 8888, packet_queue_);

        // 启动UDP接收器
        udp_receiver_->start();

        // 在单独的线程中运行io_context，避免阻塞主线程
        io_thread_ = std::thread([this]()
                                 {
            try
            {
                std::cout << "Starting io_context.run() in separate thread..." << std::endl;
                io_context_.run();
                std::cout << "io_context.run() finished." << std::endl;
            }
            catch (const std::exception& e)
            {
                std::cerr << "UDP Receiver error: " << e.what() << std::endl;
            } });

        // Give the io_context thread a moment to start
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void stopUdpReceiver()
    {
        running_ = false;

        if (udp_receiver_)
        {
            udp_receiver_->stop();
        }

        io_context_.stop();

        if (io_thread_.joinable())
        {
            io_thread_.join();
        }
    }

    void processPacketQueue()
    {
        while (running_)
        {
            UdpPacket packet;

            // 等待并取出数据包（最多等待100ms）
            if (packet_queue_.wait_and_pop(packet, std::chrono::milliseconds(100)))
            {
                // 使用处理器管理器处理数据包
                PacketProcessor::getInstance().processPacket(packet);
            }

            // 定期显示状态
            static auto last_status_time = std::chrono::steady_clock::now();
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - last_status_time).count() >= 10)
            {
                std::cout << "Server status - Queue size: " << packet_queue_.size()
                          << ", Registered handlers: " << PacketProcessor::getInstance().getHandlerCount() << std::endl;
                last_status_time = now;
            }
        }
    }
};

int main()
{
    std::cout << "Starting C++ Backend API Server with UDP Support..." << std::endl;
    std::cout << "Current working directory: " << std::flush;

    // Try to get current working directory for debugging
    try
    {
        Application app;
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
