#ifndef PACKET_PROCESSOR_H
#define PACKET_PROCESSOR_H

#include <functional>
#include <unordered_map>
#include <iostream>
#include "UdpPacket.h"

class PacketProcessor
{
public:
    using Handler = std::function<void(const UdpPacket &)>;

    static PacketProcessor &getInstance()
    {
        static PacketProcessor instance;
        return instance;
    }

    void registerHandler(uint8_t opCode, Handler handler);
    void processPacket(const UdpPacket &packet);
    size_t getHandlerCount() const;

private:
    PacketProcessor() = default;
// 析构函数，使用默认实现
// 析构函数，使用默认实现
    ~PacketProcessor() = default;

    std::unordered_map<uint8_t, Handler> handlers_;
};

// 宏定义：注册处理函数
#define REGISTER_PACKET_HANDLER(opCode, handler)                                 \
    namespace                                                                    \
    {                                                                            \
        struct HandlerRegistrar_##opCode                                         \
        {                                                                        \
            HandlerRegistrar_##opCode()                                          \
            {                                                                    \
                PacketProcessor::getInstance().registerHandler(opCode, handler); \
            }                                                                    \
        };                                                                       \
        static HandlerRegistrar_##opCode handler_registrar_##opCode;             \
    }

// 预定义的操作码常量
#define OP_MOUSE_MOVE 1
#define OP_MOUSE_CLICK 2
#define OP_MOUSE_RELEASE 3
#define OP_KEY_PRESS 4
#define OP_KEY_RELEASE 5

#endif // PACKET_PROCESSOR_H
