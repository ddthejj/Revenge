#pragma once

class CollisionComponent;
class StaticCollisionComponent;
class DynamicCollisionComponent;
class MyRectangle;
template<typename T> struct Point;
class Sprite;

#define SECTOR_SIZE 4

class PhysicsManager
{
public:

	struct Sector
	{
		MyRectangle* rectangle;
		std::vector<CollisionComponent*> collisionComponents;

		Sector(MyRectangle _rectangle);
		~Sector();
	};

	static std::vector<StaticCollisionComponent*> staticCollisionComponents;
	static std::vector<DynamicCollisionComponent*> dynamicCollisionComponents;
	static std::vector<Sector*> sectors;

	static void Init();
	static void Clean();

	static void RegisterCollision(CollisionComponent* collisionComp);
	static void UnregisterCollision(CollisionComponent* collisionComp);

	static void CreateSectors();

	static bool TryMove(CollisionComponent* collisionComp, Point<float> velocity);
	
private:

	static std::vector<CollisionComponent*> const TestCollision(CollisionComponent* collisionComp, MyRectangle* futureRectangle);

	static std::vector<Sector*> const GetSectors(MyRectangle* collisionRectangle);
};

