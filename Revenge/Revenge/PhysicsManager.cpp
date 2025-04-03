#include "defines.h"
#include "PhysicsManager.h"
#include "CollisionComponent.h"
#include "Rectangle.h"
#include "OverworldManager.h"
#include "Room.h"
#include "ComponentManager.h"
#include "TriggerComponent.h"

#include <cmath>

std::vector<StaticCollisionComponent*> PhysicsManager::staticCollisionComponents = std::vector<StaticCollisionComponent*>();
std::vector<DynamicCollisionComponent*> PhysicsManager::dynamicCollisionComponents = std::vector<DynamicCollisionComponent*>();
std::vector<PhysicsManager::Sector*> PhysicsManager::sectors = std::vector<Sector*>();

void PhysicsManager::Init()
{
}

void PhysicsManager::Clean()
{
	staticCollisionComponents.clear();
	dynamicCollisionComponents.clear();

	for (auto it = sectors.begin(); it != sectors.end(); it++)
	{
		SafeDelete(*it);
	}
	sectors.clear();
}

template <typename T>
void RegisterCollisionToVector(T* collisionComp, std::vector<T*> &vector)
{
	if (std::find(vector.begin(), vector.end(), collisionComp) == vector.end())
	{
		vector.push_back(collisionComp);
	}
}

template <typename T>
void UnregisterCollisionToVector(T* collisionComp, std::vector<T*> &vector)
{
	auto it = std::find(vector.begin(), vector.end(), collisionComp);
	if (it != vector.end())
	{
		vector.erase(it);
	}
}

void PhysicsManager::RegisterCollision(CollisionComponent* collisionComp)
{
	switch (collisionComp->GetType())
	{
	case COMPONENT_TYPE::STATIC_COLLISION:
		RegisterCollisionToVector<StaticCollisionComponent>((StaticCollisionComponent*)collisionComp, staticCollisionComponents);
		break;
	case COMPONENT_TYPE::DYNAMIC_COLLISION:
		RegisterCollisionToVector<DynamicCollisionComponent>((DynamicCollisionComponent*)collisionComp, dynamicCollisionComponents);
		break;
	}
}

void PhysicsManager::UnregisterCollision(CollisionComponent* collisionComp)
{
	switch (collisionComp->GetType())
	{
	case COMPONENT_TYPE::STATIC_COLLISION:
		UnregisterCollisionToVector<StaticCollisionComponent>((StaticCollisionComponent*)collisionComp, staticCollisionComponents);
		break;
	case COMPONENT_TYPE::DYNAMIC_COLLISION:
		UnregisterCollisionToVector<DynamicCollisionComponent>((DynamicCollisionComponent*)collisionComp, dynamicCollisionComponents);
		break;
	}
}

bool PhysicsManager::TryMove(CollisionComponent* collisionComp, Point<float> velocity)
{
	bool collided = false;

	// horizontal movement first
	MyRectangle futureRectangle(*collisionComp->collider);
	futureRectangle.SetLocation(Point<float>(futureRectangle.Location().x + velocity.x, futureRectangle.Location().y));
	std::vector<CollisionComponent*> collidedComponents = TestCollision(collisionComp, &futureRectangle);
	collided = collidedComponents.size();

	for (int i = 0; i < collidedComponents.size(); i++)
	{
		collidedComponents[i]->Collide(collisionComp);
		MyRectangle* collidedRectangle = collidedComponents[i]->collider;

		if (velocity.x > 0) // moving right
		{
			if (futureRectangle.Right() - collidedRectangle->Left() <= velocity.x)
			{
				futureRectangle.SetX(collidedRectangle->Left() - 1 - futureRectangle.Width());
			}
		}
		else if (velocity.x < 0) // moving left
		{
			if (futureRectangle.Left() - collidedRectangle->Right() >= velocity.x)
			{
				futureRectangle.SetX(collidedRectangle->Right() + 1);
			}
		}
	}

	collisionComp->collider->SetLocation(futureRectangle.Location());

	// vertical movement second
	futureRectangle.SetLocation(Point<float>(futureRectangle.Location().x, futureRectangle.Location().y + velocity.y));
	collidedComponents = TestCollision(collisionComp, &futureRectangle);
	collided = collided || collidedComponents.size();

	for (int i = 0; i < collidedComponents.size(); i++)
	{
		collidedComponents[i]->Collide(collisionComp);
		MyRectangle* collidedRectangle = collidedComponents[i]->collider;

		if (velocity.y < 0) // moving up
		{
			if (futureRectangle.Top() - collidedRectangle->Bottom() >= velocity.y)
			{
				futureRectangle.SetY(collidedRectangle->Bottom() + 1);
			}
		}
		else if (velocity.y > 0) // moving down
		{
			if (futureRectangle.Bottom() - collidedRectangle->Top() <= velocity.y)
			{
				futureRectangle.SetY(collidedRectangle->Top() - 1 - futureRectangle.Height());
			}
		}
	}

	collisionComp->collider->SetLocation(futureRectangle.Location());

	std::vector<Component*> triggerComponents = ComponentManager::GetComponentsByType(COMPONENT_TYPE::TRIGGER);
	
	for (auto it = triggerComponents.begin(); it != triggerComponents.end(); it++)
	{
		TriggerComponent* component = (TriggerComponent*)(*it);
		
		if (component->GetRectangle()->Intersects(*collisionComp->collider))
		{
			component->Trigger(collisionComp);
		}
	}

	return collided;
}

std::vector<CollisionComponent*> const PhysicsManager::TestCollision(CollisionComponent* collisionComp, MyRectangle* futureRectangle)
{
	// static collision
	std::vector<Sector*> collidedSectors = GetSectors(futureRectangle);
	int sectorTileNum = SECTOR_SIZE * SECTOR_SIZE;
	std::vector<CollisionComponent*> collidedComponents;

	for (int i = 0; i < collidedSectors.size(); i++)
	{
		for (int j = 0; j < collidedSectors[i]->collisionComponents.size(); j++)
		{
			CollisionComponent* comp = collidedSectors[i]->collisionComponents[j];

			if (comp->collider->Intersects(*futureRectangle))
			{
				collidedComponents.push_back(collidedSectors[i]->collisionComponents[j]);
			}
		}
	}

	// dynamic collision
	for (int i = 0; i < dynamicCollisionComponents.size(); i++)
	{
		if (dynamicCollisionComponents[i]->collider->Intersects(*futureRectangle))
		{
			if (dynamicCollisionComponents[i] != collisionComp)
			{
				collidedComponents.push_back(dynamicCollisionComponents[i]);
			}
		}
	}

	return collidedComponents;
}

std::vector<PhysicsManager::Sector*> const PhysicsManager::GetSectors(MyRectangle* collisionRectangle)
{
	std::vector<Sector*> outSectors;

	for (int i = 0; i < sectors.size(); i++)
	{
		if (outSectors.size() == 0 && sectors[i]->rectangle->Contains(*collisionRectangle))
		{
			outSectors.push_back(sectors[i]);
			return outSectors;
		}
		if (sectors[i]->rectangle->Intersects(*collisionRectangle))
		{
			outSectors.push_back(sectors[i]);
		}
	}

	return outSectors;
}

void PhysicsManager::CreateSectors()
{
	sectors.clear();

	int sectorDimsX = (int)std::ceil(OverworldManager::GetCurrentRoom()->GetDimensions().x / SECTOR_SIZE);
	int sectorDimsY = (int)std::ceil(OverworldManager::GetCurrentRoom()->GetDimensions().y / SECTOR_SIZE);
	int sectorNum = sectorDimsX * sectorDimsY;

	for (auto it = staticCollisionComponents.begin(); it != staticCollisionComponents.end(); it++)
	{
		MyRectangle* rectangle = (*it)->collider;

		int sectorAt = (int)(std::floor(rectangle->X() / (SECTOR_SIZE * TILE_WIDTH)) + (std::floor(rectangle->Y() / (SECTOR_SIZE * TILE_HEIGHT)) * sectorDimsX));

		while (sectors.size() <= sectorAt)
		{
			sectors.push_back(new Sector(MyRectangle((sectors.size()) % sectorDimsX * SECTOR_SIZE * TILE_WIDTH, (int)(std::floor((sectors.size()) / sectorDimsY)) * SECTOR_SIZE * TILE_HEIGHT, SECTOR_SIZE * TILE_WIDTH, SECTOR_SIZE * TILE_HEIGHT)));
		}

		sectors[sectorAt]->collisionComponents.push_back(*it);
	}
}

PhysicsManager::Sector::Sector(MyRectangle _rectangle)
{
	rectangle = new MyRectangle(_rectangle);
}

PhysicsManager::Sector::~Sector()
{
	SafeDelete(rectangle);
}
