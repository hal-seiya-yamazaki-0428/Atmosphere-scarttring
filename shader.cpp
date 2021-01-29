
#include "main.h"
#include "renderer.h"
#include "manager.h"

#include "shader.h"

//===========================================
//グローバル変数
//===========================================
ID3D11ShaderResourceView *CShader::m_ToonMap = NULL;

//===========================================
//初期化
//===========================================
CShader::CShader(const char *vs_file, const char *ps_file)
{
	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, vs_file);
	CRenderer::CreatePixelShader(&m_PixelShader, ps_file);
}

//===========================================
//終了
//===========================================
CShader::~CShader()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

//===========================================
//描画時設定
//===========================================
void CShader::ShaderDraw(bool toon_texture)
{
	//トゥーンマップの設定
	if (toon_texture)
	{
		CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_ToonMap);
	}
	//入力レイアウト
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	//シェーダー設定
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
}

//===========================================
//トゥーンマップ生成
//===========================================
void CShader::ToonTextureLoad()
{
	//トゥーンテクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/toon_map.png",
		NULL,
		NULL,
		&m_ToonMap,
		NULL);

	assert(m_ToonMap);
}

//===========================================
//トゥーンマップ解放
//===========================================
void CShader::ToonTextureUnload()
{
	m_ToonMap->Release();
}