#ifndef _MOUSE_H_
#define _MOUSE_H_

class CMouse
{
private:
	static POINT m_MousePos;

public:
	static void Init();
	static void Uninit();
	static void Update();

	//ÉQÉbÉgä÷êî
	static POINT GetMousePos() { return m_MousePos; }
	static void GetScreenToWorld(D3DXVECTOR3* pout, int Sx, int Sy, float fZ);
	static void GetScreenToXZ(D3DXVECTOR3* pout, int Sx, int Sy);
	static bool GetLineHit(D3DXVECTOR3 center, float radius, int Sx, int Sy);

};

#endif