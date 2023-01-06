
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../menu/menu.hpp"
#include "../menu/imgui/imgui.h"
#include "../menu/imgui/imgui_impl_dx9.h"
#include "../menu/imgui/imgui_impl_win32.h"
#include "../menu/config/config.hpp"
#include "../features/game_data/game_data.h"
#include "../../dependencies/utilities/renderer/fonts.h"
#include "../menu/ImGui/custom.h"
#include "../menu/ImGui/fonts.h"
#include "../features/utils/notifications.h"
#include "../../dependencies/utilities/csgo.hpp"
#include "../xor.h"
#include "clzss.h"
#include "../post-processing/postprocessing.h"
#include "../features/visuals/motion_blur.h"
#include "../features/discord/DiscordRPC.h"
#include "../../dependencies/utilities/rendering/rendering.hpp"
hooks::wnd_proc::fn wnd_proc_original = nullptr;
hooks::reset::fn reset_original = nullptr;
hooks::present::fn present_original = nullptr;
hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::lock_cursor::fn lock_cursor_original = nullptr;
hooks::emit_sound::fn emit_sound_original = nullptr;
hooks::endscene::fn endscene_original = nullptr;
hooks::dme::fn draw_model_execute_original = nullptr;
hooks::override_mouse_input::fn override_mouse_input_original = nullptr;
hooks::frame_stage_notify::fn fsn_original = nullptr;
hooks::do_post_screen_effects::fn dpse_original = nullptr;
hooks::send_datagram::fn datagram_original = nullptr;
hooks::get_color_modulation::fn get_color_modulation_original = nullptr;
hooks::hk_particle_collection_simulate::fn pcs_original = nullptr;
hooks::is_using_static_prop_debug_modes::fn is_using_static_prop_debug_modes_original = nullptr;
hooks::crc_server_check::fn crc_server_check_original = nullptr;
hooks::file_system::fn file_system_original = nullptr;
hooks::draw_text_fields::fn dtf_original = nullptr;
hooks::send_net_message::fn snm_original = nullptr;
hooks::DrawTextFields::fn DTF_original = nullptr;
hooks::LooseFileAllowed::fn LFA_original = nullptr;
hooks::scene_end::fn scene_end_original = nullptr;
hooks::Render2dEffectsPreHud::fn render2deffects_original = nullptr;
hooks::set_visuals_data::fn set_visuals_data_original = nullptr;
hooks::supress_lists::fn surpress_lists_original = nullptr;
hooks::set_draw_color::fn set_draw_color_original = nullptr;
hooks::find_or_add_filename::fn find_or_add_filename_original = nullptr;
hooks::retaddr::fn retaddr_original = nullptr;
hooks::render_smoke_overlay::fn render_smoke_overlay_original = nullptr;
hooks::draw_viewmodels::fn draw_viewmodels_original = nullptr;
inline vec3_t originalAngle;
inline float originalForwardMove, originalSideMove;
inline float mouse_dx;
inline void startMovementFix(c_usercmd* cmd) {
	originalAngle = cmd->viewangles;
	originalForwardMove = cmd->forwardmove;
	originalSideMove = cmd->sidemove;
}
inline void endMovementFix(c_usercmd* cmd) {
	// this was just taken from designer bc im lazy
	// https://github.com/designer1337/csgo-cheat-base/blob/09fa2ba8de52eef482bbc82f682976e369191077/dependencies/math/math.cpp#L4
	float deltaViewAngles;
	float f1;
	float f2;

	if (originalAngle.y < 0.f)
		f1 = 360.0f + originalAngle.y;
	else
		f1 = originalAngle.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		deltaViewAngles = abs(f2 - f1);
	else
		deltaViewAngles = 360.0f - abs(f1 - f2);

	deltaViewAngles = 360.0f - deltaViewAngles;

	cmd->forwardmove = cos(DEG2RAD(deltaViewAngles)) * originalForwardMove + cos(DEG2RAD(deltaViewAngles + 90.f)) * originalSideMove;
	cmd->sidemove = sin(DEG2RAD(deltaViewAngles)) * originalForwardMove + sin(DEG2RAD(deltaViewAngles + 90.f)) * originalSideMove;
	// TODO: support upmove
}
inline void normalizeAngles(vec3_t& angle) {
	while (angle.x > 89.0f)
		angle.x -= 180.f;

	while (angle.x < -89.0f)
		angle.x += 180.f;

	while (angle.y > 180.f)
		angle.y -= 360.f;

	while (angle.y < -180.f)
		angle.y += 360.f;
}
bool hooks::initialize() {
	wnd_proc_original = reinterpret_cast<decltype(wnd_proc_original)>(SetWindowLongA(FindWindowA("Valve001", nullptr), GWL_WNDPROC, reinterpret_cast<LONG>(wnd_proc::hook)));

	auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	auto dtf_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, 28));
	auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	auto endscene_target = reinterpret_cast<void*>(get_virtual(interfaces::directx, 42));
	auto present_target = reinterpret_cast<void*>(get_virtual(interfaces::directx, 17));
	auto lock_cursor_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, 67));
	auto emit_sound_target = reinterpret_cast<void*>(get_virtual(interfaces::engine_sound, 5));
	auto dme_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));
	auto omi_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 23));
	auto dpse_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	auto fsn_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	auto LooseFileAllowedHook_target = reinterpret_cast<void*>(get_virtual(interfaces::file_system, 128));
	auto render2deffects_target = reinterpret_cast<void*>(get_virtual(interfaces::view_render, 39));
	auto SendNetMessage_t = utilities::pattern_scan("engine.dll", "55 8B EC 83 EC 08 56 8B F1 8B 4D 04 E8");
	auto set_visuals_data_target = utilities::pattern_scan("client.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8B 53 5C");
	auto color_modulation = utilities::pattern_scan("materialsystem.dll", "55 8B EC 83 EC ? 56 8B F1 8A 46");
	auto prop_debug_modes = utilities::pattern_scan("engine.dll", "8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 ? A1 ? ? ? ? 35 ? ? ? ? EB ? 8B 01 FF 50 ? 83 F8 ? 0F 85 ? ? ? ? 8B 0D");
	auto get_color_modulation_target = reinterpret_cast<void*>(color_modulation);
	auto is_using_static_prop_debug_modes_target = reinterpret_cast<void*>(prop_debug_modes);
	void* pcs_target = Rel2Abs(utilities::pattern_scan("client.dll", "E8 ? ? ? ? 8B 0E 83 C1 10") + 1);
	void* const draw_view_models_target = utilities::pattern_scan("client.dll", "53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 ? ? ? ? ? 56 57 FF");//void* const draw_view_models_target = address(utilities::scan_pattern((uint32_t*)GetModuleHandle("client.dll"), ("E8 0D C1 ? ? ? ? 8B CF"))).relative();
	const char* sig = xorstr_("55 8B EC 56 8B F1 33 C0 57 8B 7D 08 8B 8E ? ? ? ? 85 C9 7E 16 3B 3C 86");
	void* const render_smoke_overlay_target = utilities::pattern_scan("client.dll", xorstr_("53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 48 80 7B 08 00 56 57 8B F9 89 7D F4 0F 84 ? ? ? ? 8B"));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");


	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
	else {
		hooked += 1;
		MH_EnableHook(create_move_target);
	}

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
		throw std::runtime_error(("failed to initialize paint_traverse."));
	else {
		hooked += 1;
		MH_EnableHook(paint_traverse_target);
	}

	//nice nigger

	// run present first so we can start the user "initialisation".

	if (MH_CreateHook(endscene_target, &endscene::hook, reinterpret_cast<void**>(&endscene_original)) != MH_OK)
		throw std::runtime_error("failed to initialize Endscene. (outdated index?)");
	// init it before anything else
	MH_EnableHook(endscene_target);

	if (MH_CreateHook(dtf_target, &draw_text_fields::hook, reinterpret_cast<void**>(&dtf_original)) != MH_OK)
		throw std::runtime_error("failed to initialize DrawTextFields. (outdated index?)");
	else {
		hooked += 1;
		MH_EnableHook(dtf_target);
	}
	if (MH_CreateHook(get_color_modulation_target, &hooks::get_color_modulation::hook, reinterpret_cast<void**>(&get_color_modulation_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize get_color_modulation."));
		return false;
	}
	else {
		hooked += 1;
		MH_EnableHook(get_color_modulation_target);
	}

	if (MH_CreateHook(lock_cursor_target, &lock_cursor::hook, reinterpret_cast<void**>(&lock_cursor_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize lock_cursor. (outdated index?)");
		return false;
	}
	else {
		hooked += 1;
		MH_EnableHook(lock_cursor_target);
	}
	if (MH_CreateHook(emit_sound_target, &emit_sound::hook, reinterpret_cast<void**>(&emit_sound_original)) != MH_OK)
		throw std::runtime_error("failed to initialize emit sound. (outdated index?)");
	else {
		hooked += 1;
		MH_EnableHook(emit_sound_target);
	}
	if (MH_CreateHook(pcs_target, &hooks::hk_particle_collection_simulate::hook, reinterpret_cast<void**>(&pcs_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize GetColorModulation."));
		return false;
	}
	else {
		hooked += 1;
		MH_EnableHook(pcs_target);
	}
	if (MH_CreateHook(dpse_target, &do_post_screen_effects::hook, reinterpret_cast<void**>(&dpse_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize do_post_screen_effects. (outdated index?)");
		return false;
	}
	else {
		hooked += 1;
		MH_EnableHook(dpse_target);
	}
	if (MH_CreateHook(dme_target, &dme::hook, reinterpret_cast<void**>(&draw_model_execute_original)) != MH_OK)
		throw std::runtime_error("failed to initialize draw_model_execute. (outdated index?)");
	else {
		hooked += 1;
		MH_EnableHook(dme_target);
	}
	if (MH_CreateHook(omi_target, &override_mouse_input::hook, reinterpret_cast<void**>(&override_mouse_input_original)) != MH_OK)
		throw std::runtime_error("failed to initialize override mouse input. (outdated index?)");
	else {
		hooked += 1;
		MH_EnableHook(omi_target);
	}
	if (MH_CreateHook(render2deffects_target, &Render2dEffectsPreHud::hook, reinterpret_cast<void**>(&render2deffects_original)) != MH_OK)
		throw std::runtime_error("failed to initialize Render2dEffectsPreHud. (outdated index?)");
	else {
		hooked += 1;
		MH_EnableHook(render2deffects_target);
	}
	if (MH_CreateHook(present_target, &present::hook, reinterpret_cast<void**>(&present_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize Present. (outdated index?)");
		return false;
	}
	else {
		hooked += 1;
		MH_EnableHook(present_target);
	}
	if (MH_CreateHook(SendNetMessage_t, &send_net_message::hook, reinterpret_cast<void**>(&snm_original)) != MH_OK)
		throw std::runtime_error("failed to initialize SendNetMessage. (outdated index?)");
	else {
		hooked += 1;
		MH_EnableHook(SendNetMessage_t);
	}
	if (MH_CreateHook(fsn_target, &frame_stage_notify::hook, reinterpret_cast<void**>(&fsn_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize frame_stage_notify. (outdated index?)");
		return false;
	}
	else {
		hooked += 1;
		MH_EnableHook(fsn_target);
	}
	if (MH_CreateHook(set_visuals_data_target, &set_visuals_data::set_visuals_data, reinterpret_cast<void**>(&set_visuals_data_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize set_visuals_data. (outdated index?)");
		return false;
	}
	else {
		hooked += 1;
		MH_EnableHook(set_visuals_data_target);
	}
	if (MH_CreateHook(is_using_static_prop_debug_modes_target, &hooks::is_using_static_prop_debug_modes::hook, reinterpret_cast<void**>(&is_using_static_prop_debug_modes_original)) != MH_OK) {
		throw std::runtime_error(("failed to initialize IsUsingStaticDebug."));
		return false;
	}
	else {
		hooked += 1;
		MH_EnableHook(is_using_static_prop_debug_modes_target);
	}
	if (MH_CreateHook(LooseFileAllowedHook_target, &LooseFileAllowed::hook, reinterpret_cast<void**>(&LFA_original)) != MH_OK)
		throw std::runtime_error("failed to initialize LooseFileAllowed. (outdated index?)");
	else {
		hooked += 1;
		MH_EnableHook(LooseFileAllowedHook_target);
	}

	if (MH_CreateHook(render_smoke_overlay_target, &render_smoke_overlay::hook, reinterpret_cast<void**>(&render_smoke_overlay_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize render_smoke_overlay.");
		return false;
	}
	else
		MH_EnableHook(render_smoke_overlay_target);



	void* result = utilities::pattern_scan("client.dll", sig);
	void* result1 = utilities::pattern_scan("engine.dll", sig);
	void* result2 = utilities::pattern_scan("studiorender.dll", sig);
	void* result3 = utilities::pattern_scan("materialsystem.dll", sig);

	if (MH_CreateHook(result, &retaddr::hook, reinterpret_cast<void**>(&retaddr_original)) != MH_OK) {
		throw std::runtime_error("failed to bypass retaddr_client.");
		return false;
	} else
		MH_EnableHook(result);

	if (MH_CreateHook(result1, &retaddr::hook, reinterpret_cast<void**>(&retaddr_original)) != MH_OK) {
		throw std::runtime_error("failed to bypass retaddr_engine.");
		return false;
	}
	else
		MH_EnableHook(result1);

	if (MH_CreateHook(result2, &retaddr::hook, reinterpret_cast<void**>(&retaddr_original)) != MH_OK) {
		throw std::runtime_error("failed to bypass retaddr_studiorender.");
		return false;
	}
	else
		MH_EnableHook(result2);

	if (MH_CreateHook(result3, &retaddr::hook, reinterpret_cast<void**>(&retaddr_original)) != MH_OK) {
		throw std::runtime_error("failed to bypass retaddr_materialsystem.");
		return false;
	}
	else
		MH_EnableHook(result3);

	if (MH_CreateHook(draw_view_models_target, &draw_viewmodels::hook, reinterpret_cast<void**>(&draw_viewmodels_original)) != MH_OK) {
		throw std::runtime_error("failed to hook draw_viewmodels.");
		return false;
	}
	else
		MH_EnableHook(draw_view_models_target);

	/*if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");*/

	console::log("[setup] hooks initialized!\n");
	//interfaces::surface->play_sound("buttons\\arena_switch_press_02.wav");
	return true;
}

void hooks::release() {
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);

	if (wnd_proc_original) {
		SetWindowLongA(FindWindowA("Valve001", nullptr), GWL_WNDPROC, reinterpret_cast<LONG>(wnd_proc_original));
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	if (ImGui::GetCurrentContext()) {
		ImGui::DestroyContext();
	}

	if (interfaces::inputsystem) {
		interfaces::inputsystem->enable_input(true);
	}
}

long __stdcall hooks::wnd_proc::hook(HWND window, UINT msg, WPARAM wparm, LPARAM lparm) {
	if (menu::is_open)
		if (msg == WM_MOUSEWHEEL)
			return false;

	return CallWindowProcA(wnd_proc_original, window, msg, wparm, lparm);
}

long __stdcall hooks::reset::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) {
	ImGui_ImplDX9_InvalidateDeviceObjects();
	PostProcessing::onDeviceReset();
	return reset_original(device, params);
}

IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;
DWORD dwOld_D3DRS_COLORWRITEENABLE;
DWORD colorwrite, srgbwrite;

void SaveState(IDirect3DDevice9* device) {
	D3DVIEWPORT9 d3d_viewport;
	device->GetViewport(&d3d_viewport);

	device->CreateStateBlock(D3DSBT_ALL, &pixel_state);
	pixel_state->Capture();

	device->SetVertexShader(nullptr);
	device->SetPixelShader(nullptr);
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

	device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}

void RestoreState(IDirect3DDevice9* pDevice) // not restoring everything. Because its not needed. YEAH RIGHT!
{
	pixel_state->Apply();
	pixel_state->Release();
}
long __stdcall hooks::present::hook(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region) {

	if (!menu::init) {
		menu::on_init(device);
		menu::second_init_because_lazy();
		menu::fonts::menu_font = ImGui::GetIO().Fonts->AddFontDefault();
		menu::indicator_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanab.ttf", 28);
		menu::indicator_font_small = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 18);
		menu::indicator_font_small_bold = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanab.ttf", 18);
		menu::notification_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanab.ttf", 14);
		if (!custom_gui::backend::init) {
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\calibri.ttf", 14.0);
			custom_gui::backend::bold_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\calibrib.ttf", 16.0);
			custom_gui::backend::title_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\calibrib.ttf", 22.f);
			custom_gui::backend::icons = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(icons_font_data, icons_font_size, 20.f);
			custom_gui::backend::smaller_icons = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(icons_font_data, icons_font_size, 14.f);
			custom_gui::backend::select_fonts = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(arrows_data, arrows_size, 18.f);
			custom_gui::backend::multiselect_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(arrows_data, arrows_size, 14.f);
			custom_gui::backend::init = true;
		}
		menu::fonts::indicators = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdanab.ttf", 28);

		render::device = device;
		menu::init = true;
	}
	visuals::disable_panorama_blur();

	if (GetAsyncKeyState(VK_INSERT) & 1) {
		menu::is_open = !menu::is_open;
		menu::has_toggled_menu_once = true;
	}

	IDirect3DVertexDeclaration9* vertex_dec;
	device->GetVertexDeclaration(&vertex_dec);

	IDirect3DVertexShader9* vertex_shader;
	device->GetVertexShader(&vertex_shader);

	PostProcessing::setDevice(device);
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	menu::render();

	
		if (settings::discord_rpc)
			DiscordMain();
		else
			Shutdown();
	
	visuals::spectator_list();
	misc::main::watermark();
	if (interfaces::engine) {
		if (interfaces::engine->is_in_game() || interfaces::engine->is_connected() || csgo::local_player->is_alive()) {
			if (interfaces::engine->is_in_game() || interfaces::engine->is_connected())
			{
				if (csgo::local_player) {

					if (csgo::local_player->is_alive()) {
						//ImGui::GetBackgroundDrawList()->AddText(ImVec2(20, 20), ImColor(255, 255, 255), std::to_string(csgo::local_player->next_attack()).data());
						//ImGui::GetBackgroundDrawList()->AddText(ImVec2(20, 40), ImColor(255, 255, 255), std::to_string(csgo::local_player->active_weapon()->next_primary_attack()).data());
					//	ImGui::GetBackgroundDrawList()->AddText(ImVec2(20, 60), ImColor(255, 255, 255), std::to_string(csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick).data());
						misc::movement::display_velo();
						misc::main::aspect_ratio();
						misc::movement::stamina_graph_draw();
						misc::movement::velo_graph_draw();
						if (settings::visulisation::visualize_eb && settings::visulisation::predicted_eb_circle2d && misc::movement::should_eb) {
							vec3_t screen;
							color col = color::from_float(settings::visulisation::predicted_eb_circle2d_col);
							if (math::world_to_screen(variables::player_origin_on_predicted_eb, screen)) {
								ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(screen.x, screen.y), 26.f, ImColor(col.r, col.g, col.b, col.a), 360);
								ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(screen.x, screen.y), 26.f, ImColor(0, 0, 0, col.a), 360);
							}
						}
					}
				}
			}
		}
	}


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	device->SetVertexShader(vertex_shader);
	device->SetVertexDeclaration(vertex_dec);

	return present_original(device, src, dest, window_override, dirty_region);
}
inline float init_hooks_alpha = 0.f;
inline float set_timeout = 0.f;
bool song = false;
HRESULT __stdcall hooks::endscene::hook(IDirect3DDevice9* device) {

	if (settings::misc::stream_proof) {
		static uintptr_t gameoverlay_return_address = 0;
		if (!gameoverlay_return_address) {
			MEMORY_BASIC_INFORMATION info;
			VirtualQuery(_ReturnAddress(), &info, sizeof(MEMORY_BASIC_INFORMATION));

			char mod[MAX_PATH];
			GetModuleFileNameA((HMODULE)info.AllocationBase, mod, MAX_PATH);

			if (strstr(mod, "gameoverlay"))
				gameoverlay_return_address = (uintptr_t)(_ReturnAddress());
		}

		if (gameoverlay_return_address != (uintptr_t)(_ReturnAddress()))
			return endscene_original(interfaces::directx, device);
	}
	else {
		static auto ret = _ReturnAddress();

		if (ret != _ReturnAddress())
			return endscene_original(interfaces::directx, device);
	}

	if (!settings::visuals::viewmodel) {
		variables::saved_viewmodel_x = interfaces::console->get_convar(xorstr_("viewmodel_offset_x"))->get_float();
		variables::saved_viewmodel_y = interfaces::console->get_convar(xorstr_("viewmodel_offset_y"))->get_float();
		variables::saved_viewmodel_z = interfaces::console->get_convar(xorstr_("viewmodel_offset_z"))->get_float();
	}


	IDirect3DStateBlock9* pixel_state = nullptr;
	IDirect3DVertexDeclaration9* vertex_declaration = nullptr;
	IDirect3DVertexShader9* vertex_shader = nullptr;

	device->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
	device->GetVertexDeclaration(&vertex_declaration);
	device->GetVertexShader(&vertex_shader);

	if (!variables::render_init)
		if (render::do_init(device))
		{
			variables::render_init = true;
			//menu::init = true;
		}
	render::set_states();


	float deltatime = (1.f / 0.2f) * interfaces::globals->cur_time;

	init_hooks_alpha = std::clamp(init_hooks_alpha + (3.f * deltatime * ((hooked < 21) ? 1.f : -1.f)), 0.0f, 1.f);

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	int hooks_alpha = init_hooks_alpha * 100;
	int hooks_alpha2 = init_hooks_alpha * 255;
	/*render::filled_rectangle(vec2_t(0, 0), vec2_t(width, height), color(0, 0, 0, hooks_alpha));
	render::text(vec2_t(width / 2, height / 2), color(255, 255, 255, hooks_alpha2), "Initialising hooks..", render::fonts::name, true);
	render::text(vec2_t(width / 2, height / 2 + 20), color(255, 255, 255, hooks_alpha2), std::format("{} / 21 hooked", hooked), render::fonts::name, true);
	if (init_hooks_alpha > 0.2f) {
		set_timeout = interfaces::globals->cur_time;
	}*/


	// i fucking hate myself for these sanity check lmODISOAJFUIAJSFI
	//SaveState(device);
	if (interfaces::engine) {
		if (interfaces::engine->is_in_game() || interfaces::engine->is_connected()) {
			if (csgo::local_player) {
				if (render::device) {
					visuals::run();
					visuals::run_weapon_esp();
					visuals::engine_based_force_crosshair();
					if (csgo::local_player->is_alive()) {
						misc::movement::visualize_eb();
						visuals::nade_pred::draw();

						visuals::key_strokes();

						if ((interfaces::engine->is_in_game() && csgo::local_player)) {
							if (settings::visuals::visuals_disableflash)
								csgo::local_player->flash_alpha() = 0;
							else if (!(csgo::local_player->flash_alpha() == 100))
								csgo::local_player->flash_alpha() = 100;
						}

					}
				}


			}
		}
	}
	//
	for (auto hk : lua::hook_manager::get_hooks(("render"))) {
		auto result = hk.func();
		if (!result.valid()) 
		{
			sol::error err = result;
			interfaces::console->console_color_printf(color::from_float(c::menu_accent), ("[lua] "));
			interfaces::console->console_printf(std::string(err.what()).append(" \n").c_str());
			notifications::notify(err.what());
		}

	}
	//RestoreState(device);
	pixel_state->Apply();
	pixel_state->Release();
	device->SetVertexDeclaration(vertex_declaration);
	device->SetVertexShader(vertex_shader);
	return endscene_original(interfaces::directx, device);
}
class CEffectData {
public:
	vec3_t m_vOrigin;
	vec3_t m_vStart;
	vec3_t m_vNormal;
	vec3_t m_vAngles;
	int		m_fFlags;
	entity_t m_hEntity;
	int		m_nEntIndex;

	float	m_flScale;
	float	m_flMagnitude;
	float	m_flRadius;
	int		m_nAttachmentIndex;
	short	m_nSurfaceProp;

	// Some TF2 specific things
	int		m_nMaterial;
	int		m_nDamageType;
	int		m_nHitBox;

	int		m_nOtherEntIndex;

	unsigned char	m_nColor;

	bool	m_bPositionsAreRelativeToEntity;

	// Don't mess with stuff below here. DispatchEffect handles all of this.
public:
	CEffectData() {
		/*	m_vOrigin.Init ();
			m_vStart.Init ();
			m_vNormal.Init ();
			m_vAngles.Init ();*/

		m_fFlags = 0;
		//m_hEntity = (entity_t)0;

		m_nEntIndex = 0;

		m_flScale = 1.f;
		m_nAttachmentIndex = 0;
		m_nSurfaceProp = 0;

		m_flMagnitude = 0.0f;
		m_flRadius = 0.0f;

		m_nMaterial = 0;
		m_nDamageType = 0;
		m_nHitBox = 0;

		m_nColor = 0;

		m_nOtherEntIndex = 0;

		m_bPositionsAreRelativeToEntity = false;
	}

	int GetEffectNameIndex() { return m_iEffectName; }


	i_client_renderable* GetRenderable() const;
	entity_t* GetEntity() const;
	int entindex() const;


private:
	int m_iEffectName;	// Entry in the EffectDispatch network string table. The is automatically handled by DispatchEffect().
};

using FX_TeslaFn = void(__thiscall*)(CTeslaInfo&);
void FX_Tesla(CTeslaInfo& tesla_info) {
	static auto fx = reinterpret_cast<FX_TeslaFn>(utilities::pattern_scan(("client.dll"), xorstr_("55 8B EC 81 EC ? ? ? ? 56 57 8B F9 8B 47 18")));
	fx(tesla_info);
}

using CreateSnowFallFn = void(__thiscall*)();
void CreateSnowFall() {
	static auto CreateSnowFallFunc = reinterpret_cast<CreateSnowFallFn>(utilities::pattern_scan(("client.dll"), xorstr_("55 8B EC 83 E4 F8 83 EC 4C 53 56 8B F1 8B 0D ? ? ? ? 57 8B 81 ? ? ? ? 89 44 24 30 85")));
	CreateSnowFallFunc();
}

using FX_BuildTeslaZapFn = void(__thiscall*)(CEffectData*);
void FX_BuildTeslaZap(CEffectData* effect_data) {
	static auto fx = reinterpret_cast<FX_BuildTeslaZapFn>(utilities::pattern_scan(("client.dll"), xorstr_("55 8B EC 83 E4 F8 81 EC ? ? ? ? 56 8B 75 08 51 8B CC 8B 46 34 89 01 B9 ? ? ? ? A1 ? ? ? ? FF 50 08 ")));
	fx(effect_data);
}

bool datagram_hooked = false;
bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return create_move_original(input_sample_frametime, cmd);

	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	csgo::cmd = cmd;
	//if (cmd->tick_count == 0)
	//	return create_move_original(input_sample_frametime, cmd);

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C); //0x34

	// basically saying if menu open then make sure you dont knife, shoot or aim.
	if (menu::is_open) {
		cmd->buttons &= ~in_attack;
		cmd->buttons &= ~in_attack2;
	}


	auto net_channel = interfaces::clientstate->net_channel;

	csgo::server_time(cmd);
	misc::main::auto_jumpscout(cmd);

	misc::movement::velo_graph_gather_data();
	misc::movement::stamina_graph_gather_data();

	startMovementFix(cmd);
	misc::movement::quick_stop(cmd);
	legit_bot::autostop(cmd);
	misc::main::ragdoll_gravity();
	misc::main::anti_afk(cmd);
	misc::main::reveal_rank(cmd);
	misc::main::clantag();
	visuals::draw_beams();
	misc::movement::pre_pixelsurf(cmd);
	// for ghetto ass motion blur if you couldn't tell
	mouse_dx = 1.f * (float(cmd->mousedx) / (cmd->mousedx > 0 ? (settings::misc::motion_blur_modifier * 350.f) : (settings::misc::motion_blur_modifier * -350.f)));
	if (misc::movement::will_ps)
		interfaces::effects->EnergySplash(csgo::local_player->origin(), { 0, 0, 0 }, false);
	for (auto hk : lua::hook_manager::get_hooks(xorstr_("createmove"))) {
		// For some stupid reason it decides to fucking explode when you alt tab out of the game>?>?<>><>>?
		// So i've decided to create the most deathtrap, ghetto, dogshit way of fixing it, or well i hope so...
		if (GetForegroundWindow() == menu::hwnd) {
			auto result = hk.func(cmd);
			if (!result.valid()) {
				sol::error err = result;
				interfaces::console->console_color_printf(color::from_float(c::menu_accent), xorstr_("[lua] "));
				interfaces::console->console_printf(std::string(err.what()).append(" \n").c_str());
			}
		}
	}

	// retarded me mo0emnt of handling all of  this like this in here LOL
	CTeslaInfo in_eb_tesla;
	in_eb_tesla.m_flBeamWidth = 3.f;
	in_eb_tesla.m_flRadius = 100.f;
	in_eb_tesla.m_nEntIndex = csgo::local_player->index();
	in_eb_tesla.m_vColor = vec3_t(settings::visulisation::lightning_while_eb_col[0], settings::visulisation::lightning_while_eb_col[0], settings::visulisation::lightning_while_eb_col[0]);
	in_eb_tesla.m_vPos = csgo::local_player->origin();
	in_eb_tesla.m_flTimeVisible = 1.f;
	in_eb_tesla.m_nBeams = 1;
	in_eb_tesla.m_pszSpriteName = "sprites/physbeam.vmt";
	if (misc::movement::should_eb && settings::visulisation::lightning_while_eb && settings::visulisation::visualize_eb)
		FX_Tesla(in_eb_tesla);

	// at eb tesla ;a,dklkaslkdjaskl;djask;djasijdasjhliahdfpfudhwfiludshf
	CTeslaInfo at_eb_tesla;
	at_eb_tesla.m_flBeamWidth = 3.f;
	at_eb_tesla.m_flRadius = 100.f;
	at_eb_tesla.m_nEntIndex = csgo::local_player->index();
	at_eb_tesla.m_vColor = vec3_t(settings::visulisation::lightning_while_eb_col[0], settings::visulisation::lightning_while_eb_col[0], settings::visulisation::lightning_while_eb_col[0]);
	at_eb_tesla.m_vPos = variables::player_origin_on_predicted_eb;
	at_eb_tesla.m_flTimeVisible = 1.f;
	at_eb_tesla.m_nBeams = 1;
	at_eb_tesla.m_pszSpriteName = "sprites/physbeam.vmt";
	if (misc::movement::should_eb && settings::visulisation::lightning_while_eb && settings::visulisation::visualize_eb)
		FX_Tesla(at_eb_tesla);


	if (settings::visuals::hit_effect) {
		if (interfaces::globals->cur_time - variables::hit_cooldown < 1.f) {
			CTeslaInfo t;
			t.m_flBeamWidth = 2.f;
			t.m_flRadius = 75.f;
			t.m_nEntIndex = variables::last_player_hit_index;
			t.m_vColor = vec3_t(1, 1, 1); //Vector.new(1, 1, 1);
			t.m_vPos = variables::last_player_hit_origin;
			t.m_flTimeVisible = 1.f;
			t.m_nBeams = 1;
			t.m_pszSpriteName = "sprites/physbeam.vmt";


			FX_Tesla(t);
		}

	}
	// fps optimisation?
	convar* sv_gravity = interfaces::console->get_convar(xorstr_("sv_gravity"));
	if (sv_gravity->get_float() != 800.f) {
		misc::movement::gravity_addictive = sv_gravity->get_float();
	}
	// move fix
	startMovementFix(cmd);
	misc::movement::bunny_hop(cmd);
	misc::main::door_spammer(cmd);
	misc::movement::no_duck_cooldown(cmd);
	misc::movement::auto_strafe(cmd);
	misc::movement::beam_trail();
	misc::main::viewmodel_offset();
	misc::movement::strafe_optimiser(cmd);
	visuals::nade_pred::trace(cmd);

	int pre_flags = csgo::local_player->flags();
	auto pre_velocity = csgo::local_player->velocity();

	data::called::grab_pre_command_information(cmd);
	

	lag_compensation::run(cmd);

	prediction::backup_originals(cmd);

	prediction::start(cmd); {

		
		//aim_assist::run(cmd);
		misc::movement::mini_jump(cmd, pre_flags);
	} prediction::end();

	endMovementFix(cmd);
	

	data::called::grab_post_command_information(cmd);
	legit_bot::aimbot(cmd);
	legit_bot::trigger(cmd);
	misc::movement::is_jumpbugging();
	misc::movement::jumpbug(cmd);

	if (misc::movement::successful_jb) {
		interfaces::chat_element->chat_print_f(std::format(xorstr_("\x08 \x0C divinity \x08~ jumpbugged")).data());
	}
	int post_flags = csgo::local_player->flags();
	auto post_velocity = csgo::local_player->velocity();

	if (settings::aimbot::backtrack_enable && settings::aimbot::backtrack_ping_spike)
		lag_compensation::update_incoming_sequences(net_channel);
	else
		lag_compensation::clear_incoming_sequences();

	if (utilities::do_forceupdate) {
		utilities::force_update();
		utilities::do_forceupdate = false;
	}

	misc::movement::auto_ladder_bug(cmd);

	misc::movement::auto_duck(cmd);

	misc::movement::pixel_surf(cmd);

	if (misc::movement::in_edgebug && settings::indicators::eb)
		variables::eb_ind_alpha = 1.f;

	if (misc::movement::successful_jb && settings::indicators::jb)
		variables::jb_ind_alpha = 1.f;

	if (misc::movement::successful_ej && settings::indicators::ej)
		variables::ej_ind_alpha = 1.f;

	if (misc::movement::successful_lb && settings::indicators::lb)
		variables::lb_ind_alpha = 1.f;

	float deltatime = (1.0f / 0.2f) * interfaces::globals->frame_time * 0.7f;
	variables::eb_ind_alpha = std::fmax(0.f, variables::eb_ind_alpha - 1.f * deltatime);
	variables::jb_ind_alpha = std::fmax(0.f, variables::jb_ind_alpha - 1.f * deltatime);
	variables::ej_ind_alpha = std::fmax(0.f, variables::ej_ind_alpha - 1.f * deltatime);
	variables::lb_ind_alpha = std::fmax(0.f, variables::lb_ind_alpha - 1.f * deltatime);

	misc::movement::edgejump(cmd, pre_flags, csgo::local_player->flags());

	misc::movement::long_jump(cmd);

	// view_backup will be for restoring because if you don't restore you literlaly spin like a mf
	auto view_backup = cmd->viewangles;
	misc::movement::edgebug_run(cmd);
	startMovementFix(cmd);
	cmd->viewangles = view_backup;
	endMovementFix(cmd);

	// retarded sanity checks that make me want to shoot myself
	if (!misc::movement::should_eb && csgo::local_player->flags() & fl_onground && pre_flags & fl_onground) {
		misc::movement::strafe_assisted = false;
		misc::movement::in_edgebug = false;
	}

	// clamping everything to make sure nothing explodes uwu
	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	// normal it bro....
	normalizeAngles(cmd->viewangles);
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	// used for edgebug
	misc::movement::viewangles = cmd->viewangles;

	//prediction::start(cmd);
	//prediction::end();
	//interfaces::prediction->Repredict();

	return false;
}
float init_alpha = 1.f;
void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	// this is useless who the fuck is gonna use fucking paint traverse to render anything apart from maybe effects?

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		rendering::initialize();
		notifications::draw();
		break;
	case fnv::hash("FocusOverlayPanel"):
		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

void __stdcall hooks::scene_end::hook() {

	// nice.
	scene_end_original(interfaces::render_view);
}

void __stdcall hooks::lock_cursor::hook() {

	if (menu::is_open) {
		interfaces::surface->unlock_cursor();
		if (!interfaces::engine->is_in_game())
			interfaces::inputsystem->enable_input(false);
		return;
	}
	else {
		if (!interfaces::engine->is_in_game())
			interfaces::inputsystem->enable_input(false);
		interfaces::inputsystem->enable_input(true);
	}

	return lock_cursor_original(interfaces::surface);
}

void __fastcall hooks::emit_sound::hook(void* ecx, void* edx, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const vec3_t* pOrigin, const vec3_t* pDirection, vec3_t* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, void* fds) {
	if (settings::misc::auto_accept)
		if (!strcmp(pSoundEntry, ("UIPanorama.popup_accept_match_beep"))) {
			static auto SetLocalPlayerReady = reinterpret_cast<bool(__stdcall*)(const char*)>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));

			if (SetLocalPlayerReady) {
				// setting it to "" basically removes tthe non checked state so actually by doing this we're accepting the match but you won't actually see it
				// as its server sided and not client sided. could add little hud for accepting matches n shit would be very cool
				SetLocalPlayerReady("");


				// FLASH DA WIN INFO CUH
				FLASHWINFO fi;
				fi.cbSize = sizeof(FLASHWINFO);
				fi.hwnd = menu::hwnd;
				fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
				fi.uCount = 0;
				fi.dwTimeout = 0;
				FlashWindowEx(&fi);
			}
		}

	// this is mostly used for edgebug honestly, if you're in preediction it wont return the opriginal blocking out the sounds
	if (!prediction::in_prediction)
		emit_sound_original(ecx, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, fds);
}

void __stdcall hooks::dme::hook(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {
	if (interfaces::model_render->is_forced_material_override())
		return draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);

	/// Big shoutout to my big russian paster friends at legendware, by using the legendware v3 source i was able to "borrow" this retarded check that fixed weapon chams
	float white[4] = { 1.f, 1.f, 1.f, 1.f };
	auto is_player = settings::visuals::enable_chams_vis && strstr(info.model->name, xorstr_("models/player"));
	auto is_weapon = settings::visuals::weapon_chams && strstr(info.model->name, xorstr_("weapons/v_")) && !strstr(info.model->name, xorstr_("arms"));
	auto is_arms = settings::visuals::arm_chams && strstr(info.model->name, xorstr_("arms"));
	auto is_sleeve = settings::visuals::arm_chams && strstr(info.model->name, xorstr_("sleeve"));

	if (interfaces::model_render->is_forced_material_override() && !is_weapon && is_arms && !is_sleeve)
		return draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);

	interfaces::render_view->modulate_color(white);
	if (is_arms && is_sleeve && !is_weapon)
		visuals::chams::run_local(ctx, state, info, bone_to_world);
	if (is_weapon && !is_arms && !is_sleeve)
		visuals::chams::run_weapons(ctx, state, info, bone_to_world);
	if (is_player)
		visuals::chams::run(ctx, state, info, bone_to_world);

	draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
	interfaces::model_render->override_material(NULL);
}

void __fastcall hooks::override_mouse_input::hook(void* ecx_, void* edx_, float* x, float* y) {


	if (aim_assist::is_targeting) {
		*x *= 0.4f;
		*y *= 0.4f;

		aim_assist::is_targeting = false;
	}
	// basic ass locking, yes ik its stupid to lock pitch but some people may like it so why the fuck not
	if (misc::movement::should_eb && settings::movement::lock_on_eb) {
		*x *= (1.f - (settings::movement::lock_pull_amount / 100.f));
		if (settings::movement::lock_pitch)
			*y = (1.f - (settings::movement::lock_pull_amount / 100.f));
	}
}

static int last_tick;
void __stdcall hooks::frame_stage_notify::hook(int frame_stage) {
	static auto lag_compensation_init = (lag_compensation::init(), false);
	if (!interfaces::engine->is_in_game()) {
		lag_compensation::clear_incoming_sequences();
		return fsn_original(interfaces::client, frame_stage);
	}

	if (interfaces::engine->is_in_game())
		lag_compensation::update(frame_stage);
	if (frame_stage == FRAME_RENDER_START) {
		if (csgo::local_player) {
			//	misc::main::modulate_hud();
		}
	}

	/*if (settings::visuals::chams_vis) {
		for (size_t i = 0; i < interfaces::entity_list->get_highest_index(); i++) {
			player_t* player = (player_t*)interfaces::entity_list->get_client_entity(i);

			if (!player)
				continue;

			if (!player->is_alive())
				continue;
			for (size_t patch_index = 0; patch_index < 5; patch_index++)
				player->player_patch_econ_indices()[patch_index] = vec3_t(0, 0, 0);
		}
	}*/


	// Monopoly - "your remove smoke is pasted" - "i seen this exact code from 5 other sources"
	// Monopoly you're fucking stupid and i hate u, how do you paste the most basic shit
	// funnily enough 90% of the game IS LITERALLY OPEN SOURCE
	if (frame_stage == FRAME_RENDER_START || frame_stage == FRAME_RENDER_END) {
		std::array smoke_materials{ "particle/vistasmokev1/vistasmokev1_emods", "particle/vistasmokev1/vistasmokev1_emods_impactdust","particle/vistasmokev1/vistasmokev1_fire","particle/vistasmokev1/vistasmokev1_smokegrenade" };

		for (const auto p_mat : smoke_materials) {
			i_material* selected_material = interfaces::material_system->find_material(p_mat, TEXTURE_GROUP_OTHER);
			selected_material->set_material_var_flag(material_var_wireframe, settings::visuals::wireframe_smoke);
		}

		std::array molotov_materials{ "particle/fire_burning_character/fire_env_fire","particle/fire_burning_character/fire_env_fire_depthblend","particle/fire_burning_character/fire_burning_character_depthblend","particle/fire_burning_character/fire_burning_character""particle/fire_burning_character/fire_burning_character_nodepth""particle/particle_flares/particle_flare_001","particle/particle_flares/particle_flare_004","particle/particle_flares/particle_flare_004b_mod_ob","particle/particle_flares/particle_flare_004b_mod_z","particle/fire_explosion_1/fire_explosion_1_bright","particle/fire_explosion_1/fire_explosion_1b","particle/fire_particle_4/fire_particle_4","particle/fire_explosion_1/fire_explosion_1_oriented" };

		for (const auto p_mat : molotov_materials) {
			i_material* selected_material = interfaces::material_system->find_material(p_mat, TEXTURE_GROUP_OTHER);
			selected_material->set_material_var_flag(material_var_wireframe, settings::visuals::wireframe_fire);
		}
	}

	if (frame_stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		changers::skins::run();
	}
	// TODO: dont use cvars you fucking moron its detected LOL, hf with ur bans hahaahahha hahaha
	if (frame_stage == FRAME_RENDER_END) {
		visuals::fog_modulation();
	}

	// this barely works btw, no point pasting it
	//visuals::skybox_changer();

	fsn_original(interfaces::client, frame_stage);
}

int __stdcall hooks::do_post_screen_effects::hook(c_view_setup& value) {
	visuals::glow();

	return dpse_original(interfaces::clientmode, value);
}

void __fastcall hooks::get_color_modulation::hook(void* ecx, void* edx, float* r, float* g, float* b) {
	get_color_modulation_original(ecx, r, g, b);


	const auto material = reinterpret_cast<i_material*>(ecx);

	if (!material || material->is_error_material())
		return get_color_modulation_original(ecx, r, g, b);

	const auto group = fnv::hash(material->get_texture_group_name());

	if (group != fnv::hash(("World textures")) && group != fnv::hash(("StaticProp textures"))
		&& (group != fnv::hash(("SkyBox textures"))))
		return get_color_modulation_original(ecx, r, g, b);

	bool is_prop = (group == fnv::hash(("StaticProp textures")));
	if (!is_prop && settings::visuals::world_modulation) {
		*r *= settings::visuals::world_modulation_clr[0];
		*g *= settings::visuals::world_modulation_clr[1];
		*b *= settings::visuals::world_modulation_clr[2];
	}
	if (is_prop && settings::visuals::prop_modulation) {
		*r *= settings::visuals::prop_modulation_clr[0];
		*g *= settings::visuals::prop_modulation_clr[1];
		*b *= settings::visuals::prop_modulation_clr[2];
	}
}

bool __stdcall hooks::is_using_static_prop_debug_modes::hook() {
	return settings::visuals::world_modulation || settings::visuals::prop_modulation;
}

int __fastcall hooks::send_datagram::hook(i_net_channel_t* net_channel, int edx, bf_write* datagram) {
	if (!interfaces::engine)
		return datagram_original(net_channel, edx, datagram);

	if (!interfaces::engine->is_in_game() || !settings::aimbot::backtrack_enable || !settings::aimbot::backtrack_ping_spike || datagram != nullptr)
		return datagram_original(net_channel, edx, datagram);

	int state = net_channel->in_reliable_state;
	int sequence = net_channel->in_sequence_nr;

	lag_compensation::add_latency_to_net_channel(net_channel, settings::aimbot::backtrack_ping_spike_value);

	int _return = datagram_original(net_channel, edx, datagram);

	net_channel->in_reliable_state = state;
	net_channel->in_sequence_nr = sequence;


	return _return;
}

void __fastcall hooks::hk_particle_collection_simulate::hook(CParticleCollection* thisPtr, void* edx) {
	if (!interfaces::engine->is_connected()) {
		pcs_original(thisPtr, edx);
		return;
	}

	pcs_original(thisPtr, edx);

	CParticleCollection* root_colection = thisPtr;
	while (root_colection->m_pParent)
		root_colection = root_colection->m_pParent;

	const char* root_name = root_colection->m_pDef.m_pObject->m_Name.buffer;

	if (settings::visuals::modulate_molotov) {
		switch (fnv::hash(root_name)) {
		case fnv::hash("molotov_groundfire"):
		case fnv::hash("molotov_groundfire_00MEDIUM"):
		case fnv::hash("molotov_groundfire_00HIGH"):
		case fnv::hash("molotov_groundfire_fallback"):
		case fnv::hash("molotov_groundfire_fallback2"):
		case fnv::hash("molotov_explosion"):
		case fnv::hash("explosion_molotov_air"):
		case fnv::hash("extinguish_fire"):
		case fnv::hash("weapon_molotov_held"):
		case fnv::hash("weapon_molotov_fp"):
		case fnv::hash("weapon_molotov_thrown"):
		case fnv::hash("incgrenade_thrown_trail"):
			switch (fnv::hash(thisPtr->m_pDef.m_pObject->m_Name.buffer)) {
			case fnv::hash("explosion_molotov_air_smoke"):
			case fnv::hash("molotov_smoking_ground_child01"):
			case fnv::hash("molotov_smoking_ground_child02"):
			case fnv::hash("molotov_smoking_ground_child02_cheapo"):
			case fnv::hash("molotov_smoking_ground_child03"):
			case fnv::hash("molotov_smoking_ground_child03_cheapo"):
			case fnv::hash("molotov_smoke_screen"):
				if (settings::visuals::modulate_smoke) {
					for (int i = 0; i < thisPtr->m_nActiveParticles; i++) {
						float* pColor = thisPtr->m_ParticleAttributes.FloatAttributePtr(PARTICLE_ATTRIBUTE_ALPHA, i);
						*pColor = 0.f;
					}
				}
				break;
			default:
				for (int i = 0; i < thisPtr->m_nActiveParticles; i++) {
					float* pColor = thisPtr->m_ParticleAttributes.FloatAttributePtr(PARTICLE_ATTRIBUTE_TINT_RGB, i);
					pColor[0] = settings::visuals::modulate_molotov_clr[0];
					pColor[4] = settings::visuals::modulate_molotov_clr[1];
					pColor[8] = settings::visuals::modulate_molotov_clr[2];
				}
				break;
			}
			break;
		}
	}

	if (settings::visuals::modulate_smoke) {
		switch (fnv::hash(root_name)) {
		case fnv::hash("explosion_smokegrenade"):
		case fnv::hash("explosion_smokegrenade_fallback"):

			for (int i = 0; i < thisPtr->m_nActiveParticles; i++) {
				float* pColor = thisPtr->m_ParticleAttributes.FloatAttributePtr(PARTICLE_ATTRIBUTE_TINT_RGB, i);
				pColor[0] = settings::visuals::modulate_smoke_clr[0];
				pColor[4] = settings::visuals::modulate_smoke_clr[1];
				pColor[8] = settings::visuals::modulate_smoke_clr[2];
			}
			break;
		}
	}
	// this is where it gets worse LOL
	if (settings::visuals::modulate_blood) {
		switch (fnv::hash(root_name)) {
		case fnv::hash("blood_impact_basic"):
		case fnv::hash("blood_impact_basic_fallback"):
		case fnv::hash("blood_impact_chunks1"):
		case fnv::hash("blood_impact_drops1"):
		case fnv::hash("blood_impact_dust"):
		case fnv::hash("blood_impact_goop_heavy"):
		case fnv::hash("blood_impact_goop_light"):
		case fnv::hash("blood_impact_goop_medium"):
		case fnv::hash("blood_impact_green_01"):
		case fnv::hash("blood_impact_headshot"):
		case fnv::hash("blood_impact_headshot_01b"):
		case fnv::hash("blood_impact_headshot_01c"):
		case fnv::hash("blood_impact_headshot_01d"):
		case fnv::hash("blood_impact_headshot_01e"):
		case fnv::hash("blood_impact_heavy"):
		case fnv::hash("blood_impact_light"):
		case fnv::hash("blood_impact_light_headshot"):
		case fnv::hash("blood_impact_medium"):
		case fnv::hash("blood_impact_mist1"):
		case fnv::hash("blood_impact_mist1_light"):
		case fnv::hash("blood_impact_mist_heavy"):
		case fnv::hash("blood_impact_red_01"):
		case fnv::hash("blood_impact_red_01_backspray"):
		case fnv::hash("blood_impact_red_01_chunk"):
		case fnv::hash("blood_impact_red_01_drops"):
		case fnv::hash("blood_impact_red_01_goop_a"):
		case fnv::hash("blood_impact_red_01_goop_a_backup"):
		case fnv::hash("blood_impact_red_01_goop_b"):
		case fnv::hash("blood_impact_red_01_goop_c"):
		case fnv::hash("blood_impact_red_01_mist"):
		case fnv::hash("blood_impact_yellow_01"):
		case fnv::hash("blood_pool"):
		case fnv::hash("explosion_screen_c4_blood"):
		case fnv::hash("explosion_screen_c4_blood2"):
		case fnv::hash("explosion_screen_c4_blood_OLD"):
		case fnv::hash("explosion_screen_c4_blood_distance_culled"):
		case fnv::hash("impact_blood_glow"):
		case fnv::hash("impact_blood_glow_light"):
		case fnv::hash("impact_blood_glow_light_master"):
		case fnv::hash("impact_blood_glow_normal_master"):
		case fnv::hash("screen_blood_splatter"):
		case fnv::hash("screen_blood_splatter_a"):
		case fnv::hash("screen_blood_splatter_b"):


			for (int i = 0; i < thisPtr->m_nActiveParticles; i++) {
				float* pColor = thisPtr->m_ParticleAttributes.FloatAttributePtr(PARTICLE_ATTRIBUTE_TINT_RGB, i);
				pColor[0] = settings::visuals::modulate_blood_clr[0];
				pColor[4] = settings::visuals::modulate_blood_clr[1];
				pColor[8] = settings::visuals::modulate_blood_clr[2];
			}
			break;
		}
	}

	if (settings::visuals::modulate_weapon_effects) {
		switch (fnv::hash(root_name)) {
		case fnv::hash("weapon_muzzle_flash_assaultrifle"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_FP"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_fallback"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_fallback2"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_glow"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_main"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_main_FP"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_main_sub1"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_main_sub2"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_main_sub3"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_parent_silenced"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_silenced"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_silenced_glow"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_vent"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_vent_FP"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_vent_FP_cheap"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_vent_fallback"):
		case fnv::hash("weapon_muzzle_flash_assaultrifle_vent_fire"):
		case fnv::hash("weapon_muzzle_flash_assualtrifle_flame"):
		case fnv::hash("weapon_muzzle_flash_autoshotgun"):
		case fnv::hash("weapon_muzzle_flash_autoshotgun_FP"):
		case fnv::hash("weapon_muzzle_flash_autoshotgun_FP_cheap"):
		case fnv::hash("weapon_muzzle_flash_autoshotgun_fallback"):
		case fnv::hash("weapon_muzzle_flash_autoshotgun_fallback2"):
		case fnv::hash("weapon_muzzle_flash_autoshotgun_glow"):
		case fnv::hash("weapon_muzzle_flash_autoshotgun_main"):
		case fnv::hash("weapon_muzzle_flash_autoshotgun_main2"):
		case fnv::hash("weapon_muzzle_flash_autoshotgun_main3"):
		case fnv::hash("weapon_muzzle_flash_awp"):
		case fnv::hash("weapon_muzzle_flash_awp_tracer"):
		case fnv::hash("weapon_muzzle_flash_huntingrifle"):
		case fnv::hash("weapon_muzzle_flash_huntingrifle_FP"):
		case fnv::hash("weapon_muzzle_flash_huntingrifle_FP_cheap"):
		case fnv::hash("weapon_muzzle_flash_huntingrifle_glow"):
		case fnv::hash("weapon_muzzle_flash_huntingrifle_main"):
		case fnv::hash("weapon_muzzle_flash_huntingrifle_main2"):
		case fnv::hash("weapon_muzzle_flash_huntingrifle_main_fallback"):
		case fnv::hash("weapon_muzzle_flash_huntingrifle_vent1"):
		case fnv::hash("weapon_muzzle_flash_para"):
		case fnv::hash("weapon_muzzle_flash_para_FP"):
		case fnv::hash("weapon_muzzle_flash_para_FP_cheap"):
		case fnv::hash("weapon_muzzle_flash_para_b"):
		case fnv::hash("weapon_muzzle_flash_para_fallback"):
		case fnv::hash("weapon_muzzle_flash_para_fallback2"):
		case fnv::hash("weapon_muzzle_flash_para_flash"):
		case fnv::hash("weapon_muzzle_flash_para_glow"):
		case fnv::hash("weapon_muzzle_flash_para_main"):
		case fnv::hash("weapon_muzzle_flash_para_main2"):
		case fnv::hash("weapon_muzzle_flash_para_main_fallback"):
		case fnv::hash("weapon_muzzle_flash_para_tracers"):
		case fnv::hash("weapon_muzzle_flash_para_vent1"):
		case fnv::hash("weapon_muzzle_flash_pistol"):
		case fnv::hash("weapon_muzzle_flash_pistol_FP"):
		case fnv::hash("weapon_muzzle_flash_pistol_FP_cheap"):
		case fnv::hash("weapon_muzzle_flash_pistol_core"):
		case fnv::hash("weapon_muzzle_flash_pistol_core2"):
		case fnv::hash("weapon_muzzle_flash_pistol_elite"):
		case fnv::hash("weapon_muzzle_flash_pistol_elite_FP"):
		case fnv::hash("weapon_muzzle_flash_pistol_fallback"):
		case fnv::hash("weapon_muzzle_flash_pistol_fallback2"):
		case fnv::hash("weapon_muzzle_flash_pistol_main"):
		case fnv::hash("weapon_muzzle_flash_pistol_main2"):
		case fnv::hash("weapon_muzzle_flash_pistol_main2_silenced"):
		case fnv::hash("weapon_muzzle_flash_pistol_silenced"):
		case fnv::hash("weapon_muzzle_flash_shotgun"):
		case fnv::hash("weapon_muzzle_flash_shotgun_FP"):
		case fnv::hash("weapon_muzzle_flash_shotgun_FP_cheap"):
		case fnv::hash("weapon_muzzle_flash_shotgun_glow"):
		case fnv::hash("weapon_muzzle_flash_shotgun_glow2"):
		case fnv::hash("weapon_muzzle_flash_shotgun_glow3"):
		case fnv::hash("weapon_muzzle_flash_shotgun_main"):
		case fnv::hash("weapon_muzzle_flash_shotgun_main2"):
		case fnv::hash("weapon_muzzle_flash_smg"):
		case fnv::hash("weapon_muzzle_flash_smg_FP"):
		case fnv::hash("weapon_muzzle_flash_smg_FP_cheap"):
		case fnv::hash("weapon_muzzle_flash_smg_backup"):
		case fnv::hash("weapon_muzzle_flash_smg_fallback"):
		case fnv::hash("weapon_muzzle_flash_smg_fallback2"):
		case fnv::hash("weapon_muzzle_flash_smg_glow"):
		case fnv::hash("weapon_muzzle_flash_smg_glow2"):
		case fnv::hash("weapon_muzzle_flash_smg_glow3"):
		case fnv::hash("weapon_muzzle_flash_smg_glow_silenced"):
		case fnv::hash("weapon_muzzle_flash_smg_main"):
		case fnv::hash("weapon_muzzle_flash_smg_main2"):
		case fnv::hash("weapon_muzzle_flash_smg_main_silenced"):
		case fnv::hash("weapon_muzzle_flash_smg_parent"):
		case fnv::hash("weapon_muzzle_flash_smg_parent_silenced"):
		case fnv::hash("weapon_muzzle_flash_smg_silenced"):
		case fnv::hash("weapon_muzzle_flash_smg_silenced_FP"):
		case fnv::hash("weapon_muzzle_flash_smg_silenced_FP_cheap"):
		case fnv::hash("weapon_muzzle_flash_smoke_medium"):
		case fnv::hash("weapon_muzzle_flash_smoke_small"):
		case fnv::hash("weapon_muzzle_flash_smoke_small2"):
		case fnv::hash("weapon_muzzle_flash_smoke_small3"):
		case fnv::hash("weapon_muzzle_flash_smoke_small4"):
		case fnv::hash("weapon_muzzle_flash_smoke_smallest"):
		case fnv::hash("weapon_muzzle_flash_sparks"):
		case fnv::hash("weapon_muzzle_flash_sparks2"):
		case fnv::hash("weapon_muzzle_flash_sparks3"):
		case fnv::hash("weapon_muzzle_flash_sparks4"):
		case fnv::hash("weapon_muzzle_flash_sparks5"):
		case fnv::hash("weapon_muzzle_flash_sparks6"):
		case fnv::hash("weapon_muzzle_flash_sparks7"):
		case fnv::hash("weapon_muzzle_flash_taser"):
		case fnv::hash("weapon_muzzle_flash_taser_fallback"):
		case fnv::hash("weapon_muzzle_flash_taser_fallback2"):
		case fnv::hash("weapon_muzzle_smoke"):
		case fnv::hash("weapon_muzzle_smoke_b"):
		case fnv::hash("weapon_muzzle_smoke_bVersion#2"):
		case fnv::hash("weapon_muzzle_smoke_long#2"):
		case fnv::hash("weapon_muzzle_smoke_long_b#2"):
		case fnv::hash("weapon_muzzleflash_illumination"):
		case fnv::hash("weapon_muzzleflash_illumination_add"):
		case fnv::hash("weapon_sensorgren_beeplight"):
		case fnv::hash("weapon_sensorgren_beeplight_inner"):
		case fnv::hash("weapon_sensorgren_debris"):
		case fnv::hash("weapon_sensorgren_detlight"):
		case fnv::hash("weapon_sensorgren_detlight_child01"):
		case fnv::hash("weapon_sensorgren_detlight_child02"):
		case fnv::hash("weapon_sensorgren_detonate"):
		case fnv::hash("weapon_sensorgren_glow"):
		case fnv::hash("weapon_sensorgren_glowring"):
		case fnv::hash("weapon_sensorgren_smoke"):
		case fnv::hash("weapon_sensorgren_spark_01"):
		case fnv::hash("weapon_sensorgren_spark_02"):
		case fnv::hash("weapon_sensorgren_spark_02b"):
		case fnv::hash("weapon_sensorgren_spark_03"):
		case fnv::hash("weapon_sensorgren_wiggly"):
		case fnv::hash("weapon_shell_casing_50cal"):
		case fnv::hash("weapon_shell_casing_50cal_bonus"):
		case fnv::hash("weapon_shell_casing_50cal_fallback"):
		case fnv::hash("weapon_shell_casing_9mm"):
		case fnv::hash("weapon_shell_casing_9mm_backup"):
		case fnv::hash("weapon_shell_casing_9mm_fallback"):
		case fnv::hash("weapon_shell_casing_9mm_fp"):
		case fnv::hash("weapon_shell_casing_9mm_fp_fallback"):
		case fnv::hash("weapon_shell_casing_candycorn"):
		case fnv::hash("weapon_shell_casing_minigun"):
		case fnv::hash("weapon_shell_casing_minigun_fallback"):
		case fnv::hash("weapon_shell_casing_rifle"):
		case fnv::hash("weapon_shell_casing_rifle_fallback"):
		case fnv::hash("weapon_shell_casing_shotgun"):
		case fnv::hash("weapon_shell_casing_shotgun_fallback"):
		case fnv::hash("weapon_shell_casing_super_trail"):
		case fnv::hash("weapon_shell_casing_super_trail_base"):
		case fnv::hash("weapon_shell_casing_super_trail_glow"):
		case fnv::hash("weapon_shell_casing_super_trail_smoke"):
		case fnv::hash("weapon_shell_casing_super_trail_sparks"):
		case fnv::hash("weapon_shell_casing_super_trail_sparks2"):
		case fnv::hash("weapon_shell_eject_smoke_assrifle2"):
		case fnv::hash("weapon_shell_eject_smoke_assrifle3"):
		case fnv::hash("weapon_shell_eject_smoke_awp2"):
		case fnv::hash("weapon_shell_eject_smoke_awp3"):
		case fnv::hash("weapon_shell_eject_smoke_para2"):
		case fnv::hash("weapon_shell_eject_smoke_para3"):
		case fnv::hash("weapon_shell_eject_smoke_pistol1"):
		case fnv::hash("weapon_shell_eject_smoke_pistol2"):
		case fnv::hash("weapon_shell_eject_smoke_pistol3"):
		case fnv::hash("weapon_shell_eject_smoke_pistol_elite"):
		case fnv::hash("weapon_shell_eject_smoke_rifle2"):
		case fnv::hash("weapon_shell_eject_smoke_rifle3"):
		case fnv::hash("weapon_shell_eject_smoke_shotgun1"):
		case fnv::hash("weapon_shell_eject_smoke_shotgun2"):
		case fnv::hash("weapon_shell_eject_smoke_shotgun3"):
		case fnv::hash("weapon_shell_eject_smoke_smg1"):
		case fnv::hash("weapon_shell_eject_smoke_smg3"):
		case fnv::hash("weapon_taser_flash"):
		case fnv::hash("weapon_taser_flash_impact"):
		case fnv::hash("weapon_taser_glow"):
		case fnv::hash("weapon_taser_glow_impact"):
		case fnv::hash("weapon_taser_sparks"):
		case fnv::hash("weapon_taser_sparks_impact"):
		case fnv::hash("weapon_tracers"):
		case fnv::hash("weapon_tracers_50cal"):
		case fnv::hash("weapon_tracers_50cal_glow"):
		case fnv::hash("weapon_tracers_50cal_low"):
		case fnv::hash("weapon_tracers_50cal_low_glow"):
		case fnv::hash("weapon_tracers_assrifle"):
		case fnv::hash("weapon_tracers_backup"):
		case fnv::hash("weapon_tracers_mach"):
		case fnv::hash("weapon_tracers_original"):
		case fnv::hash("weapon_tracers_pistol"):
		case fnv::hash("weapon_tracers_rifle"):
		case fnv::hash("weapon_tracers_shot"):
		case fnv::hash("weapon_tracers_smg"):
		case fnv::hash("weapon_tracers_taser"):
		case fnv::hash("weapon_tracers_taser_fallback"):
		case fnv::hash("weapon_tracers_taser_fallback2"):
		case fnv::hash("weapon_tracers_taser_wire1a"):
		case fnv::hash("weapon_tracers_taser_wire1b"):
		case fnv::hash("weapon_tracers_taser_wire2"):

		case fnv::hash("ricochet_sparks"):
		case fnv::hash("ricochet_sparks_contrast_glow"):
		case fnv::hash("ricochet_sparks_contrast_glow_soft"):
		case fnv::hash("ricochet_sparks_contrast_shadow"):
		case fnv::hash("effects/tesla_glow_noz"):

			for (int i = 0; i < thisPtr->m_nActiveParticles; i++) {
				float* pColor = thisPtr->m_ParticleAttributes.FloatAttributePtr(PARTICLE_ATTRIBUTE_TINT_RGB, i);
				pColor[0] = settings::visuals::modulate_weapon_effects_clr[0];
				pColor[4] = settings::visuals::modulate_weapon_effects_clr[1];
				pColor[8] = settings::visuals::modulate_weapon_effects_clr[2];
			}
			break;

		}
	}
}


// POV: pasted from valve.
#define FRAMERATE_AVG_FRAC 0.9
#define PACKETLOSS_AVG_FRAC 0.5
#define PACKETCHOKE_AVG_FRAC 0.5
#define GRAPH_RED	(0.9 * 255)
#define GRAPH_GREEN (0.9 * 255)
#define GRAPH_BLUE	(0.7 * 255)
color textColorDefault = color(GRAPH_RED, GRAPH_GREEN, GRAPH_BLUE, 255);
void __fastcall hooks::draw_text_fields::hook(void* ecx, void* edx, const wchar_t* text, int text_length, void* draw_type) {
	if (settings::misc::custom_netgraph) {
		if (wcsstr(text, L"fps:")) {
			wchar_t buf[128] = L"[divinity]  ";
			if (settings::misc::modulate_netgraph)
				interfaces::surface->set_text_color(c::menu_accent[0] * 255, c::menu_accent[1] * 255, c::menu_accent[2] * 255);
			wcscat_s(buf, text);
			return dtf_original(ecx, edx, buf, wcslen(buf), draw_type);
		}
	}
	dtf_original(ecx, edx, text, text_length, draw_type);
}

// these comments make me laugh
bool __fastcall hooks::send_net_message::hook(i_net_channel* this_ptr, void* edx, INetMessage& msg, bool force_reliable, bool voice) {

	if (msg.GetType() == 14) // blocks crc file check uwu
		return false;

	if (msg.GetGroup() == 9) // Fix lag when transmitting voice and fakelagging
		voice = true;

	return snm_original(this_ptr, edx, msg, force_reliable, voice);
}
// POV: pasted from valve again...
//-----------------------------------------------------------------------------
// Purpose: Draws overlay text fields showing framerate, latency, bandwidth breakdowns, 
//  and, optionally, packet loss and choked packet percentages
// Input  : graphvalue - 
//			x - 
//			y - 
//			*graph - 
//			*cmdinfo - 
//			count - 
//			avg - 
//			*framerate - 
//			0.0 - 
//			avg - 
//-----------------------------------------------------------------------------
/// Because this is an thiscall it needs it's ecx pointer or void* thisptr for this example.
void __fastcall hooks::DrawTextFields::hook(void* thisptr, void* edx, int x, int y, int w, void* graph, void* cmdinfo) {

	y = 10;

	return DTF_original(thisptr, edx, x, y, w, graph, cmdinfo);
}


bool __fastcall hooks::LooseFileAllowed::hook(void* edx, void* ecx) {
	// returning true allows the custom material files aka loose files as they're not in ownership or actually a part of main csgo files.
	// basically if loose file allowed allow chammy wammy uwu
	return true;
}

// assembly mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
#ifdef _WIN32
#undef xor
#define DRAW_SCREEN_EFFECT(material) \
{ \
    const auto drawFunction = interfaces::drawScreenEffectMaterial; \
    int w, h; \
    interfaces::engine->get_screen_size(w, h); \
    __asm { \
        __asm push h \
        __asm push w \
        __asm push 0 \
        __asm xor edx, edx \
        __asm mov ecx, material \
        __asm call drawFunction \
        __asm add esp, 12 \
    } \
}

#else
#define DRAW_SCREEN_EFFECT(material) \
{ \
    int w, h; \
    interfaces::engine->get_screen_size(w, h); \
    reinterpret_cast<void(*)(Material*, int, int, int, int)>(interfaces::drawScreenEffectMaterial)(material, 0, 0, w, h); \
}
#endif

#include "../features/game_data/game_data.h"

void __stdcall hooks::Render2dEffectsPreHud::hook(void* viewSetup) {

	render2deffects_original(viewSetup);
	misc::can_do_glow = settings::edgebug_effects::effect_on_eb != 2 && !settings::edgebug_effects::eb_effect;

	std::array effects = { ("effects/healthboost"), ("effects/dangerzone_screen"), ("dev/bloomadd") };
	if (!settings::edgebug_effects::eb_effect)
		return;

	i_material* material_effect = interfaces::material_system->find_material(effects.at(settings::edgebug_effects::effect_on_eb), nullptr);
	int w, h;
	interfaces::engine->get_screen_size(w, h);

	misc::effect_alpha -= ((1.0 / 0.5f) * interfaces::globals->frame_time);

	misc::effect_alpha = std::clamp(misc::effect_alpha, 0.f, 10.f);
	if (misc::effect_alpha < 0.05f) {
		misc::effect_alpha = 0.f;
		return;
	}

	bool found;
	material_effect->find_var("$c0_x", &found, true)->set_float_value(misc::effect_alpha);

	DRAW_SCREEN_EFFECT(material_effect)

}

// thanks sexy mao uwu!!!!!!!
// another note kys mao LOL
void __fastcall hooks::set_visuals_data::set_visuals_data(void* ecx, void* edx, const char* shader_name)
{
	set_visuals_data_original(ecx, edx, shader_name);
	auto m_visualsData = reinterpret_cast<WeaponVisualData_t*>((uintptr_t(ecx) - 0x4));
	std::string buffer;

	if (!m_visualsData)
		return;

	for (int i = 0; i <= 260; i++)
		buffer.append(&m_visualsData->weapon_path[i]);

	if (!strstr(buffer.c_str(), xorstr_("knife")))
		return;

	if (!settings::skins::knife_changer)
		return;

	if (settings::skins::knife_model == 0)
		return;

	if (!settings::skins::modulate_knife)
		return;


	// ah yes, r = blue!
	// ah yes, g = red!
	// ah yes, b = green!
	m_visualsData->color1_r = settings::skins::knife_color1[2] * 255;
	m_visualsData->color1_g = settings::skins::knife_color1[0] * 255;
	m_visualsData->color1_b = settings::skins::knife_color1[1] * 255;

	m_visualsData->color2_r = settings::skins::knife_color2[2] * 255;
	m_visualsData->color2_g = settings::skins::knife_color2[0] * 255;
	m_visualsData->color2_b = settings::skins::knife_color2[1] * 255;

	m_visualsData->color3_r = settings::skins::knife_color3[2] * 255;
	m_visualsData->color3_g = settings::skins::knife_color3[0] * 255;
	m_visualsData->color3_b = settings::skins::knife_color3[1] * 255;

	m_visualsData->color4_r = settings::skins::knife_color4[2] * 255;
	m_visualsData->color4_g = settings::skins::knife_color4[0] * 255;
	m_visualsData->color4_b = settings::skins::knife_color4[1] * 255;
}

void __fastcall hooks::supress_lists::hook(void* _this, int, int a2, bool a3) {
	if (_ReturnAddress() == interfaces::frame_render_start)
	{

	}

	surpress_lists_original(interfaces::spatial_partition, 0, a2, a3);
}

void __stdcall hooks::set_draw_color::hook(int r, int g, int b, int a) {

	/*if (variables::console_is_drawing && settings::misc::modulate_hud) {
		int newR = r;
		int newG = min(255, g + 25);
		int newB = min(255, b + 80);
		set_draw_color_original(newR, newG, newB, a);
		return;
	}*/


	set_draw_color_original(r, g, b, a);
}

void __fastcall hooks::find_or_add_filename::hook(void* self, int edx, char const* file_name)
{
	// my attempt of recreating someones post of old weapon sounds, if you noticed this hook isnt being called within
	// hooks::initialize i must ever so wonder why...
	auto start = strstr(file_name, "weapons/");
	if (start && strstr(file_name, ".wav")) {
		char patched_name[128] = "divinity/";
		strcpy(patched_name + 9, start + 9);
		return find_or_add_filename_original(self, patched_name);
	}

	return find_or_add_filename_original(self, file_name);
}

bool __fastcall hooks::retaddr::hook(void* ecx, void* rcx, const char* m_modulename)
{
	return true;
}

// This renders under the viewmodel but over glow, gay as fuck lmfao
void __fastcall hooks::render_smoke_overlay::hook(void* thisptr, void* edx, bool pre_viewmodel)
{
	render_smoke_overlay_original(thisptr, edx, pre_viewmodel);
}

void __fastcall hooks::draw_viewmodels::hook(void* thisptr, void* edx, c_view_setup& view, bool draw_view_model, bool draw_scope_lens_mask)
{
	motion_blur::gather_information(view);


	motion_blur::render();

	return draw_viewmodels_original(thisptr, edx, view, draw_view_model, draw_scope_lens_mask);
}