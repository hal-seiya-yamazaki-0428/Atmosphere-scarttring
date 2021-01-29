#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "myimGui.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"

#define PLAYER_RANGE_OUT (3.0f)
#define SPEED (0.3f)

static D3DXVECTOR2 g_Reverse(1.0f, 1.0f);
static float g_MouseSens = 2.0f;
//static bool g_hit = false;

//===========================================
//������
//===========================================
void CCamera::Init()
{
	m_Name = "camera";

	m_Position = D3DXVECTOR3(0.0f, 1.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_vecFront = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_Angle = 0.0f;
	m_Velocity = 0.0f;
	m_Mouse.velocity.x = m_Mouse.velocity.y = 0.0f;
	m_Mouse.velocity_release.x = m_Mouse.velocity_release.y = 0.0f;
	m_Mouse.pos.x = m_Mouse.pos.y = 0.0f;
	m_Mouse.pos_click.x = m_Mouse.pos_click.y = 0.0f;

	g_Reverse = D3DXVECTOR2(1.0f, 1.0f);
	g_MouseSens = 2.0f;
}

//===========================================
//�I��
//===========================================
void CCamera::Uninit()
{

}

//===========================================
//�X�V
//===========================================
void CCamera::Update()
{
	//�E�N���b�N(���_�ړ�)
	if (CInput::GetKeyPress(VK_RBUTTON))
	{
		m_Mouse.pos_click = CMouse::GetMousePos();	//�N���b�N���̃}�E�X���W�擾
		//�}�E�X�̈ړ���
		m_Mouse.velocity.x = (m_Mouse.pos.x - m_Mouse.pos_click.x) + m_Mouse.velocity_release.x;
		m_Mouse.velocity.y = (m_Mouse.pos.y - m_Mouse.pos_click.y) + m_Mouse.velocity_release.y;

		//x����]�̏���ݒ�
		m_Mouse.velocity.y = std::min((LONG)SCREEN_HEIGHT / 2, m_Mouse.velocity.y);

		//�����_�̉�]�p�ϐ�
		m_Rotation.x = (float)m_Mouse.velocity.y / (float)SCREEN_HEIGHT * g_MouseSens * g_Reverse.y;
		m_Rotation.y = (float)m_Mouse.velocity.x / (float)SCREEN_WIDTH * g_MouseSens * g_Reverse.x;

		//�����x�N�g���̉�]
		m_vecFront = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		m_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		D3DXMATRIX rot;
		D3DXMatrixIdentity(&rot);
		D3DXMatrixRotationYawPitchRoll(&rot, -m_Rotation.y, -m_Rotation.x, m_Rotation.z);

		D3DXVec3TransformCoord(&m_vecFront, &m_vecFront, &rot);
		D3DXVec3TransformCoord(&m_vecRight, &m_vecRight, &rot);

	}
	else
	{
		//�}�E�X���W�ƁA�N���b�N�����[�X���̃}�E�X���x�ݒ�
		(CInput::GetKeyRelease(VK_RBUTTON)) ? m_Mouse.velocity_release = m_Mouse.velocity : m_Mouse.pos = CMouse::GetMousePos();
	}

	//�O�i
	if (CInput::GetKeyPress('W'))
	{
		m_vecDir += m_vecFront;
		m_Velocity = SPEED;
	}
	//���
	else if (CInput::GetKeyPress('S'))
	{
		m_vecDir -= m_vecFront;
		m_Velocity = SPEED;
	}
	//�E�ړ�
	if (CInput::GetKeyPress('D'))
	{
		m_vecDir += m_vecRight;
		m_Velocity = SPEED;
	}
	//���ړ�
	else if (CInput::GetKeyPress('A'))
	{
		m_vecDir -= m_vecRight;
		m_Velocity = SPEED;
	}

	//�㏸
	if (CInput::GetKeyPress(VK_SPACE))
	{
		m_vecDir += m_vecUp;
		m_Velocity = SPEED;
	}
	//���~
	else if (CInput::GetKeyPress(VK_LSHIFT))
	{
		m_vecDir -= m_vecUp;
		m_Velocity = SPEED;
	}

	//�i�s�������K��
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);

	m_Position += m_vecDir * m_Velocity;
	m_Velocity *= 0.9f;

	//�����_�ݒ�
	m_Target.x = m_Position.x + 5.0f * -sin(m_Rotation.y);
	m_Target.y = m_Position.y + 5.0f * sin(m_Rotation.x);
	m_Target.z = m_Position.z + 5.0f * cos(m_Rotation.y);
}

//===========================================
//�`��
//===========================================
void CCamera::Draw()
{
	//�r���[�}�g���N�X�ݒ�
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	CRenderer::SetViewMatrix(&m_ViewMatrix);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);

	CRenderer::SetCameraPosition(m_Position);


	//GUI
	{
		////3�����x�N�g��
		//std::string str = "front.x = " + std::to_string(m_vecFront.x) + "  front.y = " + std::to_string(m_vecFront.y) + "  front.z = " + std::to_string(m_vecFront.z);
		//ImGui::Text(str.c_str());

		//std::string str4 = "right.x = " + std::to_string(m_vecRight.x) + "  right.y = " + std::to_string(m_vecRight.y) + "  right.z = " + std::to_string(m_vecRight.z);
		//ImGui::Text(str4.c_str());

		//std::string str5 = "dir.x = " + std::to_string(m_vecDir.x) + "  dir.y = " + std::to_string(m_vecDir.y) + "  dir.z = " + std::to_string(m_vecDir.z);
		//ImGui::Text(str5.c_str());

		////�����蔻��
		////ImGui::Text(std::to_string(g_hit).c_str());

		//ImGui::Text(u8"�e�X�g���j���[");

		////���W
		//std::string str2 = "rotation.x = " + std::to_string(m_Rotation.x) + "  rotation.y = " + std::to_string(m_Rotation.y);
		//ImGui::Text(str2.c_str());

		////���x
		//std::string str3 = "velocity.x = " + std::to_string(m_Mouse.velocity.x) + "  velocity.y = " + std::to_string(m_Mouse.velocity.y);
		//ImGui::Text(str3.c_str());

		//std::string str4 = "width = " + std::to_string((float)m_Mouse.velocity.x / (float)SCREEN_WIDTH) + "  height = " + std::to_string(SCREEN_HEIGHT);
		//ImGui::Text(str4.c_str());
	}

}

//===========================================
//�t�s��̎擾
//===========================================
D3DXMATRIX CCamera::GetInv()
{
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &m_ViewMatrix);
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	return invView;
}

//===========================================
//�J�����ƃI�u�W�F�N�g�̋���
//===========================================
float CCamera::GetCameraToLength(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 length_vec = pos - m_Position;
	return D3DXVec3Length(&length_vec);
}