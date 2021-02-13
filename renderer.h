#pragma once

// 頂点構造体
struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};



// マテリアル構造体
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];//16byte境界用
};




// マテリアル構造体
struct DX11_MODEL_MATERIAL
{
	MATERIAL		Material;
	class CTexture*	Texture;
};


// 描画サブセット構造体
struct DX11_SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};

struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];//16byte境界用
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;

	D3DXMATRIX ViewMatrix;
	D3DXMATRIX ProjectionMatrix;
};

//大気散乱構造体
struct Atmosphere
{
	D3DXVECTOR3 Color;
	float Dummy;
	D3DXVECTOR3 LightDir;
	float Kr;
	float Km;
	float SunPower;
	float ScaleDepth;
	float g;
	D3DXVECTOR4 lum;
};

struct GaussBlurParam
{
	float weight[8];
	D3DXVECTOR2 texsize;
	D3DXVECTOR2 Dummy;
};


class CVertexBuffer;
class CIndexBuffer;
class CTexture;

class CRenderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_D3DDevice;
	static ID3D11DeviceContext*    m_ImmediateContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11SamplerState* m_SamplerState;
	static ID3D11SamplerState* m_ShadowSamplerState;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*			m_CameraBuffer;
	static ID3D11Buffer*			m_ParameterBuffer;
	static ID3D11Buffer*			m_AtmosphereBuffer;
	static ID3D11Buffer*			m_GaussBuffer;


	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;




public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetClearZBuffer();
	static void SetDepthEnable(bool Enable);

	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX * WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX * ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX * ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetCameraPosition(D3DXVECTOR3 CameraPosition);
	static void SetParameter(D3DXVECTOR4 Parameter);
	static void SetAtmosphere(Atmosphere atmosphere);
	static void SetGaussBuler(GaussBlurParam gauss);

	static void SetAddBlend();
	static void SetDefaultBlend();

	static void SetDefaultSampleState();
	static void SetShadowSampleState();

	static ID3D11DepthStencilView* GetDepthStencilView() { return m_DepthStencilView; }

	static ID3D11Device* GetDevice( void ){ return m_D3DDevice; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_ImmediateContext; }

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreateGeometryShader(ID3D11GeometryShader** GeometryShader, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	static void ChangeFillMode(bool wire_frame);
	static void SetCulMode(bool cul);
};
