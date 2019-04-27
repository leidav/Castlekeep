#include "input_manager.h"

#include <cstring>
#include "engine.h"

namespace castlekeep
{
namespace input
{
InputManager::InputManager()
{
	std::memset(&m_state, 0, sizeof(m_state));
}

bool InputManager::readInput()
{
	return core::Engine::g_engine->platform()->processEvents(m_state);
}

};  // namespace input
};  // namespace castlekeep
