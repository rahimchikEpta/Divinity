#include "xor.h"
#include "menu/config/config.hpp"
#include "hooks/events/listener.h"
#include "features/utils/notifications.h"
#include "../source-sdk/steam-sdk/steam_api.h"
#include "../core/menu/lua/lua.h"
bool once = false;
unsigned long WINAPI initialize(void* instance) {
	while (!GetModuleHandleA("server.dll"))
		Sleep(200);


#ifdef _DEBUG
	if (!once) {
		csgo::build_type = "debug";
		csgo::cheat_username = "debug";
		once = true;
	}
	console::initialize("debug");
#else
	if (!once) {
		csgo::build_type = xorstr_("release");
		const std::string file = getenv(xorstr_("APPDATA")) + std::string(xorstr_("\\divinity\\login"));
		char temp_username[32];
		GetPrivateProfileStringA(xorstr_("Login"), xorstr_("Username"), "", temp_username, sizeof(temp_username), file.data());
		const std::string_view username = temp_username;
		csgo::cheat_username = username;
		once = true;
	}

#endif


		try {


			lua::init_state();
			interfaces::initialize();
			hooks::initialize();

			changers::parser::initialize();

#ifdef _DEBUG
#else
			changers::knife_proxy::initialize();
#endif		
			event_listener.initialize();
			settings::create_directory();
			lua::init_command();
			hooks::fully_init = true;
		}
		catch (const std::runtime_error& error) {
			MessageBoxA(nullptr, error.what(), xorstr_("Runtime error!"), MB_OK | MB_ICONERROR);
			FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	while (!hooks::disable)
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	//close menu so input is restored to user in the hooks::paint_traverse::hook hook.
	variables::menu::opened = false;
	//wait for paint_traverse::hook to be called and restore input. //designer is autistic
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

unsigned long WINAPI release() {
	hooks::release();
	event_listener.release();
#ifdef _DEBUG
	console::release();
#endif

	return TRUE;
}

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	DisableThreadLibraryCalls(instance);

	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
			CloseHandle(handle);

		break;
	}

	case DLL_PROCESS_DETACH: {
		release();
		break;
	}
	}

	return true;
}
