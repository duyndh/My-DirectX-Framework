﻿#include "Rigidbody.h"
#include "GameObject.h"

CRigidbody::CRigidbody(Framework::Object::CGameObject * gameObject)
{
	this->_gameObject = gameObject;
	this->_velocity = Vector2(0, 0);
	this->_mass = 0;
	this->_gravityScale = 1;
	this->_isKinematic = false;
}

void CRigidbody::Update(DWORD dt)
{
	auto tranform = _gameObject->GetTranform();
	if(tranform)
		tranform->m_position += _velocity * dt / 10;
}

void CRigidbody::Render()
{
}