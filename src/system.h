#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory/allocator.h>

namespace castlekeep
{
namespace core
{
template <class Derived>
class EngineSystem
{
public:
	static constexpr size_t minimalArenaSize()
	{
		return Derived::k_minimal_arena_size;
	}
	int startUp();
};

}  // namespace core
}  // namespace castlekeep

#endif
