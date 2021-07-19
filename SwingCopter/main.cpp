#include <gd.h>
#undef snprintf
#include "hook_helper.h"

static inline void update_swing_copter(
	gd::PlayerObject* __this, 
	const float delta)
{
	const auto _direction = __this->m_isUpsideDown 
		? -1.f 
		:  1.f;

	const auto _size = (__this->m_fScale != 1.f)  
		? .85f 
		: 1.f;

	const auto _gravity = (std::fabs(__this->m_yAccel) > 6.) 
		? __this->m_gravity + 1. 
		: __this->m_gravity;

	const auto modifier = -.5 * _gravity * delta * _direction / _size;
	const auto y_acceleration = __this->m_yAccel + modifier;
	
	if (std::fabs(__this->m_yAccel) <= 6. ||
		std::fabs(y_acceleration) <= std::fabs(__this->m_yAccel))
	{
		__this->m_yAccel = y_acceleration;
	}

	if (__this->m_isHolding && __this->m_hasJustHeld)
	{
		__this->m_hasJustHeld = false;
		__this->flipGravity(!__this->m_isUpsideDown, true);
	}
}

void (__thiscall* o_updateJump) (gd::PlayerObject* __this, float delta);
void  __fastcall  h_updateJump	(gd::PlayerObject* __this, void*)
{
	auto delta = 0.f;
	__asm movss[delta], xmm1;
	if (!__this->m_isShip) return o_updateJump(__this, delta);
	return update_swing_copter(__this, delta);
}

DWORD WINAPI _dll_main(LPVOID lpParam)
{
	MH_Initialize();
	HOOK_GD(0x1E8F80, updateJump);
	return TRUE;
}