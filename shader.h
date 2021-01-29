#ifndef _SHADER_H_
#define _SHADER_H_

class CShader
{
private:
	ID3D11VertexShader *m_VertexShader;
	ID3D11PixelShader *m_PixelShader;
	ID3D11InputLayout *m_VertexLayout;

	static ID3D11ShaderResourceView *m_ToonMap;

public:
	CShader() = delete;
	CShader(const char *vs_file, const char *ps_file);
	virtual ~CShader();

	static void ToonTextureLoad();
	static void ToonTextureUnload();
	virtual void ShaderDraw(bool toon_texture = false);
};

#endif