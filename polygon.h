#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "game_object.h"

class CPolygon : public CGameObject
{
private:
	CShader* m_Shader;
	CSprite *m_Sprite;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Draw(ID3D11ShaderResourceView* srv, CShader* shader);
	void FullScreenDraw(ID3D11ShaderResourceView* srv, CShader* shader);
};

#endif _POLYGON_H_