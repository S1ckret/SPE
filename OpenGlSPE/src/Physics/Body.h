#pragma once
#include "Shapes\Shape.h"
#include "Shapes\Circle.h"
#include "Shapes\Polygon.h"

#include "Physics\Material.h"
#include "Physics\AABB.h"

class Body
{
public:
	Body();
	~Body();

	void SetShape(Shape * shape);

	void ApplyImpulseToCenter(glm::vec2 impulse);
	void ApplyForceToCenter(glm::vec2 force);
	void ApplyTorque(float torque);

	void ApplyImpulse(glm::vec2 impulse, glm::vec2 point);
	void ApplyForce(glm::vec2 force, glm::vec2 point);

	void Update(float dt);

	void Draw(Renderer& renderer);
private:
	void ComputeMass();

private:
	Shape* m_shape;
	AABB* m_aabb;

	glm::vec2 m_velocity;
	glm::vec2 m_acceleration;
	glm::vec2 m_force;
	float m_angular_velocity;
	float m_angular_acceleration;
	float m_torque;

	MassData m_MassData;
	glm::vec2 m_centeroid;
};