#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <Windows.h>
#include <vector>

// Window event callback function
LRESULT WINAPI WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

typedef D3DXVECTOR3 Vector3;
typedef LPDIRECT3DTEXTURE9 Texture;
