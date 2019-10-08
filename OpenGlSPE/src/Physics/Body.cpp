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
	delete m_shape;
	delete m_aabb;
	LOG_CRITICAL("---Body");
}

void Body::SetShape(Shape * shape)
{
	LOG_INFO("=== Body");
	m_shape = shape->Clone();
	m_aabb = new AABB;
	m_aabb->SetShape(m_shape);
	m_aabb->GenerateVerticies(m_shape->GetVerticies(), m_shape->GetRotationMat(), m_shape->GetVerticiesCount());
	ComputeMass();
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
	m_aabb->GenerateVerticies(m_shape->GetVerticies(), m_shape->GetRotationMat(), m_shape->GetVerticiesCount());
}

const Shape * Body::GetShape() const
{
	return m_shape;
}

const AABB * Body::GetAABB() const
{
	return m_aabb;
}

void Body::Draw(Renderer& renderer)
{
	m_shape->Draw(renderer);
	m_aabb->Draw(renderer, m_shape->GetTranslationMat());
}

// Source_0 >> http://richardson.eng.ua.edu/Former_Courses/CE_331_fa09/Projects/A_and_I_of_Polygon.pdf
// Source_1 >> https://github.com/RandyGaul/ImpulseEngine/blob/master/Shape.h
void Body::ComputeMass()
{
	float area = 0.0f;
	float I = 0.0f;
	constexpr float k_inv3 = 1.0f / 3.0f;

	unsigned int i = 0;
	const Vertex * verticies = m_shape->GetVerticies();
	do
	{
		unsigned int next = (i + 1) % m_shape->GetVerticiesCount();
		
		glm::vec2 A = verticies[i].position;
		glm::vec2 B = verticies[next].position;

		float crossProd = glm::cross(glm::vec3(A, 0.f), glm::vec3(B, 0.f)).z;
		float triangleArea = 0.5f * crossProd;
		area += triangleArea;

		m_centeroid += triangleArea * k_inv3 * (A + B);

		float Ix = A.x * A.x + A.x * B.x + B.x + B.x;
		float Iy = A.y * A.y + A.y * B.y + B.y + B.y;
		I += (0.25f * k_inv3 * crossProd) * (Ix + Iy);
		
		i = next;
	} while (i);

	m_centeroid *= 1.0f / area;

	m_MassData.mass = m_shape->GetMaterial().density * area;
	m_MassData.inv_mass = (m_MassData.mass) ? 1.0f / m_MassData.mass : 0.0f;
	m_MassData.I = I * m_shape->GetMaterial().density;
	m_MassData.inv_I = m_MassData.I ? 1.0f / m_MassData.I : 0.0f;
	LOG_INFO("\t Mass: {0}\t Inv_Mass: {1}\t I: {2}\tInv_I: {3}", m_MassData.mass, m_MassData.inv_mass, m_MassData.I, m_MassData.inv_I);
}
