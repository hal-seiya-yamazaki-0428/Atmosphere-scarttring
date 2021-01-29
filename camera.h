#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "game_object.h"

struct MOUSE
{
	POINT pos;
	POINT pos_click;
	POINT velocity;
	POINT velocity_release;
};

class CCamera : public CGameObject
{
private:
	MOUSE m_Mouse;
	float m_Angle;
	float m_Velocity;

protected:
	D3DXVECTOR3 m_Target;			//注視点
	D3DXMATRIX m_ViewMatrix;		//ビュー行列
	D3DXMATRIX m_ProjectionMatrix;	//プロジェクション行列
	D3DXVECTOR3 m_vecDir;


public:
	CCamera() {}
	~CCamera() {}

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	//ゲット関数
	D3DXMATRIX GetInv();
	D3DXMATRIX GetViewMtx() { return m_ViewMatrix; }
	D3DXMATRIX GetProjectionMtx() { return m_ProjectionMatrix; }
	float GetCameraToLength(D3DXVECTOR3 pos);

};

#endif