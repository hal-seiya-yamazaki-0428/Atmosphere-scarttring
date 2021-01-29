#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "sprite.h"
#include "shader.h"
#include "light.h"

#include "polygon.h"

ID3D11ShaderResourceView *g_Texture = NULL;

void CPolygon::Init()
{ 
	m_Shader = new CShader("unlitTextureVS.cso", "unlitTexturePS.cso");

	m_Sprite = new CSprite;
	m_Sprite->Load_2D("asset/texture/number.png", 256, 256);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/pattern_tile.png",
		NULL,
		NULL,
		&g_Texture,
		NULL);

	assert(g_Texture);
}

void CPolygon::Uninit()
{
	delete m_Shader;

	m_Sprite->Uninit();
	delete m_Sprite;
}

void CPolygon::Update()
{

}

void CPolygon::Draw()
{
	//ライト無効
	CLight::SetLightOn(false);

	m_Shader->ShaderDraw();

	m_Sprite->Draw(D3DXVECTOR2(0.0f, 0.0f), 256, 256);

	CLight::SetLightOn(true);
}

void CPolygon::Draw(ID3D11ShaderResourceView * srv, CShader* shader)
{
	CRenderer::SetShadowSampleState();

	shader->ShaderDraw();

	m_Sprite->Draw(D3DXVECTOR2(0.0f, 0.0f), 256, 256, srv);

	CRenderer::SetDefaultSampleState();
}

void CPolygon::FullScreenDraw(ID3D11ShaderResourceView * srv, CShader* shader)
{
	CRenderer::SetShadowSampleState();

	shader->ShaderDraw();

	m_Sprite->Draw(D3DXVECTOR2(0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, srv);

	CRenderer::SetDefaultSampleState();
}
