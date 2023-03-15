#ifndef __LEAF_HPP
#define __LEAF_HPP

#include "Master.hpp"

namespace Xerxes
{


class Leaf
{
private:
    Master *master;
    address_t _my_addr;
public:
    Leaf(){};
    Leaf(address_t leaf_addr, Master *master);
    ~Leaf();


    /**
     * @brief Ping the leaf
     * 
     * @return ping_reply_t
     * @throw std::runtime_error if the ping fails or times out 
     */
    ping_reply_t ping();

    const address_t getAddr() const;


    template<class T>
    const bool writeValue(
        const uint16_t mem_addr, 
        const T value
    ) const
    {
        return master->writeValue(_my_addr, mem_addr, value);
    }


    template<class T>
    const T readValue(
        const address_t device_addr, 
        const uint16_t mem_addr
    ) const
    {
        return master->readValue<T>(_my_addr, mem_addr);
    }
};


} // namespace Xerxes

#endif // __LEAF_HPP