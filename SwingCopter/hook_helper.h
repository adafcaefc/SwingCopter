#ifndef HOOK_HELPER_H
#define HOOK_HELPER_H

#pragma once
#include <MinHook.h>
#include <Windows.h>
#include <cinttypes>
#include <string>

#define HOOK_GD(offset, name) create_hook_gd(offset, h_##name, reinterpret_cast<LPVOID*>(&o_##name))
#define HOOK_METHOD(module_name, signature, name) create_hook_method(module_name, signature, h_##name, reinterpret_cast<LPVOID*>(&o_##name))

static bool create_hook(
	LPVOID target,
	LPVOID hook,
	LPVOID* original)
{
	auto hook_status = MH_CreateHook(target, hook, original);
	if (hook_status != MH_STATUS::MH_OK) return false;
	return MH_EnableHook(target) == MH_STATUS::MH_OK;
}

static bool create_hook_gd(
	const std::uintptr_t offset,
	LPVOID hook,
	LPVOID* original)
{
	const auto base = reinterpret_cast<std::uintptr_t>(GetModuleHandle(NULL));
	auto pTarget = reinterpret_cast<LPVOID>(base + offset);
	return create_hook(pTarget, hook, original);
}

static bool create_hook_method(
	const std::string module_name,
	const std::string signature,
	LPVOID hook,
	LPVOID* original)
{
	auto h_module = GetModuleHandleA(module_name.c_str());
	if (!h_module) return false;
	auto method_address = GetProcAddress(h_module, signature.c_str());
	if (!method_address) return false;
	return create_hook(method_address, hook, original);
}

#endif // HOOK_HELPER_H