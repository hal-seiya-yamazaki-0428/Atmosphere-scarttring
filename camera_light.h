#ifndef _CAMERA_LIGHT_H_
#define _CAMERA_LIGHT_H_

#include "camera.h"

class CLightCamera : public CCamera
{
private:
	float m_Near;
	float m_Far;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void ImGuiDraw();
};

#endif