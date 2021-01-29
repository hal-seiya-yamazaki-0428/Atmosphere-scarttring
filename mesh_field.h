#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

#include "game_object.h"

#define MESH_X (12)
#define MESH_Z (12)


class CMeshField : public CGameObject
{
private:
	ID3D11Buffer *m_VertexBuffer = NULL;
	ID3D11Buffer *m_IndexBuffer = NULL;
	ID3D11ShaderResourceView *m_Texture = NULL;
	
	class CShader *m_DefaultShader;
	class CShader *m_ShadowShader;

	VERTEX_3D m_Vertex[MESH_X][MESH_Z];//21. 21

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void ShadowShaderDraw() override;
	void DefaultShaderDraw() override;

	float GetHeight(D3DXVECTOR3 Position);
};

#endif