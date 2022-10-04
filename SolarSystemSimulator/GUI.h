#pragma once
#include <d3d9.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include "Vector3D.h"
#include "celestialBody.h"
#include <vector>
#include <string>

#include "Vector3D.h"
#include "LinkedList.h"
#include "queue.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter);
namespace Gui {
	
	long __stdcall windowProcess(HWND, UINT, WPARAM, LPARAM);

	void createHWindow(const std::string, const std::string);
	void destroyHWindow();

	bool createDevice();
	void resetDevice();
	void destroyDevice();

	void createImGui();
	void destroyImGui();

	void beginRender();
	void endRender();
	void Render(std::vector<celestialBody*> solarSystem);


	bool exit = true;
	int width = 1600, height = 900;
	std::string name = "Soler sym";
	HWND window = nullptr;
	WNDCLASSEXA windowClass = {};
	POINTS position = {};
	//direct x
	PDIRECT3D9 d3d = nullptr;
	LPDIRECT3DDEVICE9 device = nullptr;
	D3DPRESENT_PARAMETERS presentParameters = {};

	double zoomScalar = 20/6.9e8, planetScalar = 10, offsetX = width / 2, offsetY = height / 2, wheel;
	int focusPlanet = 0;
	std::vector<queue<Vector3D>> pathDots(12);
	float secondsPerFrame = 1;

	long __stdcall windowProcess(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter) {

		if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter)) return true;

		switch (message) {
		case WM_SIZE:
			if (device && wideParameter != SIZE_MINIMIZED) {
				presentParameters.BackBufferWidth = LOWORD(longParameter);
				presentParameters.BackBufferHeight = HIWORD(longParameter);
				resetDevice();
			}
			return 0;
		case WM_SYSCOMMAND:
			if ((wideParameter & 0xfff0) == SC_KEYMENU) return 0;
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_LBUTTONDOWN:
			position = MAKEPOINTS(longParameter);
			return 0;
		case WM_MOUSEMOVE:
			if (wideParameter != MK_LBUTTON) return 0;
			const auto points = MAKEPOINTS(longParameter);
			auto rect = RECT{};
			GetWindowRect(window, &rect);
			rect.left += points.x - position.x;
			rect.top += points.y - position.y;
			if (position.x >= 0 && position.x <= width && position.y >= 0 && position.y <= 19) SetWindowPos(window, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
			return 0;
		case WM_MOUSEWHEEL:
			zoomScalar *= (float)(100 + GET_WHEEL_DELTA_WPARAM(wideParameter) / 30)/100;
			if (zoomScalar > 3e1) zoomScalar = 3e1;
			if (zoomScalar < 0.00000000009) zoomScalar = 0.00000000009;
			return 0;
		}

		return DefWindowProcW(window, message, wideParameter, longParameter);
	}
	void createHWindow(const std::string windowName, const std::string className) {
		windowClass.cbSize = sizeof(WNDCLASSEXA);
		windowClass.style = CS_CLASSDC;
		windowClass.lpfnWndProc = windowProcess;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandleA(0);
		windowClass.hIcon = 0;
		windowClass.hCursor = 0;
		windowClass.hbrBackground = 0;
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = className.c_str();
		windowClass.hIconSm = 0;

		RegisterClassExA(&windowClass);

		window = CreateWindowA(className.c_str(), windowName.c_str(), WS_POPUP, 100, 100, width, height, 0, 0, windowClass.hInstance, 0);

		ShowWindow(window, SW_SHOWDEFAULT);
		UpdateWindow(window);
	}
	void destroyHWindow() {
		DestroyWindow(window);
		UnregisterClass((LPWSTR)windowClass.lpszClassName, windowClass.hInstance);
	}
	bool createDevice() {
		d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if (!d3d) return false;
		ZeroMemory(&presentParameters, sizeof(presentParameters));

		presentParameters.Windowed = true;
		presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
		presentParameters.EnableAutoDepthStencil = true;
		presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
		presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

		if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &device) < 0) return false;
		return true;
	}
	void resetDevice() {
		ImGui_ImplDX9_InvalidateDeviceObjects();

		const auto result = device->Reset(&presentParameters);

		if (result == D3DERR_INVALIDCALL) IM_ASSERT(0);

		ImGui_ImplDX9_CreateDeviceObjects();
	}
	void destroyDevice() {
		if (device) {
			device->Release();
			device = nullptr;
		}
		if (d3d) {
			d3d->Release();
			d3d = nullptr;
		}
	}
	void createImGui() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.IniFilename = 0;

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(device);
	}
	void destroyImGui() {
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void beginRender() {
		MSG massage;
		while (PeekMessage(&massage, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&massage);
			DispatchMessage(&massage);
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	void endRender() {
		ImGui::EndFrame();

		device->SetRenderState(D3DRS_ZENABLE, false);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

		if (device->BeginScene() >= 0) {
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			device->EndScene();
		}

		const auto result = device->Present(0, 0, 0, 0);

		if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) resetDevice();
	}
	void Render(std::vector<celestialBody*> solarSystem) {
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize({ (float)width, (float)height });
		if (!ImGui::Begin(name.c_str(), &exit, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
			ImGui::End();
			return;
		};
		if (!ImGui::BeginChild(1, { 500 , (float)height - 200})) {
			ImGui::EndChild();
			return;
		}

		if (ImGui::SliderFloat("Time multiplayer", &secondsPerFrame, 0, 100, "%.2f", ImGuiSliderFlags_Logarithmic));

		if (ImGui::Button("Sun")) {
			focusPlanet = 0;
			zoomScalar = 20 / solarSystem[0]->getRadius();
		}

		if (ImGui::Button("Mercury")) {
			focusPlanet = 1;
			zoomScalar = 20 / solarSystem[1]->getRadius();
		}
		if (ImGui::Button("Venus")) {
			focusPlanet = 2;
			zoomScalar = 20 / solarSystem[2]->getRadius();
		}
		if (ImGui::Button("Earth")) {
			focusPlanet = 3;
			zoomScalar = 20 / solarSystem[3]->getRadius();
		}
		if (ImGui::Button("Moon")) {
			focusPlanet = 4;
			zoomScalar = 20 / solarSystem[4]->getRadius();
		}
		if (ImGui::Button("Mars")) {
			focusPlanet = 5;
			zoomScalar = 20 / solarSystem[5]->getRadius();
		}
		if (ImGui::Button("Jupiter")) {
			focusPlanet = 6;
			zoomScalar = 20 / solarSystem[6]->getRadius();
		}
		if (ImGui::Button("Saturn")) {
			focusPlanet = 7;
			zoomScalar = 20 / solarSystem[7]->getRadius();
		}
		if (ImGui::Button("Titan")) {
			focusPlanet = 8;
			zoomScalar = 20 / solarSystem[8]->getRadius();
		}
		if (ImGui::Button("Uranus")) {
			focusPlanet = 9;
			zoomScalar = 20 / solarSystem[9]->getRadius();
		}
		if (ImGui::Button("Neptune")) {
			focusPlanet = 10;
			zoomScalar = 20 / solarSystem[10]->getRadius();
		}
		if (ImGui::Button("Probe")) {
			focusPlanet = 11;
			zoomScalar = 20 / solarSystem[11]->getRadius();
		}
		ImGui::EndChild();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		float scala = zoomScalar;
		offsetX = width /2 - solarSystem[focusPlanet]->position.x * scala;
		offsetY = height /2 - solarSystem[focusPlanet]->position.y * scala;
		for (int i = 1; i < pathDots.size(); i++) {
			LinkedList<Vector3D>* node = pathDots[i].head;
			while (node != nullptr) {
				float x = node->element.x * scala + offsetX, y = node->element.y * scala + offsetY, r = 2;
				if (x >= 0 - r && x <= width + r && y >= 0 - r && y <= height + r) {
					draw_list->AddCircleFilled(ImVec2(x, y), r, D3DCOLOR_RGBA(solarSystem[i]->getColor()[2], solarSystem[i]->getColor()[1], solarSystem[i]->getColor()[0], solarSystem[i]->getColor()[3]));
				}
				node = node->next;
			}
		}
		for (celestialBody* planet : solarSystem) {
			float x = planet->position.x * scala + offsetX, y = planet->position.y * scala + offsetY, r = planet->getRadius() * planetScalar * scala;
			if (x >= 0 - r && x <= width + r && y >= 0 - r && y <= height + r && r < INT_MAX) {
				draw_list->AddCircleFilled(ImVec2(x, y), r, D3DCOLOR_RGBA(planet->getColor()[2], planet->getColor()[1], planet->getColor()[0], planet->getColor()[3]));
				if(!((planet->name == "Moon" || planet->name == "Titan") && scala < 1e-8))draw_list->AddText(ImVec2(x, y - 7), D3DCOLOR_RGBA(255, 255, 255, 255), planet->name.c_str());
			}
		}
		ImGui::End();
	}

}