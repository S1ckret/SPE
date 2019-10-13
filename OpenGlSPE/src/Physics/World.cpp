#include "World.h"

World::World()
{
}

World::~World()
{
	for (auto i = m_bodies.begin(); i != m_bodies.end(); ++i)
	{
		delete (*i);
	}
	m_bodies.clear();
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
	BroadPhase();
	NarrowPhase();
}

void World::CollisionResolution()
{
	for (auto i : m_manifolds)
	{
		solveCollision(i);
	}
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
	m_broad_phase_bodies.clear();
	for (auto i = m_bodies.begin(); i != m_bodies.end(); ++i)
	{
		for (auto j = std::next(i); j != m_bodies.end(); ++j)
		{
			const AABB * A = (*i)->GetAABB();
			const AABB * B = (*j)->GetAABB();

			if ( A->isOverlaping(B) )
			{
				m_broad_phase_bodies.push_back( std::make_pair((*i), (*j)) );
			}
		}
	}
}

void World::NarrowPhase()
{
	ClearManifols();
	for (auto & i : m_broad_phase_bodies)
	{
		Manifold * m = new Manifold;
		m->A = i.first;
		m->B = i.second;
		if (CheckCollision()(m))
		{
			m_manifolds.push_back(m);
		}
	}
}

void World::ClearManifols()
{
	m_manifolds.clear();
}
