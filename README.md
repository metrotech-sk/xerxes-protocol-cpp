# Installation

```bash
git clone git@github.com:metrotech-sk/xerxes-protocol-cpp.git
cd xerxes-protocol-cpp
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release # or Debug
sudo make install -j 16
```

# Usage

## CMakeListst.txt

```cmake

target_link_libraries(
	${PROJECT_NAME} 
...
	xerxes-protocol
)
```

## main.cpp

```cpp
#include <xerxes-protocol/Network.hpp>
```

