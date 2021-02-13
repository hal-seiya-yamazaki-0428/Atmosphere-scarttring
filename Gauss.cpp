
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "sprite.h"

#include "polygon.h"
#include "gauss.h"

CPolygon* CGauss::m_Polygon;
CShader* CGauss::m_GaussX;
CShader* CGauss::m_GaussY;
GaussBlurParam CGauss::m_Parameter;

ID3D11RenderTargetView*   CGauss::m_RenderTarget[GAUSS_COUNT];
ID3D11ShaderResourceView* CGauss::m_ShaderResourceView[GAUSS_COUNT];
ID3D11Texture2D*		  CGauss::m_Texture2D[GAUSS_COUNT];

//===========================================
//初期化
//===========================================
void CGauss::Init()
{
	{
		// 2次元テクスチャの設定
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = SCREEN_WIDTH;
		td.Height = SCREEN_HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		// レンダーターゲットビューの設定
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		memset(&rtvDesc, 0, sizeof(rtvDesc));
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		// シェーダリソースビューの設定
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		memset(&srvDesc, 0, sizeof(srvDesc));
		srvDesc.Format = rtvDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		for (int i = 0; i < GAUSS_COUNT; i++)
		{
			CRenderer::GetDevice()->CreateTexture2D(&td, NULL, &m_Texture2D[i]);
			CRenderer::GetDevice()->CreateRenderTargetView(m_Texture2D[i], &rtvDesc, &m_RenderTarget[i]);
			CRenderer::GetDevice()->CreateShaderResourceView(m_Texture2D[i], &srvDesc, &m_ShaderResourceView[i]);
		}
	}

	m_GaussX = new CShader("gaussVS.cso", "gaussXPS.cso");
	m_GaussY = new CShader("gaussVS.cso", "gaussYPS.cso");

	m_Polygon = new CPolygon;
	m_Polygon->Init();
}

//===========================================
//終了
//===========================================
void CGauss::Uninit()
{
	for (int i = 0; i < GAUSS_COUNT; i++)
	{
		m_RenderTarget[i]->Release();
		m_ShaderResourceView[i]->Release();
		m_Texture2D[i]->Release();
	}

	delete m_GaussX;
	delete m_GaussY;
	m_Polygon->Uninit();
	delete m_Polygon;
}

//===========================================
//更新
//===========================================
void CGauss::Update()
{
	float w = (float)SCREEN_WIDTH;
	float h = (float)SCREEN_HEIGHT;
	// パラメータ設定
	SetParameter(32.0f, w, h);
	CRenderer::SetGaussBuler(m_Parameter);
}

//===========================================
//描画
//===========================================
ID3D11ShaderResourceView* CGauss::Create(ID3D11ShaderResourceView* texture)
{
	//1パス目
	Begin(0);
	//縦方向ガウスフィルター
	m_Polygon->FullScreenDraw(texture, m_GaussY);

	//2パス目
	Begin(1);
	//横方向ガウスフィルター
	m_Polygon->FullScreenDraw(GetSRV(0), m_GaussX);

	return GetSRV(1);
}

//===========================================
//レンダーターゲット切り替え
//===========================================
void CGauss::Begin(int path)
{
	if (path > GAUSS_COUNT)
	{
		assert(!"領域外のレンダーターゲットにアクセスしようとしています");
		return;
	}
	auto depth = CRenderer::GetDepthStencilView();
	CRenderer::GetDeviceContext()->OMSetRenderTargets(1, &m_RenderTarget[path], depth);

	// バックバッファクリア
	float ClearColor[4] = { 0.0f, 0.8f, 0.0f, 1.0f };
	CRenderer::GetDeviceContext()->ClearRenderTargetView(m_RenderTarget[path], ClearColor);
	CRenderer::GetDeviceContext()->ClearDepthStencilView(depth, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//===========================================
//ガウス用パラメータ設定
//===========================================
void CGauss::SetParameter(float power, float width, float height)
{
	float total = 0;
	float disperision = power;
	for (int i = 0; i < 8; i++) {
		float pos = 1.0f + 2.0f * (float)i;
		m_Parameter.weight[i] = exp(-0.5f * pos * pos / disperision);
		if (i == 0) {
			total += m_Parameter.weight[i];
		}
		else {
			total += 2.0f * m_Parameter.weight[i];
		}
	}

	for (int i = 0; i < 8; i++) {
		m_Parameter.weight[i] /= total;
	}

	m_Parameter.texsize.x = width;
	m_Parameter.texsize.y = height;
}