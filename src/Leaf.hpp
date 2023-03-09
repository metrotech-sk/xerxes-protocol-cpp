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
    Leaf(address_t leaf_addr, Master *master);
    ~Leaf();


    /**
     * @brief Ping the leaf
     * 
     * @return ping_reply_t
     * @throw std::runtime_error if the ping fails or times out 
     */
    ping_reply_t ping();
};


} // namespace Xerxes

#endif // __LEAF_HPP