#include "Physics\Collision.h"
#include "Body.h"
bool (*const CheckCollision::checkCollision[SHAPETYPE_COUNT][SHAPETYPE_COUNT])(Manifold * m)
{
	{CircleVsCircle, CircleVsPoly},
	{ PolyVsCircle, PolyVsPoly }
};

bool CheckCollision::operator()(Manifold * m)
{
	return checkCollision[m->A->GetShape()->GetType()][m->B->GetShape()->GetType()](m);
}

bool CheckCollision::CircleVsCircle(Manifold * m)
{
	const Circle * A = dynamic_cast<const Circle*>(m->A->GetShape());
	const Circle * B = dynamic_cast<const Circle*>(m->B->GetShape());

	if (A && B)
	{
		glm::vec2 distance = B->GetTranslationVec() - A->GetTranslationVec();
		float distance_length = distance.x * distance.x + distance.y + distance.y;
		float radius_sum = A->GetRadius() + B->GetRadius();
		// Early out
		if (distance_length > radius_sum * radius_sum)
		{
			m->contactCount = 0;
			return false;
		}
		distance_length = sqrt(distance_length);
		if (distance_length)
		{
			m->normal = distance / distance_length;
			m->penetration = distance_length - radius_sum;
			m->contacts[0] = m->normal * A->GetRadius();
			m->contacts[1] = -m->normal * B->GetRadius();
			m->contactCount = 2;
			return true;
		}
		else
		{
			m->normal = {1.f, 0.f};
			m->penetration = A->GetRadius();
			m->contacts[0] = {0.f, 0.f};
			m->contacts[1] = {0.f, 0.f};
			m->contactCount = 1;
			return true;
		}

	}
	return false;
}

bool CheckCollision::CircleVsPoly(Manifold * m)
{
	return false;
}

bool CheckCollision::PolyVsCircle(Manifold * m)
{
	return false;
}

bool CheckCollision::PolyVsPoly(Manifold * m)
{
	return false;
}

void solveCollision(Manifold * m)
{

	Body * A = m->A;
	Body * B = m->B;
	glm::vec2 relative_velocity = B->GetVelocity() - A->GetVelocity();
	float velocity_along_normal = glm::dot(relative_velocity, m->normal);

	if (velocity_along_normal > 0.f) {
		return;
	}

	float restitution = std::min(A->GetShape()->GetMaterial().restitution, B->GetShape()->GetMaterial().restitution);

	float impulse_scalar = 0.f;
	float inv_mass = A->GetMassData().inv_mass + B->GetMassData().inv_mass;
	float first = std::pow(glm::cross(glm::vec3(m->contacts[0], 0.f), glm::vec3(m->normal, 0.f)).z, 2) * A->GetMassData().inv_I;
	float second = std::pow(glm::cross(glm::vec3(m->contacts[1], 0.f), glm::vec3(m->normal, 0.f)).z, 2) * B->GetMassData().inv_I;

	impulse_scalar += -(1 + restitution) * velocity_along_normal;
	impulse_scalar /= inv_mass + first + second;

	glm::vec2 impulse = impulse_scalar * m->normal;
	A->ApplyImpulse(-impulse, m->contacts[0]);
	B->ApplyImpulse(impulse, m->contacts[1]);
}

void positionCorrection(Manifold * m)
{
}
