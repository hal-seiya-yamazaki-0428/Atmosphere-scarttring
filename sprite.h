#ifndef _SPRITE_H_
#define _SPRITE_H_

class CSprite
{
private:
	ID3D11Buffer *m_VertexBuffer = NULL;
	ID3D11ShaderResourceView *m_Texture = NULL;
public:
	CSprite() {}
	~CSprite() {}

	void LoadLine();
	void Load_2D(std::string pFilename, int width, int height);
	void Load_3D(std::string pFilename, bool uv = false);
	void Uninit();

	void DrawLine(D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR color);
	void Draw(D3DXVECTOR2 pos, int width, int height);
	void Draw(D3DXVECTOR2 pos, int width, int height, ID3D11ShaderResourceView* srv);
	void Draw(D3DXVECTOR2 pos, int width, int height, float tx, float ty, float tu, float tv);
	void Draw(D3DXVECTOR2 pos, int width, int height, D3DXCOLOR color);
	void Draw(D3DXMATRIX mtxWorld);
	void Draw(D3DXMATRIX mtxWorld, D3DXCOLOR color);
	void Draw(D3DXMATRIX mtxWorld, float x, float y, float width, float height);
	void Draw(D3DXMATRIX mtxWorld, float x, float y, float width, float height, D3DXCOLOR color);
	void Draw(D3DXMATRIX mtxWorld, int u, int v, int count, int delay);
};

#endif