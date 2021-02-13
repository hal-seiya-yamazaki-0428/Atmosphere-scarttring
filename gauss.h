#ifndef _CGAUSS_H_
#define _CGAUSS_H_

#define GAUSS_COUNT (2)

class CGauss
{
private:
	static class CPolygon* m_Polygon;
	static CShader* m_GaussX;
	static CShader* m_GaussY;
	static GaussBlurParam m_Parameter;

	static ID3D11RenderTargetView*   m_RenderTarget[GAUSS_COUNT];
	static ID3D11ShaderResourceView* m_ShaderResourceView[GAUSS_COUNT];
	static ID3D11Texture2D*		  m_Texture2D[GAUSS_COUNT];

public:
	static void Init();
	static void Uninit();
	static void Update();
	static ID3D11ShaderResourceView* Create(ID3D11ShaderResourceView* texture);

	static void Begin(int path);

	static void SetParameter(float power, float width, float height);

	static ID3D11ShaderResourceView* GetSRV(int index) { return m_ShaderResourceView[index]; }
};

#endif

