#include "platform.h"
namespace platform_abstraction
{
Platform::Platform(memory::Allocator *allocator) : m_allocator(allocator) {}

Platform::~Platform() {}

};  // namespace platform_abstraction
