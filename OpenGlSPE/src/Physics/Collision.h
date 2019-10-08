#pragma once 

class Body;

const unsigned int SHAPETYPE_COUNT = 2;

struct Manifold {
	Body * A;
	Body * B;

	unsigned int contactCount;

	glm::vec2 contacts[2];
	glm::vec2 normal;
	float penetration;
};

class CheckCollision
{
public:
	bool operator()(Manifold *m);
private:

static bool CircleVsCircle(Manifold * m);
static bool CircleVsPoly(Manifold * m);

static bool PolyVsCircle(Manifold * m);
static bool PolyVsPoly(Manifold * m);

static bool (*const checkCollision[SHAPETYPE_COUNT][SHAPETYPE_COUNT])(Manifold * m);

};


void solveCollision(Manifold * m);
void positionCorrection(Manifold * m);