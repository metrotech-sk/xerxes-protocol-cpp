#include "Master.hpp"
#include <chrono>
#include <stdexcept>

namespace Xerxes
{


Master::Master(
    Protocol *protocol, 
    const address_t device_addr, 
    const uint32_t timeoutUs
) : _timeoutUs(timeoutUs)
{
    xp = protocol;
    _my_addr = device_addr;
}


Master::~Master()
{
}


ping_reply_t Master::ping(
    address_t device_addr
)
{
    ping_reply_t reply;
    reply.device_id = 0;
    reply.v_major = 0;
    reply.v_minor = 0;
    reply.latency_ms = 0.0;

    const Message ping_msg(_my_addr, device_addr, MSGID_PING);
    Message reply_msg;

    auto start_time = std::chrono::steady_clock::now();
    
    xp->sendMessage(ping_msg);

    if(xp->readMessage(reply_msg, _timeoutUs))
    {
        auto end_time = std::chrono::steady_clock::now();
        if(reply_msg.msgId == MSGID_PING_REPLY)
        {
            auto payload_it = reply_msg.payloadBegin();
            reply.device_id = *payload_it++;
            reply.v_major = *payload_it++;
            reply.v_minor = *payload_it++;
            reply.latency_ms = (float)std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        return reply;
        }
        else
        {
            throw std::runtime_error("Invalid ping reply received.");
        }
    }
    else
    {
        throw TimeoutError("Ping timeout.");
    }
}

    
void Master::broadcast(
    const msgid_t msgid, 
    const uint8_t *payload, 
    const uint8_t payload_size
)
{
    std::vector<uint8_t> payload_vec(payload, payload + payload_size);

    broadcast(msgid, payload_vec);
}


void Master::broadcast(
    const msgid_t msgid, 
    const std::vector<uint8_t> &payload
)
{
    Message msg(_my_addr, BROADCAST_ADDRESS, msgid, payload);

    xp->sendMessage(msg);
}


void Master::broadcast(const msgid_t msgid)
{
    Message msg(_my_addr, BROADCAST_ADDRESS, msgid);

    xp->sendMessage(msg);
}

void Master::sync()
{
    broadcast(MSGID_SYNC);
}

std::vector<uint8_t> Master::readMemory(
    address_t device_addr, 
    const uint16_t address, 
    const uint8_t size
)
{
    std::vector<uint8_t> payload;
    payload.push_back((uint8_t)(address & 0xff));  // little endian
    payload.push_back((uint8_t)(address >> 8));
    payload.push_back(size);

    Message msg(_my_addr, device_addr, MSGID_READ, payload);
    Message reply_msg;

    xp->sendMessage(msg);

    if(xp->readMessage(reply_msg, _timeoutUs))
    {
        if(reply_msg.msgId == MSGID_READ_VALUE)
        {
            return std::vector<uint8_t>(reply_msg.payloadBegin(), reply_msg.end());
        }
        else
        {
            throw std::runtime_error("Invalid read memory reply received.");
        }
    }
    else
    {
        throw TimeoutError("Read memory timeout.");
    }
}


bool Master::writeMemory(
    address_t device_addr, 
    const uint16_t address, 
    const uint8_t *payload, 
    const uint8_t payload_size
)
{
    std::vector<uint8_t> payload_vec;

    payload_vec.push_back((uint8_t)(address & 0xff));  // little endian
    payload_vec.push_back((uint8_t)(address >> 8));
    payload_vec.insert(payload_vec.end(), payload, payload + payload_size);
    
    xp->sendMessage(Message(_my_addr, device_addr, MSGID_WRITE, payload_vec));
    uint32_t old_timeout = _timeoutUs;
    if(address < 256)
    {
        setTimeout(100000); // 100ms for memory write in FLASH
    }
    else
    {
        setTimeout(10000); // 10ms for memory write in RAM
    }

    Message reply_msg;
    bool read_ok = xp->readMessage(reply_msg, _timeoutUs);
    setTimeout(old_timeout);

    if(read_ok)
    {
        if(reply_msg.msgId == MSGID_ACK_OK)
        {
            return true;
        }
        else if(reply_msg.msgId == MSGID_ACK_NOK)
        {
            return false;
        }
        else
        {
            throw std::runtime_error("Invalid write memory reply received.");
        }
    }
    else
    {
        throw TimeoutError("Write memory timeout.");
    }

}


void Master::setTimeout(const uint32_t timeoutUs)
{
    _timeoutUs = timeoutUs;
}


} // namespace Xerxes
