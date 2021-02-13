#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "shader.h"
#include "model.h"
#include "myimGui.h"
#include "scene.h"

#include "camera.h"
#include "sky_dome.h"
#include "light.h"

#define SIZE (250.0f)//125

void CSkyDome::Init()
{
	m_Name = u8"skydome";

	m_Shader = new CShader("skyVS.cso", "skyPS.cso");

	m_Model = new CModel();
	m_Model->Load("asset\\model\\atomosphere.obj");
	//m_Model->Load("asset\\model\\half_sky_dome.obj");

	m_Position = D3DXVECTOR3(0.0f, -2.5f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(SIZE, SIZE, SIZE);
}

void CSkyDome::Uninit()
{
	delete m_Shader;

	m_Model->Unload();
	delete m_Model;
}

void CSkyDome::Update()
{

}

void CSkyDome::Draw()
{
	//CLight::SetLightOn(false);

	m_Shader->ShaderDraw();

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	CRenderer::SetWorldMatrix(&world);

	m_Model->Draw();

	//CLight::SetLightOn(true);

}