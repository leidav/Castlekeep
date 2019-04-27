#ifndef INPUT_H
#define INPUT_H

#include <cstdint>

namespace castlekeep
{
namespace input
{
enum Key {
	k_key_0 = 0,
	k_key_1 = 1,
	k_key_2 = 2,
	k_key_3 = 3,
	k_key_4 = 4,
	k_key_5 = 5,
	k_key_6 = 6,
	k_key_7 = 7,
	k_key_8 = 8,
	k_key_9 = 9,
	k_key_q = 10,
	k_key_w = 11,
	k_key_e = 12,
	k_key_r = 13,
	k_key_t = 14,
	k_key_y = 15,
	k_key_u = 16,
	k_key_i = 17,
	k_key_o = 18,
	k_key_p = 19,
	k_key_a = 20,
	k_key_s = 21,
	k_key_d = 22,
	k_key_f = 23,
	k_key_g = 24,
	k_key_h = 25,
	k_key_j = 26,
	k_key_k = 27,
	k_key_l = 28,
	k_key_z = 29,
	k_key_x = 30,
	k_key_c = 31,
	k_key_v = 32,
	k_key_b = 33,
	k_key_n = 34,
	k_key_m = 35,
	k_key_space = 36,
	k_key_return = 37,
	k_key_backspace = 38,
	k_key_arrow_up = 39,
	k_key_arrow_down = 40,
	k_key_arrow_left = 41,
	k_key_arrow_right = 42
};

struct MouseState {
	bool left_button_down;
	bool right_button_down;
	bool middle_button_down;
	int16_t x;
	int16_t y;
	int16_t dx;
	int16_t dy;
};

struct KeyState {
	bool key_down;
	bool keys[43];
};

struct InputState {
	MouseState mouse;
	KeyState keyboard;
};

class InputManager
{
public:
	InputManager();
	bool readInput();
	const InputState& state() const { return m_state; }

private:
	InputState m_state;
};

};  // namespace input
};  // namespace castlekeep
#endif
