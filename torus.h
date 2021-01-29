#ifndef _TORUS_H_
#define _TORUS_H_

#include "game_object.h"

class CTorus : public CGameObject
{
private:
	CModel* m_Model;

	class CShader *m_DefaultShader;
	class CShader *m_ShadowShader;

	D3DXVECTOR3 m_RotVelocity;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void ShadowShaderDraw() override;
	void DefaultShaderDraw() override;

	void SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 rot_velocity);
};

#endif