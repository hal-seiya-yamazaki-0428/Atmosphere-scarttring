#ifndef _SKY_DOME_H_
#define _SKY_DOME_H_

#include "game_object.h"

class CSkyDome : public CGameObject
{
private:
	class CShader *m_Shader;
	class CModel *m_Model;
public:
	CSkyDome() {}
	~CSkyDome() {}
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

#endif