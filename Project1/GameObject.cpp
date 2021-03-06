#include "GameObject.h"
#include <unordered_map>
#include "Renderer.h"
#include <functional>
#include "GameManager.h"

using namespace Framework::Object;

bool CGameObject::AddComponent(EBuilderType componentType,
                               UBuilderData data)
{
	SBuilder builder = { componentType, data };

	// Use map instead of switch case
	std::unordered_map<EBuilderType, std::function<bool()>> callback = {
		{
			EBuilderType::RENDERER,
			[&]()
			{
				if (!m_rendererComponent)
				{
					m_rendererComponent = reinterpret_cast<Component::CRenderer*>(CComponent::Instantiate(builder));
					return true;
				}
				else
					return false;
			}
		},
		{
			Object::EBuilderType::TRANSFORM,
			[&]()
			{
				if (!m_tranformComponent)
				{
					m_tranformComponent = reinterpret_cast<Component::CTransform*>(CComponent::Instantiate(builder));
					return true;
				}
				else
					return false;
			}
		}
	};

	// Invoke the command corresponding to the builder
	return callback[componentType]();
}

bool CGameObject::RemoveComponent(EBuilderType componentType)
{
	// Use map instead of switch case
	std::unordered_map<EBuilderType, std::function<bool()>> callback = {
		{
			EBuilderType::RENDERER,
			[&]()
			{
				if (m_rendererComponent)
				{
					CComponent::Release(reinterpret_cast<CComponent*&>(m_rendererComponent));
					return true;
				}
				else
					return false;
			}
		}
	};

	// Invoke the command corresponding to the builder
	return callback[componentType]();
}

bool CGameObject::Init()
{
	m_rendererComponent = nullptr;
	return true;
}

void CGameObject::Destroy()
{
	if (m_rendererComponent)
		Component::CRenderer::Release(m_rendererComponent);
}

CGameObject* CGameObject::Instantiate(SBuilder builder)
{
	CGameObject* instance = nullptr;
	SAFE_ALLOC(instance, CGameObject);

	GameManager::IGameManager::AddGameObject(instance);

	if (!instance->Init())
		SAFE_DELETE(instance);

	return instance;
}

void CGameObject::Release(CGameObject*& instance)
{
	instance->Destroy();
	SAFE_DELETE(instance);
}
void CGameObject::Update()
{
	if (m_rendererComponent)
	{
		m_rendererComponent->Update(m_tranformComponent->position);
	}

	if (m_tranformComponent)
	{
		m_tranformComponent->Update();
	}
}

void CGameObject::Render()
{
	m_rendererComponent->Update(m_tranformComponent->position);
}
