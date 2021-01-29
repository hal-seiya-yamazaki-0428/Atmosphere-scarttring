
#include "main.h"
#include "renderer.h"
#include <io.h>

D3D_FEATURE_LEVEL       CRenderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           CRenderer::m_D3DDevice = NULL;
ID3D11DeviceContext*    CRenderer::m_ImmediateContext = NULL;
IDXGISwapChain*         CRenderer::m_SwapChain = NULL;
ID3D11RenderTargetView* CRenderer::m_RenderTargetView = NULL;
ID3D11DepthStencilView* CRenderer::m_DepthStencilView = NULL;

ID3D11RenderTargetView* CRenderer::m_ShadowRenderTargetView = NULL;
ID3D11ShaderResourceView* CRenderer::m_ShadowDepthShaderResourceView = NULL;

ID3D11RenderTargetView*   CRenderer::m_PassRenderTarget[PASS_COUNT] = { NULL };
ID3D11ShaderResourceView* CRenderer::m_PassShaderResourceView[PASS_COUNT] = { NULL };
ID3D11Texture2D*		  CRenderer::m_PassTexture[PASS_COUNT] = { NULL };

ID3D11SamplerState* CRenderer::m_SamplerState = NULL;
ID3D11SamplerState* CRenderer::m_ShadowSamplerState = NULL;

ID3D11Buffer*			CRenderer::m_WorldBuffer = NULL;
ID3D11Buffer*			CRenderer::m_ViewBuffer = NULL;
ID3D11Buffer*			CRenderer::m_ProjectionBuffer = NULL;
ID3D11Buffer*			CRenderer::m_MaterialBuffer = NULL;
ID3D11Buffer*			CRenderer::m_LightBuffer = NULL;
ID3D11Buffer*			CRenderer::m_CameraBuffer = NULL;
ID3D11Buffer*			CRenderer::m_ParameterBuffer = NULL;
ID3D11Buffer*			CRenderer::m_AtmosphereBuffer = NULL;
ID3D11Buffer*			CRenderer::m_GaussBuffer = NULL;

ID3D11DepthStencilState* CRenderer::m_DepthStateEnable = NULL;
ID3D11DepthStencilState* CRenderer::m_DepthStateDisable = NULL;

//-------------------------------------------------------------------------------------------------
//      ブラーパラメータを計算します.
//-------------------------------------------------------------------------------------------------
GaussBlurParam CRenderer::CalcBlurParam(float power, float width, float height)
{
	GaussBlurParam result;
	float total = 0;
	float disperision = power;
	for (int i = 0; i < 8; i++) {
		float pos = 1.0f + 2.0f * (float)i;
		result.weight[i] = exp(-0.5f * pos * pos / disperision);
		if (i == 0) {
			total += result.weight[i];
		}
		else {
			total += 2.0f * result.weight[i];
		}
	}

	for (int i = 0; i < 8; i++) {
		result.weight[i] /= total;
	}

	result.texsize.x = width;
	result.texsize.y = height;

	return result;
}

void CRenderer::Init()
{
	HRESULT hr = S_OK;

	// デバイス、スワップチェーン、コンテキスト生成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = GetWindow();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_SwapChain,
		&m_D3DDevice,
		&m_FeatureLevel,
		&m_ImmediateContext);


	// レンダーターゲットビュー生成、設定
	ID3D11Texture2D* pBackBuffer = NULL;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	pBackBuffer->Release();

	{
		//ステンシル用テクスチャー作成
		ID3D11Texture2D* depthTexture = NULL;
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = sd.BufferDesc.Width;
		td.Height = sd.BufferDesc.Height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		td.SampleDesc = sd.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		m_D3DDevice->CreateTexture2D(&td, NULL, &depthTexture);

		//ステンシルターゲット作成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = td.Format;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Flags = 0;
		m_D3DDevice->CreateDepthStencilView(depthTexture, &dsvd, &m_DepthStencilView);

		depthTexture->Release();
	}

	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports(1, &vp);

	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID; //ワイヤーフレーム切り替え
	//rd.FillMode = D3D11_FILL_WIREFRAME; //ワイヤーフレーム切り替え
	rd.CullMode = D3D11_CULL_BACK;
	//rd.CullMode = D3D11_CULL_FRONT;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	m_D3DDevice->CreateRasterizerState(&rd, &rs);

	m_ImmediateContext->RSSetState(rs);

	// ブレンドステート設定
	SetDefaultBlend();

	// 深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//深度無効ステート

	m_ImmediateContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);

	{
		// サンプラーステート設定
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_D3DDevice->CreateSamplerState(&samplerDesc, &m_SamplerState);

	}

	{
		// サンプラーステート設定
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		m_D3DDevice->CreateSamplerState(&samplerDesc, &m_ShadowSamplerState);

	}

	m_ImmediateContext->PSSetSamplers(0, 1, &m_SamplerState);

	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_WorldBuffer);
	m_ImmediateContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);
	m_ImmediateContext->PSSetConstantBuffers(0, 1, &m_WorldBuffer);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_ViewBuffer);
	m_ImmediateContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);
	m_ImmediateContext->PSSetConstantBuffers(1, 1, &m_ViewBuffer);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_ProjectionBuffer);
	m_ImmediateContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);
	m_ImmediateContext->PSSetConstantBuffers(2, 1, &m_ProjectionBuffer);

	hBufferDesc.ByteWidth = sizeof(MATERIAL);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_MaterialBuffer);
	m_ImmediateContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);
	m_ImmediateContext->PSSetConstantBuffers(3, 1, &m_MaterialBuffer);

	hBufferDesc.ByteWidth = sizeof(LIGHT);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_LightBuffer);
	m_ImmediateContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
	m_ImmediateContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);

	hBufferDesc.ByteWidth = sizeof(D3DXVECTOR4);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_CameraBuffer);
	m_ImmediateContext->PSSetConstantBuffers(5, 1, &m_CameraBuffer);

	hBufferDesc.ByteWidth = sizeof(D3DXVECTOR4);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_ParameterBuffer);
	m_ImmediateContext->PSSetConstantBuffers(6, 1, &m_ParameterBuffer);

	hBufferDesc.ByteWidth = sizeof(Atmosphere);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_AtmosphereBuffer);
	m_ImmediateContext->PSSetConstantBuffers(7, 1, &m_AtmosphereBuffer);


	hBufferDesc.ByteWidth = sizeof(GaussBlurParam);

	m_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &m_GaussBuffer);
	m_ImmediateContext->PSSetConstantBuffers(8, 1, &m_GaussBuffer);

	// ライト無効化
	LIGHT light;
	light.Enable = false;
	SetLight(light);

	// マテリアル初期化
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	{
		// 2次元テクスチャの設定
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = sd.BufferDesc.Width;
		td.Height = sd.BufferDesc.Height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
		td.SampleDesc = sd.SampleDesc;
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

		for (int i = 0; i < PASS_COUNT; i++)
		{
			m_D3DDevice->CreateTexture2D(&td, NULL, &m_PassTexture[i]);
			m_D3DDevice->CreateRenderTargetView(m_PassTexture[i], &rtvDesc, &m_PassRenderTarget[i]);
			m_D3DDevice->CreateShaderResourceView(m_PassTexture[i], &srvDesc, &m_PassShaderResourceView[i]);
		}
	}
	{
		// 2次元テクスチャの設定
		ID3D11Texture2D *TexCoord;
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = sd.BufferDesc.Width;
		td.Height = sd.BufferDesc.Height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R16G16B16A16_TYPELESS;
		td.SampleDesc = sd.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		// レンダーターゲットビューの設定
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		memset(&rtvDesc, 0, sizeof(rtvDesc));
		rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		// シェーダリソースビューの設定
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		memset(&srvDesc, 0, sizeof(srvDesc));
		srvDesc.Format = rtvDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		m_D3DDevice->CreateTexture2D(&td, NULL, &TexCoord);
		m_D3DDevice->CreateRenderTargetView(TexCoord, &rtvDesc, &m_ShadowRenderTargetView);
		m_D3DDevice->CreateShaderResourceView(TexCoord, &srvDesc, &m_ShadowDepthShaderResourceView);
		TexCoord->Release();
	}
}

void CRenderer::Uninit()
{
	// オブジェクト解放
	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();
	m_CameraBuffer->Release();
	m_ParameterBuffer->Release();
	m_AtmosphereBuffer->Release();

	m_ImmediateContext->ClearState();
	m_RenderTargetView->Release();
	m_SwapChain->Release();
	m_ImmediateContext->Release();
	m_D3DDevice->Release();

	m_ShadowRenderTargetView->Release();
	m_ShadowDepthShaderResourceView->Release();

	m_SamplerState->Release();
	m_ShadowSamplerState->Release();

	for (int i = 0; i < PASS_COUNT; i++)
	{
		m_PassRenderTarget[i]->Release();
		m_PassShaderResourceView[i]->Release();
		m_PassTexture[i]->Release();
	}
}

void CRenderer::Begin()
{
	m_ImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	// バックバッファクリア
	float ClearColor[4] = { 0.0f, 0.8f, 0.0f, 1.0f };
	m_ImmediateContext->ClearRenderTargetView(m_RenderTargetView, ClearColor);
	m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CRenderer::BeginRenderTargetViewSwitch(int num)
{
	m_ImmediateContext->OMSetRenderTargets(1, &m_PassRenderTarget[num], m_DepthStencilView);

	// バックバッファクリア
	float ClearColor[4] = { 0.0f, 0.5f, 0.0f, 1.0f };
	m_ImmediateContext->ClearRenderTargetView(m_PassRenderTarget[num], ClearColor);
	m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CRenderer::BeginDepth()
{
	m_ImmediateContext->OMSetRenderTargets(1, &m_ShadowRenderTargetView, m_DepthStencilView);

	// バックバッファクリア
	float ClearColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
	m_ImmediateContext->ClearRenderTargetView(m_ShadowRenderTargetView, ClearColor);
	m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CRenderer::End()
{
	m_SwapChain->Present(1, 0);
}

void CRenderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		m_ImmediateContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
	else
		m_ImmediateContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);

}

void CRenderer::SetWorldViewProjection2D()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	m_ImmediateContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);
	m_ImmediateContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);

	D3DXMATRIX projection;
	D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&projection, &projection);
	m_ImmediateContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);

}

void CRenderer::SetWorldMatrix(D3DXMATRIX *WorldMatrix)
{
	D3DXMATRIX world;
	D3DXMatrixTranspose(&world, WorldMatrix);
	m_ImmediateContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);
}

void CRenderer::SetViewMatrix(D3DXMATRIX *ViewMatrix)
{
	D3DXMATRIX view;
	D3DXMatrixTranspose(&view, ViewMatrix);
	m_ImmediateContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
}

void CRenderer::SetProjectionMatrix(D3DXMATRIX *ProjectionMatrix)
{
	D3DXMATRIX projection;
	D3DXMatrixTranspose(&projection, ProjectionMatrix);
	m_ImmediateContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}

void CRenderer::SetMaterial(MATERIAL Material)
{

	m_ImmediateContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &Material, 0, 0);

}

void CRenderer::SetLight(LIGHT Light)
{
	D3DXMatrixTranspose(&Light.ViewMatrix, &Light.ViewMatrix);
	D3DXMatrixTranspose(&Light.ProjectionMatrix, &Light.ProjectionMatrix);
	m_ImmediateContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);

}

void CRenderer::SetCameraPosition(D3DXVECTOR3 CameraPosition)
{
	m_ImmediateContext->UpdateSubresource(m_CameraBuffer, 0, NULL, &D3DXVECTOR4(CameraPosition.x, CameraPosition.y, CameraPosition.z, 1.0f), 0, 0);
}

void CRenderer::SetParameter(D3DXVECTOR4 Parameter)
{
	m_ImmediateContext->UpdateSubresource(m_ParameterBuffer, 0, NULL, &Parameter, 0, 0);
}

void CRenderer::SetAtmosphere(Atmosphere atmosphere)
{
	m_ImmediateContext->UpdateSubresource(m_AtmosphereBuffer, 0, NULL, &atmosphere, 0, 0);
}

void CRenderer::SetGaussBuler(GaussBlurParam gauss)
{
	m_ImmediateContext->UpdateSubresource(m_GaussBuffer, 0, NULL, &gauss, 0, 0);
}

// 頂点シェーダ生成
void CRenderer::CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName)
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_D3DDevice->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	// 入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_D3DDevice->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}

//ジオメトリシェーダ生成
void CRenderer::CreateGeometryShader(ID3D11GeometryShader** GeometryShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_D3DDevice->CreateGeometryShader(buffer, fsize, NULL, GeometryShader);

	delete[] buffer;
}

// ピクセルシェーダ生成
void CRenderer::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_D3DDevice->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}

void CRenderer::ChangeFillMode(bool wire_frame)
{
	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	//rd.FillMode = D3D11_FILL_SOLID; //ワイヤーフレーム切り替え
	rd.FillMode = wire_frame ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID; //ワイヤーフレーム切り替え
	rd.CullMode = D3D11_CULL_BACK;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	m_D3DDevice->CreateRasterizerState(&rd, &rs);

	m_ImmediateContext->RSSetState(rs);
}

void CRenderer::SetClearZBuffer()
{
	m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void CRenderer::SetAddBlend()
{
	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_D3DDevice->CreateBlendState(&blendDesc, &blendState);
	m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);
}

void CRenderer::SetDefaultBlend()
{
	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_D3DDevice->CreateBlendState(&blendDesc, &blendState);
	m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);
}

void CRenderer::SetDefaultSampleState()
{
	m_ImmediateContext->PSSetSamplers(0, 1, &m_SamplerState);
}

void CRenderer::SetShadowSampleState()
{
	m_ImmediateContext->PSSetSamplers(0, 1, &m_ShadowSamplerState);
}

void CRenderer::SetCulMode(bool cul)
{
	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = (cul) ? D3D11_CULL_FRONT : D3D11_CULL_BACK;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	m_D3DDevice->CreateRasterizerState(&rd, &rs);

	m_ImmediateContext->RSSetState(rs);
}