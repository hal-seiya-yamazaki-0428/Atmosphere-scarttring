#ifndef _LIGHT_H_
#define _LIGHT_H_

class CLight
{
private:
	static LIGHT m_Light;
public:
	static void SetDefaultLight();
	static void SetLightColor(D3DXCOLOR *ambient, D3DXCOLOR *diffuse);
	static void SetLightOn(bool on);
};

#endif