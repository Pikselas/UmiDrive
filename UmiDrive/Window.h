#pragma once
#include<Windows.h>
#include<sstream>
#include<bitset>
#include<functional>
#include<atomic>
#include<source_location>

class Window
{
	public:
		class Mouse
		{
		friend Window;
		private:
			int x = -1;
			int y = -1;
			int wheelDelta = 0;
		private:
			bool LeftPressed = false;
			bool RightPressed = false;
		public:
			bool IsLeftPressed() const;
			bool IsRightPressed() const;
			int  GetWheelDelta() const;
			int GetX() const;
			int GetY() const;
			std::pair<int, int> GetXY() const;
			void Reset();
		public:
			using EventHandlerType = std::function<void(Window&)>;
			EventHandlerType OnMove				= nullptr;
			EventHandlerType OnWheel			= nullptr;
			EventHandlerType OnLeftPress		= nullptr;
			EventHandlerType OnRightPress		= nullptr;
			EventHandlerType OnLeftRelease		= nullptr;
			EventHandlerType OnRightRelease		= nullptr;
			EventHandlerType OnLeftDoubleClick	= nullptr;
			EventHandlerType OnRightDoubleClick = nullptr;
		};
		class KeyBoard
		{
			friend Window;
		private:
			std::bitset<256> KEY_STAT;
			bool REPEAT_ENABLED = false;
		public:
			bool IsKeyDown(unsigned char keycode) const;
			bool IsRepeatEnabled() const;
			void EnableKeyRepeat();
			void DisableKeyRepeat();
			void Reset();
		public:
			struct EventT
			{
				Window& window;
				unsigned char KEY_CODE;
				bool IS_REPEATED;
				EventT(Window& wnd, unsigned char code, bool repeat);
			};
		public:
			using EventHandlerType = std::function<void(EventT)>;
			EventHandlerType OnKeyPress		= nullptr;
			EventHandlerType OnKeyRelease	= nullptr;
			EventHandlerType OnCharInput	= nullptr;
		};
		class Exception
		{
		private:
			const HRESULT code;
			std::string message;
			std::string file;
			unsigned int line;
		public:
			Exception(HRESULT hr , std::source_location s = std::source_location::current());
			std::string GetReason() const;
			unsigned int GetLine() const;
			std::string GetFile() const;
			HRESULT GetErrorCode() const;
		};
	private:
		unsigned int height;
		unsigned int width;
		bool Closed = false;
		static std::atomic_uint WindowCount;
	public:
		HWND window_handle;
	private:
		static LRESULT StaticEventHandler(HWND handle , UINT msgcode , WPARAM wparam ,LPARAM lparam);
	protected:
		virtual LRESULT EventHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam);
	public:
		Mouse mouse;
		KeyBoard keyboard;
	public:
		Window(Window * Parent, DWORD exStyle, const std::string& window_class ,const std::string& title, DWORD Style, int x, int y, int width, int height);
		~Window();
		unsigned int GetHeight() const;
		unsigned int GetWidth() const;
		bool IsOpen() const;
		void Redraw() const;
		void Update() const;
		void ChangeTitle(const std::string& title);
		void SetFocus();
	public:
		using EventDispatcher = void (&)(const HWND);
		void ProcessEvents(EventDispatcher e = DispatchWindowEvents) const;
	public:
		constexpr static Window* ALL_WINDOWS = nullptr;
	public:
		static void MainLoop(const Window* const window, EventDispatcher e = DispatchWindowEvents);
		static void DispatchWindowEvents(const HWND handle = nullptr);
		static void DispatchWindowEventsNonBlocking(const HWND handle = nullptr);
		static unsigned int GetWindowCount();
};

#define THROW_IF_FAILED(hrcall) if(HRESULT hrcode = hrcall ; FAILED(hrcode)) throw Window::Exception(hrcode)