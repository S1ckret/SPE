#include "Body.h"
Body::Body() :
	m_velocity({0.f, 0.f}),
	m_acceleration({0.f, 0.f}),
	m_force({0.f, 0.f}),
	m_angular_velocity(0.f),
	m_angular_acceleration(0.f),
	m_torque(0.f)
{
}

Body::~Body()
{
}

void Body::SetShape(Shape * shape)
{
	m_shape = shape->Clone();
	LOG_INFO(typeid(m_shape).name());
}

void Body::ApplyImpulseToCenter(glm::vec2 impulse)
{
	m_velocity += impulse * m_MassData.inv_mass;
}

void Body::ApplyForceToCenter(glm::vec2 force)
{
	m_force += force;
}

void Body::ApplyTorque(float torque)
{
	m_torque += torque;
}

void Body::ApplyImpulse(glm::vec2 impulse, glm::vec2 point)
{
	m_velocity += impulse * m_MassData.inv_mass;
	glm::vec3 crossVec = glm::cross(glm::vec3(point, 0.f) - glm::vec3(m_shape->GetPosition(), 0.f), glm::vec3(impulse, 0.f));
	m_angular_velocity += crossVec.z * m_MassData.inv_I;
}

void Body::ApplyForce(glm::vec2 force, glm::vec2 point)
{
	m_force += force;
	glm::vec3 crossVec = glm::cross(glm::vec3(point, 0.f) - glm::vec3(m_shape->GetPosition(), 0.f), glm::vec3(force, 0.f));
	m_torque += crossVec.z * m_MassData.inv_I;
}

void Body::Update(float dt)
{
	m_acceleration = m_force * m_MassData.inv_mass;
	m_angular_acceleration = m_torque * m_MassData.inv_I;
	m_force = glm::vec2(0.f, 0.f);
	m_torque = 0.f;

	m_velocity += m_acceleration;
	m_angular_velocity += m_angular_acceleration;

	m_shape->Translate(m_velocity);
	m_shape->Rotate(m_angular_velocity);
}

void Body::Draw(Renderer& renderer)
{
	m_shape->Draw(renderer);
}