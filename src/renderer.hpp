#pragma once
#include "classes/vector.hpp"

namespace ellohim
{
	class renderer
	{
		struct process
		{
			DWORD m_id;
			HANDLE m_handle;
			HWND m_hwnd;
			WNDPROC m_wndproc;
			int m_width;
			int m_height;
			int m_left;
			int m_right;
			int m_top;
			int m_bottom;
			LPCSTR m_title;
			LPCSTR m_class_name;
			LPCSTR m_path;
		} process;

		struct overlay
		{
			WNDCLASSEX m_window_class;
			HWND m_hwnd;
			LPCSTR m_name;
		} overlay;

		static renderer& get()
		{
			static renderer instance;
			return instance;
		}
	public:
        explicit renderer() = default;
        ~renderer() = default;

		static void init() { get().init_impl(); }
		static void destroy() { get().destroy_impl(); }
		static ImFont* get_font() { return get().m_font; }
		static ImFont* get_monospace_font() { return get().m_monospace_font; }
		static ScreenResolution get_screen_res() { return get().m_screen_res; }
	private:
		void on_present();
		bool create_d3d_device(HWND hwnd);
		void create_render_target();
		void cleanup_render_target();
		void clear_d3d();
		void clear_all();
		void reset_device();
		bool init_imgui();
		void init_impl();
		void destroy_impl();
		bool register_window_class();
		static LRESULT wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		void merge_icon_with_latest_font(float font_size, bool FontDataOwnedByAtlas);
		void load_texture(ID3D11Device* device);
		bool m_init_pos = false;
	private:
		ScreenResolution m_screen_res{};
		WNDCLASSEX m_window_class{};
		LPCSTR m_name{};
		MSG m_message{};
		ImFont* m_font{};
		ImFont* m_monospace_font{};
		ATOM m_atom{};
	public:
		IDXGISwapChain* m_swap_chain = nullptr;
		ID3D11Device* m_device = nullptr;
		ID3D11DeviceContext* m_context = nullptr;
		ID3D11RenderTargetView* m_render_target_view = nullptr;
	};
}