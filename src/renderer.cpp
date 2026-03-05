#include "renderer.hpp"
#include "gui.hpp"

#include "ui/canvas.hpp"
#include "fonts/font_list.hpp"
#include "fonts/icon_list.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace ellohim
{
	void renderer::init_impl()
	{
        std::thread([this] {
            g_gui.init();
            if (this->init_imgui())
            {
                g_gui.dx_init();
                renderer::on_present();
            }
        }).detach();
	}

	void renderer::destroy_impl()
	{
		clear_all();
	}

    bool renderer::register_window_class()
    {
        m_name = "Gottvergessen (DX11)";
        m_window_class = {
            sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, wndproc, 0L, 0L,
            GetModuleHandle(NULL), NULL, NULL, NULL, NULL, m_name, NULL
        };

        m_atom = RegisterClassEx(&m_window_class);

		return m_atom != 0;
    }

	void renderer::on_present()
	{
		while (m_message.message != WM_QUIT)
        {
            if (PeekMessage(&m_message, NULL, 0U, 0U, PM_REMOVE))
            {
                TranslateMessage(&m_message);
                DispatchMessage(&m_message);
                continue;
            }

            if (g_running == false)
                break;

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            for (const auto& cb : g_gui.m_dx_callbacks | std::views::values)
                cb();

            ImGui::EndFrame();

            // Clear background
            const float clearColor[4] = { 0, 0, 0, 0.0f };
            m_context->OMSetRenderTargets(1, &m_render_target_view, NULL);
            m_context->ClearRenderTargetView(m_render_target_view, clearColor);

            // Render ImGui
            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            // Handle multi-viewports
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }

            m_swap_chain->Present(1, 0);
        }
	}

    bool renderer::create_d3d_device(HWND hwnd)
    {
        DXGI_SWAP_CHAIN_DESC sd{};
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // harus support alpha
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // flip
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        UINT createDeviceFlags = 0;
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_0,
        };

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
            createDeviceFlags, featureLevelArray, 2,
            D3D11_SDK_VERSION, &sd, &m_swap_chain,
            &m_device, &featureLevel, &m_context);

        if (FAILED(hr))
            return false;

        create_render_target();
        return true;
    }

    void renderer::create_render_target()
    {
        ID3D11Texture2D* pBackBuffer = nullptr;
        m_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        m_device->CreateRenderTargetView(pBackBuffer, NULL, &m_render_target_view);
        pBackBuffer->Release();
    }

    void renderer::cleanup_render_target()
    {
        if (m_render_target_view)
        {
            m_render_target_view->Release();
            m_render_target_view = nullptr;
        }
    }

    void renderer::clear_d3d()
    {
        cleanup_render_target();

        if (m_swap_chain)
        {
            m_swap_chain->Release();
            m_swap_chain = nullptr;
        }

        if (m_context)
        {
            m_context->Release();
            m_context = nullptr;
        }

        if (m_device)
        {
            m_device->Release();
            m_device = nullptr;
        }
    }

    void renderer::clear_all()
    {
        clear_d3d();
        UnregisterClass(m_window_class.lpszClassName, m_window_class.hInstance);
    }

    void renderer::reset_device()
    {
        cleanup_render_target();
        m_swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
        create_render_target();
    }

    bool renderer::init_imgui()
    {
		if (!register_window_class())
        {
            clear_all();
            return false;
        }

		m_screen_res = ScreenResolution(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));

        g_pointers->m_hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_NOACTIVATE, m_window_class.lpszClassName, m_name, WS_POPUP,
            0, 0, m_screen_res.x, m_screen_res.y, NULL, NULL, m_window_class.hInstance, NULL);

        SetWindowLong(g_pointers->m_hwnd, GWL_EXSTYLE, GetWindowLong(g_pointers->m_hwnd, GWL_EXSTYLE)
            | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST);
        SetLayeredWindowAttributes(g_pointers->m_hwnd, RGB(0, 0, 0), BYTE(255), LWA_ALPHA); // alpha 0

        {
            RECT client_area{};
            GetClientRect(g_pointers->m_hwnd, &client_area);

            RECT window_area{};
            GetWindowRect(g_pointers->m_hwnd, &window_area);

            POINT diff{};
            ClientToScreen(g_pointers->m_hwnd, &diff);

            const MARGINS margins = {
                window_area.left + (diff.x - window_area.left),
                window_area.top + (diff.y - window_area.top),
                client_area.right,
                client_area.bottom
            };

            DwmExtendFrameIntoClientArea(g_pointers->m_hwnd, &margins);
        }

        if (!create_d3d_device(g_pointers->m_hwnd))
        {
            clear_all();
            return false;
        }

        ShowWindow(g_pointers->m_hwnd, SW_SHOWDEFAULT);
        UpdateWindow(g_pointers->m_hwnd);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui_ImplWin32_Init(g_pointers->m_hwnd);
        ImGui_ImplWin32_EnableAlphaCompositing(g_pointers->m_hwnd);
        ImGui_ImplDX11_Init(m_device, m_context);
        ZeroMemory(&m_message, sizeof(m_message));

        ImFontConfig font_cfg{};
        font_cfg.FontDataOwnedByAtlas = false;
        std::strcpy(font_cfg.Name, "Rubik");

        m_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_rubik), sizeof(font_rubik), 17.f, &font_cfg);
        merge_icon_with_latest_font(14.f, false);

        m_monospace_font = ImGui::GetIO().Fonts->AddFontDefault();

        load_texture(m_device);

        return true;
    }

    LRESULT renderer::wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        for (const auto& cb : g_gui.m_wndproc_callbacks)
            cb(hWnd, msg, wParam, lParam);

        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        switch (msg)
        {
        case WM_SIZE:
            if (get().m_device != NULL && wParam != SIZE_MINIMIZED)
            {
                get().reset_device();
            }
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU)
                return 0;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    void renderer::merge_icon_with_latest_font(float font_size, bool FontDataOwnedByAtlas)
    {
        static const ImWchar icons_ranges[3] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.FontDataOwnedByAtlas = FontDataOwnedByAtlas;

        g_settings.window.font_icon = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)font_icons, sizeof(font_icons), font_size, &icons_config, icons_ranges);
    }

    void renderer::load_texture(ID3D11Device* device)
    {
        for (auto& callback : g_gui.m_texture_callbacks)
        {
            callback(device);
        }
    }
}
