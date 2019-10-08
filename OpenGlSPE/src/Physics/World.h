#pragma once

#include "Shapes\Shape.h"
#include "Shapes\Circle.h"
#include "Shapes\Polygon.h"

#include "Physics\Body.h"
#include "Physics\Collision.h"


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

	void ClearManifols();
private:
	std::list<Body*> m_bodies;
	std::vector<std::pair<Body*, Body*>> m_broad_phase_bodies;
	std::vector<Manifold*> m_manifolds;
};