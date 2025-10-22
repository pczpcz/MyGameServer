#ifndef UDP_PACKET_QUEUE_H
#define UDP_PACKET_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "UdpPacket.h"

// 线程安全队列
class PacketQueue
{
public:
    void push(const UdpPacket &packet)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(packet);
        condition_.notify_one();
    }

    bool pop(UdpPacket &packet)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            return false;
        }
        packet = queue_.front();
        queue_.pop();
        return true;
    }

    bool wait_and_pop(UdpPacket &packet, std::chrono::milliseconds timeout)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (condition_.wait_for(lock, timeout, [this]
                                { return !queue_.empty(); }))
        {
            packet = queue_.front();
            queue_.pop();
            return true;
        }
        return false;
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    std::queue<UdpPacket> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;
};

#endif // UDP_PACKET_QUEUE_H
