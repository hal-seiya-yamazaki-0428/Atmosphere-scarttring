#ifndef _ATMOSPHERE_H_
#define _ATMOSPHERE_H_

class CAtmosphere
{
private:
	static ID3D11ShaderResourceView *m_Texture[2];
	static Atmosphere m_Atm;
	static bool		  m_Stop;
	static bool       m_On;
	static int		  m_TimeRate;
	static float	  m_TimeFrame;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static D3DXVECTOR3 GetLightDir() { return m_Atm.LightDir; }
};

#endif