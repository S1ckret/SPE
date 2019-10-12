#include "Physics\Collision.h"
#include "Physics\Body.h"

#include "Shapes\Polygon.h"

struct Edge
{
	Edge() {}
	Edge(const glm::vec2& begin, const glm::vec2& end) : 
		vBegin(begin),
		vEnd(end)
	{
		vec = vEnd - vBegin;
	}

	glm::vec2 vBegin;
	glm::vec2 vEnd;
	glm::vec2 vec;
};


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
	const Poly * A = dynamic_cast<const Poly*>(m->A->GetShape());
	const Poly * B = dynamic_cast<const Poly*>(m->B->GetShape());

	if (A && B)
	{
		unsigned int index_vertex_A = 0;
		float distance_A = FindAxisLeastPenetration(&index_vertex_A, A, B);
		unsigned int index_vertex_B = 0;
		float distance_B = FindAxisLeastPenetration(&index_vertex_B, B, A);

		// Early out
		if (distance_A > 0 || distance_B > 0)
		{
			return false;
		}
		// To Do: Rotate normals
		if (distance_A > distance_B)
		{
			m->penetration = distance_A;
			m->normal = A->GetNormal(index_vertex_A);
		}
		else
		{
			m->penetration = distance_B;
			m->normal = A->GetNormal(index_vertex_B);
		}

		Edge best_edge_A = FindTheMostPerpendicularEdgeToNormal(index_vertex_A, A, m->normal);
		Edge best_edge_B = FindTheMostPerpendicularEdgeToNormal(index_vertex_B, B, -m->normal);
		
		// The reference edge is the edge most perpendicular to the separation normal.
		// The reference edge will be used to clip the incident edge vertices to generate the contact manifold.
		Edge reference;
		Edge incident;
		bool flip = false;

		if (abs(glm::dot(best_edge_A.vec, m->normal)) <= abs(glm::dot(best_edge_B.vec, m->normal)))
		{
			reference = best_edge_A;
			incident = best_edge_B;
		}
		else
		{
			reference = best_edge_B;
			incident = best_edge_A;
			flip = true;
		}


		
	}

	return false;
}

float CheckCollision::FindAxisLeastPenetration(unsigned int * faceIndex, const Poly * A, const Poly * B)
{
	float best_distance = -1000000.f;
	unsigned int best_index;

	for (unsigned int i = 0; i < A->GetVerticiesCount(); i++)
	{
		glm::vec2 normal = A->GetNormal(i);
		glm::vec2 support = B->GetSupport(-normal);

		// Transform A vertex position to B's local space. only translation.
		glm::mat4 rot_mat = A->GetRotationMat();
		glm::vec4 current_pos_A = glm::vec4(A->GetVerticies()[i].position, 0.f, 0.f);
		glm::vec2 position = rot_mat * current_pos_A;
		position += A->GetTranslationVec();
		position -= B->GetTranslationVec();

		float distance = glm::dot(normal, support - position);

		if (distance > best_distance)
		{
			best_distance = distance;
			best_index = i;
		}
	}

	*faceIndex = best_index;
	return best_distance;
}

Edge CheckCollision::FindTheMostPerpendicularEdgeToNormal(const unsigned int vertexIndex, const Poly * A, const glm::vec2 & normal)
{
	// Check left and right verticies.
	glm::vec2 current_pos = A->GetVertex(vertexIndex).position;
	// right is clockwise.
	unsigned int right = vertexIndex - 1;
	if (!vertexIndex)
	{
		right = A->GetVerticiesCount() - 1;
	}
	// left is counterclockwise.
	unsigned int left = (vertexIndex + 1) % A->GetVerticiesCount();

	glm::vec2 right_pos = A->GetVertex(right).position;
	glm::vec2 left_pos = A->GetVertex(left).position;

	glm::vec2 left_edge = current_pos - left_pos;
	glm::vec2 right_edge = current_pos - right_pos;

	// The more perpendicular the more dot product is closer to zero.
	if (glm::dot(right_edge, normal) <= glm::dot(left_edge, normal))
	{
		// Right edge is the best.
		return Edge(right_pos, current_pos);
	}
	else
	{
		// Left edge is the best.
		return Edge(current_pos, left_pos);
	}
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
