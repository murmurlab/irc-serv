#include "Client.hpp"
#include <cstring>

Client::Client(): desc(-1), len(sizeof(addr)) {
	std::memset(&addr, 0, sizeof(addr));
}
