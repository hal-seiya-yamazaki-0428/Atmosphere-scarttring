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
	D3DXVECTOR3 m_Target;			//�����_
	D3DXMATRIX m_ViewMatrix;		//�r���[�s��
	D3DXMATRIX m_ProjectionMatrix;	//�v���W�F�N�V�����s��
	D3DXVECTOR3 m_vecDir;


public:
	CCamera() {}
	~CCamera() {}

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	//�Q�b�g�֐�
	D3DXMATRIX GetInv();
	D3DXMATRIX GetViewMtx() { return m_ViewMatrix; }
	D3DXMATRIX GetProjectionMtx() { return m_ProjectionMatrix; }
	float GetCameraToLength(D3DXVECTOR3 pos);

};

#endif