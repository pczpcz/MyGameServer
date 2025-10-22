#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "../udp/UdpPacket.h"
#include "../PacketProcessor.h"
#include <iostream>

// 鼠标移动处理函数
void handleMouseMove(const UdpPacket &packet)
{
    std::cout << "[MouseMove] User " << packet.userId
              << " moved mouse to (" << packet.mouseX << ", " << packet.mouseY << ")"
              << " from " << packet.endpoint.address().to_string() << ":" << packet.endpoint.port() << std::endl;
}

// 鼠标点击处理函数
void handleMouseClick(const UdpPacket &packet)
{
    const char *buttonName = "Unknown";
    switch (packet.mouseButton)
    {
    case 1:
        buttonName = "Left";
        break;
    case 2:
        buttonName = "Right";
        break;
    case 3:
        buttonName = "Middle";
        break;
    case 4:
        buttonName = "X1";
        break;
    case 5:
        buttonName = "X2";
        break;
    }

    std::cout << "[MouseClick] User " << packet.userId
              << " clicked " << buttonName << " button at ("
              << packet.mouseX << ", " << packet.mouseY << ")"
              << " from " << packet.endpoint.address().to_string() << ":" << packet.endpoint.port() << std::endl;
}

// 鼠标释放处理函数
void handleMouseRelease(const UdpPacket &packet)
{
    const char *buttonName = "Unknown";
    switch (packet.mouseButton)
    {
    case 1:
        buttonName = "Left";
        break;
    case 2:
        buttonName = "Right";
        break;
    case 3:
        buttonName = "Middle";
        break;
    case 4:
        buttonName = "X1";
        break;
    case 5:
        buttonName = "X2";
        break;
    }

    std::cout << "[MouseRelease] User " << packet.userId
              << " released " << buttonName << " button at ("
              << packet.mouseX << ", " << packet.mouseY << ")"
              << " from " << packet.endpoint.address().to_string() << ":" << packet.endpoint.port() << std::endl;
}

// 按键按下处理函数
void handleKeyPress(const UdpPacket &packet)
{
    std::cout << "[KeyPress] User " << packet.userId
              << " pressed key code: " << packet.keyCode
              << " from " << packet.endpoint.address().to_string() << ":" << packet.endpoint.port() << std::endl;
}

// 按键释放处理函数
void handleKeyRelease(const UdpPacket &packet)
{
    std::cout << "[KeyRelease] User " << packet.userId
              << " released key code: " << packet.keyCode
              << " from " << packet.endpoint.address().to_string() << ":" << packet.endpoint.port() << std::endl;
}

// 使用宏注册处理函数
REGISTER_PACKET_HANDLER(OP_MOUSE_MOVE, handleMouseMove)
REGISTER_PACKET_HANDLER(OP_MOUSE_CLICK, handleMouseClick)
REGISTER_PACKET_HANDLER(OP_MOUSE_RELEASE, handleMouseRelease)
REGISTER_PACKET_HANDLER(OP_KEY_PRESS, handleKeyPress)
REGISTER_PACKET_HANDLER(OP_KEY_RELEASE, handleKeyRelease)

#endif // INPUT_HANDLER_H
