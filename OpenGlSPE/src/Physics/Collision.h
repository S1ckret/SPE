#pragma once 

class Body;
class Poly;

struct Edge;
struct ClippedPoints;

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

static float FindAxisLeastPenetration(unsigned int *faceIndex, const Poly *A, const Poly *B);
static Edge FindTheMostPerpendicularEdgeToNormal(const Poly * A, const glm::vec2& normal);
static const std::vector<glm::vec2> Clip(glm::vec2 pos1, glm::vec2 pos2, glm::vec2 normal, float dot);
};


void solveCollision(Manifold * m);
void positionCorrection(Manifold * m);