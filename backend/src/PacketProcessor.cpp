#include "PacketProcessor.h"

void PacketProcessor::registerHandler(uint8_t opCode, Handler handler)
{
    handlers_[opCode] = handler;
    std::cout << "Registered handler for opCode: " << static_cast<int>(opCode) << std::endl;
}

void PacketProcessor::processPacket(const UdpPacket &packet)
{
    auto it = handlers_.find(packet.opType);
    if (it != handlers_.end())
    {
        it->second(packet);
    }
    else
    {
        std::cout << "No handler registered for opCode: " << static_cast<int>(packet.opType) << std::endl;
    }
}

size_t PacketProcessor::getHandlerCount() const
{
    return handlers_.size();
}
