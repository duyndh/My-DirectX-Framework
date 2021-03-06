#include "Renderer.h"
#include "Macros.h"
#include "Direct3DCore.h"

using namespace Framework::Component;

bool CRenderer::Init(LPCSTR texturePath)
{
	m_texture = Base::IDirect3DCore::GetInstance()->CreateTexture(texturePath);
	return m_texture == nullptr ? false : true;
}

void CRenderer::Destroy()
{
	if (m_texture)
		m_texture->Release();
}

CRenderer* CRenderer::Instantiate(LPCSTR texturePath)
{
	CRenderer* instance = nullptr;
	SAFE_ALLOC(instance, CRenderer);

	if (!instance->Init(texturePath))
		SAFE_DELETE(instance);

	return instance;
}

void CRenderer::Release(CRenderer* &instance)
{
	instance->Destroy();
	SAFE_DELETE(instance);
}

void CRenderer::Update(Vector3 position)
{
	Base::IDirect3DCore::GetInstance()->Draw(position.x, position.y, m_texture);
}
