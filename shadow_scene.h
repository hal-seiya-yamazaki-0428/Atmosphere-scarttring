#ifndef _SHADOW_SCENE_H_
#define _SHADOW_SCENE_H_

#include "scene.h"

class CShadowScene : public CScene
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

#endif