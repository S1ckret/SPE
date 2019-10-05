#pragma once

#include "Physics\Body.h"

#include "Shapes\Shape.h"
#include "Shapes\Circle.h"
#include "Shapes\Polygon.h"


struct Manifold
{
	Body* A;
	Body* B;

	glm::vec2 contacts[2];
	glm::vec2 normal;
	float penetration;
};

class World
{
public:
	World();
	~World();

	void Step(float dt);

	Body * CreateBody(Shape * shape);
	void DestroyBody(Body * body);

	void Draw(Renderer& renderer);

private:
	void CollisionDetection();
	void CollisionResolution();
	void BodyUpdate(float dt);
	void BroadPhase();
	void NarrowPhase();
private:
	std::list<Body*> m_bodies;
};