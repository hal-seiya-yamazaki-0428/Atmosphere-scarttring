
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "input.h"

#include "mouse.h"

//===========================================
//�O���[�o���ϐ�
//===========================================
POINT CMouse::m_MousePos;

//===========================================
//������
//===========================================
void CMouse::Init()
{
	m_MousePos.x = m_MousePos.y = 0.0f;
}

//===========================================
//�I��
//===========================================
void CMouse::Uninit()
{

}

//===========================================
//�X�V
//===========================================
void CMouse::Update()
{
	//�}�E�X���W���N���C�A���g���W�ɕϊ�
	GetCursorPos(&m_MousePos);
	ScreenToClient(GetWindow(), &m_MousePos);

	//��ʊO���W����
	m_MousePos.x = std::max((LONG)0.0f, m_MousePos.x);			//��ʍ�����
	m_MousePos.x = std::min((LONG)SCREEN_WIDTH, m_MousePos.x);	//��ʉE����
	m_MousePos.y = std::max((LONG)0.0f, m_MousePos.y);			//��ʏ㐧��
	m_MousePos.y = std::min((LONG)SCREEN_HEIGHT, m_MousePos.y);	//��ʉ�����

}

//===========================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//===========================================
void CMouse::GetScreenToWorld(D3DXVECTOR3* pout, int Sx, int Sy, float fZ)
{
	//�J�����̍s��擾
	D3DXMATRIX View, Prj;
	CCamera *camera = CManager::GetScene()->GetGameObject<CCamera>(Layer_Camera);
	View = camera->GetViewMtx();
	Prj = camera->GetProjectionMtx();
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, &View);
	D3DXMatrixInverse(&InvPrj, NULL, &Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = SCREEN_WIDTH / 2.0f; VP._22 = -SCREEN_HEIGHT / 2.0f;
	VP._41 = SCREEN_WIDTH / 2.0f; VP._42 = SCREEN_HEIGHT / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);
}

//===========================================
// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
//===========================================
void CMouse::GetScreenToXZ(D3DXVECTOR3* pout, int Sx, int Sy)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	GetScreenToWorld(&nearpos, Sx, Sy, 0.0f);
	GetScreenToWorld(&farpos, Sx, Sy, 1.0f);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	// ���Ƃ̌������N���Ă���ꍇ�͌�_��
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (ray.y <= 0) {
		// ����_
		float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
		float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
		*pout = nearpos + (LP0 / Lray)*ray;
	}
	else {
		*pout = farpos;
	}
}

//===========================================
//���������ɓ������Ă��邩�ǂ����@
//===========================================
bool CMouse::GetLineHit(D3DXVECTOR3 center, float radius, int Sx, int Sy)
{
	D3DXVECTOR3 start;
	D3DXVECTOR3 end;
	GetScreenToWorld(&start, Sx, Sy, 0.0f);
	GetScreenToWorld(&end, Sx, Sy, 1.0f);

	//�x�N�g���̗p��
	D3DXVECTOR3 start_center = center - start;	//�n�_����~
	D3DXVECTOR3 end_center = center - end;		//�I�_����~
	D3DXVECTOR3 start_end = end - start;		//�n�_����I�_
	D3DXVECTOR3 start_end_normal;				//���K���p
	D3DXVec3Normalize(&start_end_normal, &start_end);

	D3DXVECTOR3 cross;
	D3DXVec3LengthSq(D3DXVec3Cross(&cross, &start_center, &start_end_normal));
	float length = D3DXVec3LengthSq(&cross);
	
	//���������a���Z���ꍇ
	if (fabsf(length) <= radius)
	{
		float dot_len = D3DXVec3Dot(&start_center, &start_end);
		float dot_len2 = D3DXVec3Dot(&end_center, &start_end);

		//���ς̊|���Z����
		if (dot_len * dot_len2 <= 0.0f)
		{
			return true;
		}
		//�n�_���I�_�����a���Z����
		else if (D3DXVec3LengthSq(&start_center) < radius || D3DXVec3LengthSq(&end_center) < radius)
		{
			return true;
		}
	}
	return false;
}