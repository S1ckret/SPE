#include "World.h"

World::World()
{
}

World::~World()
{

}

void World::Step(float dt)
{
	CollisionDetection();
	CollisionResolution();
	BodyUpdate(dt);
}

Body * World::CreateBody(Shape * shape)
{
	Body* body = new Body;
	body->SetShape(shape);
	m_bodies.push_back(body);
	return body;
}

void World::DestroyBody(Body * body)
{
	m_bodies.remove(body);
	delete body;
}

void World::Draw(Renderer& renderer)
{
	for (auto& body : m_bodies)
	{
		body->Draw(renderer);
	}
}

void World::CollisionDetection()
{
	for (auto i = m_bodies.begin(); i != m_bodies.end(); ++i)
	{
		for (auto j = std::next(i); j != m_bodies.end(); ++j)
		{
			const AABB * A = (*i)->GetAABB();
			const AABB * B = (*j)->GetAABB();

			if ( A->isOverlaping(B) )
			{
				LOG_WARN("Overlap");
			}
		}
	}
}

void World::CollisionResolution()
{
}

void World::BodyUpdate(float dt)
{
	for (auto& body : m_bodies)
	{
		body->Update(dt);
	}
}

void World::BroadPhase()
{
}

void World::NarrowPhase()
{
}
