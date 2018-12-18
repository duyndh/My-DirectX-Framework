#include "HeadGunnerEnemyController.h"
#include "Rigidbody.h"
#include "CameraController.h"
#include "Graphic.h"
#include "Input.h"
#include "Animator.h"
#include <math.h>
#include "Header.h"

void HeadGunnerEnemyController::OnCollisionEnter(CCollision * collision)
{
}

void HeadGunnerEnemyController::Update(DWORD dt)
{
	CRigidbody *rigidbody = m_pGameObject->GetComponent<CRigidbody>();
	CTransform *transform = m_pGameObject->GetComponent<CTransform>();
	CRenderer *renderer = m_pGameObject->GetComponent<CRenderer>();
	CAnimator *anim = m_pGameObject->GetComponent<CAnimator>();
	const Vector2 velocity = rigidbody->GetVelocity();

	if (m_target)
	{
		Vector2 targetPosition = m_target->GetComponent<CTransform>()->Get_Position();
		Vector2 currentPosition = m_pGameObject->GetComponent<CTransform>()->Get_Position();

		if (sqrt(pow(targetPosition.x - currentPosition.x, 2)) < 200)
		{
			anim->SetBool("isShoot", true);
			
			m_reloadTime += dt;
			if (m_reloadTime >= RELOAD_TIME)
			{
				Vector2 pos = m_pGameObject->GetComponent<CTransform>()->Get_Position();
				bool isFlip = m_pGameObject->GetComponent<CRenderer>()->GetFlipX();

				pos.x = (isFlip ? pos.x + 10 : pos.x - 10);
				pos.y;
				auto pBullet = CGameObject::Instantiate(Prefab_HeadGunnerBullet, nullptr, pos);

				pBullet->GetComponent<CRigidbody>()->SetVelocity({ (isFlip ? .3f : -.3f) , 0 });

				m_reloadTime = 0;
			}
		}
		else
		{
			anim->SetBool("isShoot", false);
		}

		renderer->SetFlipX(targetPosition.x > currentPosition.x);
	}
}

void HeadGunnerEnemyController::Render()
{
}