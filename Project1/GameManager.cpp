#include "GameManager.h"
#include "Macros.h"
#include "Window.h"
#include "Direct3DCore.h"

using namespace Framework::GameManager;

// Internal Game Manager Class
class CGameManager_Internal final : public IGameManager
{
private:
	static CGameManager_Internal* __instance;

	// Properties
private:
	Framework::Base::IWindow* m_pWindow;
	Framework::Base::IDirect3DCore* m_pDirect3DCore;

	// Cons/Des
public:
	CGameManager_Internal()
	{
		m_pDirect3DCore = nullptr;
		m_pWindow = nullptr;
	}

	~CGameManager_Internal()
	{
		Framework::Base::IDirect3DCore::Release();
	}

	// Getters / Setters
public:

	// Override methods
public:
	bool Init(HINSTANCE hInstance, int nShowCmd, int screenWidth, int screenHeight, bool fullscreen)
	{
		bool result = false;
		do
		{
			// Init Window
			m_pWindow = Framework::Base::IWindow::Instantiate(hInstance, nShowCmd, screenWidth, screenHeight, fullscreen);
			if (!m_pWindow)
			{
				OutputDebugStringA("[Error] IWindow::Instantiate failed\n");
				break;
			}
			HWND hWnd = m_pWindow->Get_WindowHandle();

			// Init Direct3DCore
			m_pDirect3DCore = Framework::Base::IDirect3DCore::Instantiate(hWnd, fullscreen);
			if (!m_pDirect3DCore)
			{
				OutputDebugStringA("[Error] IDirect3DCore::Instantiate failed\n");
				break;
			}
	
			result = true;
		} while (false);

		return true;
	}

	static CGameManager_Internal* Instantiate(HINSTANCE hInstance, int nShowCmd, int screenWidth, int screenHeight, bool fullscreen);
	static void Destroy();

	bool Run() override
	{
		DWORD frameStart = GetTickCount();
		DWORD tickPerFrame = 1000 / FRAME_RATE;
		
		MSG message = {};
		bool done = false;
		while (!done)
		{
			MSG message = {};
			if (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE))
			{
				// check for escape key (to exit program)
				if (message.message == WM_QUIT)
					done = true;

				// translate message and send back to WinProc
				TranslateMessage(&message);
				DispatchMessageA(&message);
			}

			// dt: the time between (beginning of last frame) and now
			// this frame: the frame we are about to render
			DWORD now = GetTickCount();
			DWORD dt = now - frameStart;

			if (dt >= tickPerFrame)
			{
				frameStart = now;

				// process game loop
				bool renderResult = m_pDirect3DCore->Render();
				if (!renderResult)
				{
					OutputDebugStringA("[Error] m_pDirect3DCore::Render failed\n");
					break;
				}
			}
			else
				Sleep(tickPerFrame - dt);
		}

		return true;
	}

	// Internal methods
private:
};

CGameManager_Internal* CGameManager_Internal::__instance = nullptr;

CGameManager_Internal * CGameManager_Internal::Instantiate(HINSTANCE hInstance, int nShowCmd, int screenWidth, int screenHeight, bool fullscreen)
{
	if (!__instance)
	{
		SAFE_ALLOC(__instance, CGameManager_Internal);
	
		if (!__instance->Init(hInstance, nShowCmd, screenWidth, screenHeight, fullscreen))
			SAFE_DELETE(__instance);
	}

	return __instance;
}

void CGameManager_Internal::Destroy()
{
	SAFE_DELETE(__instance);
}

IGameManager* IGameManager::Instantiate(HINSTANCE hInstance, int nShowCmd, int screenWidth, int screenHeight, bool fullscreen)
{
	return CGameManager_Internal::Instantiate(hInstance, nShowCmd, screenWidth, screenHeight, fullscreen);
}

void Framework::GameManager::IGameManager::Destroy()
{
	CGameManager_Internal::Destroy();
}