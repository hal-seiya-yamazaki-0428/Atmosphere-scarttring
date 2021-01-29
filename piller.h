#ifndef _PILLER_H_
#define _PILLER_H_

#include "game_object.h"

class CPiller : public CGameObject
{
private:
	CModel* m_Model;

	class CShader *m_DefaultShader;
	class CShader *m_ShadowShader;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void ShadowShaderDraw() override;
	void DefaultShaderDraw() override;
};

#endif