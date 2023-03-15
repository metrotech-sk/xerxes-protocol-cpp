#include "Packet.hpp"
#include <sstream>


namespace Xerxes
{


Packet::Packet()
{
}

Packet::Packet(std::vector<uint8_t> &message)
{
    checksum = 0;
    uint8_t msgLen = message.size() + 3;
    _data.emplace_back(SOH); // start of packet
    _data.emplace_back(msgLen); // message length
    checksum = SOH + msgLen;

    for(const auto &el:message)
    {
        _data.emplace_back(el);
        checksum += el;
    }

    checksum = ~(checksum & 0xFF) + 1; // two's complement
    _data.emplace_back(checksum);
}


Packet::Packet(const std::vector<uint8_t> &message)
{
    checksum = 0;
    uint8_t msgLen = message.size() + 3;
    _data.emplace_back(SOH); // start of packet
    _data.emplace_back(msgLen); // message length
    checksum = SOH + msgLen;

    for(const auto &el:message)
    {
        _data.emplace_back(el);
        checksum += el;
    }

    checksum = ~(checksum & 0xFF) + 1; // two's complement
    _data.emplace_back(checksum);
}


Packet::~Packet()
{
}


const uint8_t* Packet::data() const
{
    return _data.data();
}


size_t Packet::size() const
{
    return _data.size();
}


std::vector<uint8_t> Packet::getData() const
{
    return this->_data;
}


Packet Packet::EmptyPacket()
{
    std::vector<uint8_t> emptyVector {};
    return Packet(emptyVector);
}


uint8_t Packet::at(const uint8_t pos) const
{
    return _data.at(pos);
}


void Packet::setData(const std::vector<uint8_t> &data)
{
    _data = data;
}

bool Packet::isValidPacket()
{
    if(_data[0] != SOH)
    {
        return false;
    }

    uint8_t chks = 0;
    for(const auto & c : _data)
    {
        chks += c;
    }
    if(chks != 0)
    {
        return false;
    }
    return true;
}

bool isValidPacket(const std::vector<uint8_t> &data)
{
    if(data[0] != SOH)
    {
        return false;
    }

    uint8_t chks = 0;
    for(const auto & c : data)
    {
        chks += c;
    }
    if(chks != 0)
    {
        return false;
    }
    return true;
}


std::string Packet::toString() const
{
    std::string str;
    std::stringstream ss;
    for(const auto &c : _data)
    {
        ss << std::hex << (int)c << ":";
    }
    
    return ss.str();
}


} // namespace Xerxes