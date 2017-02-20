#include "Grid.h"
#include "stdio.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "Collider\Collider.h"
#include "SpatialPartition.h"

/********************************************************************************
Constructor
********************************************************************************/
CGrid::CGrid(void)
	: index(Vector3(-1, -1, -1))
	, size(Vector3(-1, -1, -1))
	, offset(Vector3(-1, -1, -1))
	, min(Vector3(-1, -1, -1))
	, max(Vector3(-1, -1, -1))
	, theMesh(NULL)
	, ListOfObjects(NULL)
{
}

/********************************************************************************
Destructor
********************************************************************************/
CGrid::~CGrid(void)
{
	if (theMesh)
	{
		delete theMesh;
		theMesh = NULL;
	}
	Remove();
}

/********************************************************************************
Initialise this grid
********************************************************************************/
void CGrid::Init(	const int xIndex, const int yIndex,
					const int xGridSize, const int yGridSize,
					const float xOffset, const float yOffset)
{
	index.Set(xIndex, yIndex, 0);
	size.Set(xGridSize, yGridSize, 0);
	offset.Set(xOffset, yOffset, 0);
	min.Set(index.x * size.x - offset.x, 0.0f, index.y * size.y - offset.y);
	max.Set(index.x * size.x - offset.x + xGridSize, 0.0f, index.y * size.y - offset.y + yGridSize);
}

/********************************************************************************
 Set a particular grid's Mesh
********************************************************************************/
void CGrid::SetMesh(const std::string& _meshName)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh != nullptr)
	{
		theMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	}
}

/********************************************************************************
Update the grid
********************************************************************************/
void CGrid::Update(vector<EntityBase*>* migrationList)
{
	// Check each object to see if they are no longer in this grid
	std::vector<EntityBase*>::iterator it;
	it = ListOfObjects.begin();
	while (it != ListOfObjects.end())
	{
		// Check if the entity has a collider
		if ((*it)->HasCollider())
		{
			Vector3 colliderMax = (*it)->GetCollider()->GetMax();
			Vector3 colliderMin = (*it)->GetCollider()->GetMin();
			
			// Checks if bounding box max and min is within grid
			if (colliderMin.x < max.x &&
				colliderMax.x > min.x &&
				colliderMin.y < max.y &&
				colliderMax.y > min.y)
			{
				// Checks if any part of the bounding box is leaving the grid.
				bool check = false;
				if (colliderMin.x < min.x)
				{
					migrationList->push_back((*it));
					check = true;
				}
				if (colliderMax.x > max.x)
				{
					migrationList->push_back((*it));
					check = true;
				}
				if (colliderMin.y < min.y)
				{
					migrationList->push_back((*it));
					check = true;
				}
				if (colliderMax.y > max.y)
				{
					migrationList->push_back((*it));
					check = true;
				}

				if (check)
				{
					it = ListOfObjects.erase(it);
				}
				else
					++it;
			}
			else
			{
				// Bounding box has completely left grid
				migrationList->push_back((*it));
				it = ListOfObjects.erase(it);
			}
		}
		else
		{
			Vector3 position = (*it)->GetPosition();
			// Check is entity's position is within grid
			if (position.x > min.x && position.x < max.x &&
				position.y > min.y && position.y < min.y)
				it++;
			else
			{
				// Entity has left the grid
				migrationList->push_back((*it));
				it = ListOfObjects.erase(it);
			}
		}
	}
}

/********************************************************************************
RenderScene
********************************************************************************/
void CGrid::Render(void)
{
	if (theMesh && ListOfObjects.size())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		RenderHelper::RenderMesh(theMesh);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

/********************************************************************************
Add a new object to this grid
********************************************************************************/
void CGrid::Add(EntityBase* theObject)
{
	for (int i = 0; i < ListOfObjects.size(); ++i)
	{
		if (ListOfObjects[i] == theObject)
			return;
	}
	ListOfObjects.push_back( theObject );
}

/********************************************************************************
 Remove but not delete object from this grid
********************************************************************************/
void CGrid::Remove(void)
{
	for( int i = 0 ; i < ListOfObjects.size(); i++)
	{
		// Do not delete the objects as they are stored in EntityManager and will be deleted there.
		//delete ListOfObjects[i];
		ListOfObjects[i] = NULL;
	}
	ListOfObjects.clear();
}

/********************************************************************************
 Remove but not delete an object from this grid
********************************************************************************/
bool CGrid::Remove(EntityBase* theObject)
{
	// Clean up entities that are done
	std::vector<EntityBase*>::iterator it, end;
	it = ListOfObjects.begin();
	end = ListOfObjects.end();
	while (it != end)
	{
		if ((*it) == theObject)
		{
			it = ListOfObjects.erase(it);
			return true;
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
	return false;
}

/********************************************************************************
 Check if an object is in this grid
********************************************************************************/
bool CGrid::IsHere(EntityBase* theObject) const
{
	for (int i = 0; i < ListOfObjects.size(); ++i)
	{
		if (ListOfObjects[i] == theObject)
			return true;
	}
	return false;
}

/********************************************************************************
Get list of objects in this grid
********************************************************************************/
vector<EntityBase*> CGrid::GetListOfObject(void)
{
	return ListOfObjects;
}

/********************************************************************************
 PrintSelf
 ********************************************************************************/
void CGrid::PrintSelf()
{
	if (ListOfObjects.size() > 0)
	{
		cout << "CGrid::PrintSelf()" << endl;
		cout << "\tIndex\t:\t" << index << "\t\tOffset\t:\t" << offset << endl;
		cout << "\tMin\t:\t" << min << "\tMax\t:\t" << max << endl;
		cout << "\tNum of objects: " << ListOfObjects.size() << endl;
		cout << "\t------------------------------------------------------------------------" << endl;
		for (int i = 0; i < ListOfObjects.size(); ++i)
		{
			cout << "\t" << i << "\t:\t" << ListOfObjects[i]->GetPosition() << endl;
		}
		if (ListOfObjects.size()>0)
			cout << "\t------------------------------------------------------------------------" << endl;
		cout << "********************************************************************************" << endl;
	}
}

void CGrid::CheckForCollision()
{
	std::vector<EntityBase*>::iterator it, otherIt;
	for (it = ListOfObjects.begin(); it != ListOfObjects.end(); ++it)
	{
		if ((*it)->HasCollider() && !(*it)->IsDone())
		{
			for (otherIt = it + 1; otherIt != ListOfObjects.end(); ++otherIt)
			{
				if ((*otherIt)->HasCollider() && !(*otherIt)->IsDone())
				{
					if ((*it)->GetCollider()->CheckCollision((*otherIt)->GetCollider()))
					{
						(*it)->HandleCollision(*otherIt);
						(*otherIt)->HandleCollision(*it);
					}
				}
			}
		}
	}

	CheckDead();
}

void CGrid::CheckDead()
{
	std::vector<EntityBase*>::iterator it = ListOfObjects.begin();
	while (it != ListOfObjects.end())
	{
		if ((*it)->IsDone())
			it = ListOfObjects.erase(it);
		else
			++it;
	}
}
