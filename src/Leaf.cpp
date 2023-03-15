#include "Leaf.hpp"

namespace Xerxes
{


Leaf::Leaf(address_t leaf_addr, Master *master)
{
    master = master;
    _my_addr = leaf_addr;
}


Leaf::~Leaf()
{
}


ping_reply_t Leaf::ping()
{
    return master->ping(_my_addr);
}

const address_t Leaf::getAddr() const
{
    return _my_addr;
}


} // namespace Xerxes