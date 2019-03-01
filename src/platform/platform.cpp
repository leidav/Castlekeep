#include "platform.h"

namespace platform
{
PlatformInterface::PlatformInterface(const memory::Arena &arena)
    : m_allocator(arena)
{
}

PlatformInterface::~PlatformInterface() {}

};  // namespace platform
