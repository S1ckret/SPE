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
			m->contacts[1] = m->normal * B->GetRadius();
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
		LOG_CRITICAL("wtf?");

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
}

void positionCorrection(Manifold * m)
{
}
