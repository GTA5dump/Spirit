#pragma once
#include "ImGui/imgui.h"

namespace Big
{
	class D3DRenderer
	{
	public:
		explicit D3DRenderer();
		~D3DRenderer() noexcept;
		D3DRenderer(D3DRenderer const&) = delete;
		D3DRenderer(D3DRenderer&&) = delete;
		D3DRenderer& operator=(D3DRenderer const&) = delete;
		D3DRenderer& operator=(D3DRenderer&&) = delete;

		/**
		 * \brief Prepares the frame.
		 */
		void BeginFrame();

		/**
		 * \brief Finishes the frame and renders.
		 */
		void EndFrame();

		/**
		 * \brief Invalidates rendering resources
		 */
		void PreResize();

		/**
		 * \brief Finishes the frame and renders.
		 */
		void PostResize();

		/**
		 * \brief Processes input messages.
		 */
		void WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		ImFont* m_font;
		ImFont* m_iconfont;
		ImFont* m_nasafont;
		ImFont* m_monospace_font;
		ImFont* m_titlefont;
		ImFont* m_Spiritfont;
		ImVec2 m_windowSize;

	private:
		IDXGISwapChain* m_Swapchain{};
		ID3D11Device* m_Device{};
		ID3D11DeviceContext* m_Context{};
	};

	inline std::unique_ptr<D3DRenderer> g_D3DRenderer;
}
