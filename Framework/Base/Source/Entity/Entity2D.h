#ifndef ENTITY_2D_H
#define ENTITY_2D_H

#include "EntityBase.h"
#include "../Animation/Animator.h"

class Entity2D : public EntityBase
{
	Animator anim;

public:
	Entity2D();
	virtual ~Entity2D();

	virtual void Update();
	virtual void Render();
	virtual void RenderUI();

	inline Animator* GetAnimator(){ return &anim;};
};

#endif