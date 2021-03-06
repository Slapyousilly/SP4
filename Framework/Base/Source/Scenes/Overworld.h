#ifndef OVERWORLD_H
#define OVERWORLD_H

#include "Scene.h"
#include <vector>

#include "../Overworld/CameraFollow.h"
#include "../Overworld/OverworldEntity.h"
#include "../Overworld/OverworldAsset.h"
#include "../Overworld/TriggerArea.h"
#include "../Overworld/NPC.h"

#include "../Entity/SpriteEntity.h"
#include "../Scenes/BattleState.h"
#include "../SpatialPartition/SpatialPartition.h"


using std::vector;

class Overworld : public Scene
{
public:
	Overworld();
	virtual ~Overworld();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Exit();

	virtual void UnPause();
	
	bool battle;

private:
	CameraFollow camera;
	SpriteEntity* background;
    CBattleState* battlestate;
	CSpatialPartition spatial;
	NPC* npc;

	OverworldEntity* player;
};

#endif