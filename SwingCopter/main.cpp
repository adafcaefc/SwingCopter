#include <gd.h>
#undef snprintf
#include "hook_helper.h"

static inline void update_swing_copter(
	gd::PlayerObject* __this, 
	const float delta)
{
	const auto _go_size = __this->m_fScale;
	const auto _direction = __this->m_isUpsideDown ? -1.f : 1.f;
	const auto _size = (_go_size != 1.f)  ? .85f : 1.f;
	auto _gravity = __this->m_gravity;

	const auto _y_accel = __this->m_yAccel;
	if (std::fabs(_y_accel) > 6.) _gravity += 1.;

	const auto n_y_accel = _y_accel + (_gravity * delta * _direction * -.5) / _size;
	if (std::fabs(_y_accel) <= 6. ||
		std::fabs(n_y_accel) <= std::fabs(_y_accel))
	{
		__this->m_yAccel = n_y_accel;
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