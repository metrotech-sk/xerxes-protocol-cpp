#ifndef __MASTER_HPP
#define __MASTER_HPP


#include "MessageId.h"
#include "Protocol.hpp"
#include <vector> 


typedef struct 
{
    uint8_t device_id;
    uint8_t v_major;
    uint8_t v_minor;
    float latency_ms;
} ping_reply_t;


typedef uint8_t address_t;

constexpr address_t BROADCAST_ADDRESS = 0xff;
constexpr uint64_t _default_timeout_us = 10000; // 10ms

namespace Xerxes
{

class Master
{
private:
    Protocol *xp;
    address_t _my_addr;
    uint32_t _timeoutUs;

public:
    /**
     * @brief Construct a new Master object
     * 
     * @param protocol Communication protocol to be used 
     * @param device_addr 
     */
    Master(
        Protocol *protocol, 
        const address_t device_addr, 
        const uint32_t timeoutUs = _default_timeout_us
    );
    
    ~Master();

    void setTimeout(const uint32_t timeoutUs);

    /**
     * @brief Ping a device on the bus
     * 
     * @param device_addr device address
     * @param timeoutUs timeout in microseconds
     * @return ping_reply_t with the ping reply
     * @throw std::runtime_error if the ping fails
     */
    ping_reply_t ping(address_t device_addr);

    /**
     * @brief Broadcast a message to all devices on the bus
     * 
     * @param msgid 
     * @param payload 
     * @param payload_size 
     */
    void broadcast(
        const msgid_t msgid, 
        const uint8_t *payload, 
        const uint8_t payload_size
    );

    /**
     * @brief Broadcast a message to all devices on the bus
     * 
     * @overload 
     * @param msgid - message id to broadcast
     * @param payload - payload to broadcast
    */
    void broadcast(
        const msgid_t msgid, 
        const std::vector<uint8_t> &payload
    );

    /**
     * @brief Broadcast an empty message to all devices on the bus
     * 
     * @param msgid 
     */
    void broadcast(const msgid_t msgid);


    /**
     * @brief Synchronize all devices on the bus with SYNC packet
     */
    void sync();


    /**
     * @brief Read a block of memory from a device
     * 
     * @param device_addr 
     * @param mem_addr 
     * @param size 
     * @return std::vector<uint8_t> memory block
     */
    std::vector<uint8_t> readMemory(
        address_t device_addr, 
        const uint16_t mem_addr, 
        const uint8_t size
    );

    bool writeMemory(
        address_t device_addr, 
        const uint16_t mem_addr, 
        const uint8_t *data, 
        const uint8_t size
    );

};
    
} // namespace Xerxes

#endif // !__MASTER_HPP