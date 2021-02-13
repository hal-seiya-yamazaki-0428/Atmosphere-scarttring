#ifndef _SHADOW_H_
#define _SHADOW_H_

class CShadow
{
private:
	static ID3D11RenderTargetView* m_RenderTarget;
	static ID3D11ShaderResourceView* m_ShaderResourceView;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Create();

	static ID3D11ShaderResourceView* GetShadowTexture() { return m_ShaderResourceView; }
};

#endif