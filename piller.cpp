
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"
#include "model.h"
#include "piller.h"

void CPiller::Init()
{
	m_Model = new CModel();
	m_Model->Load("asset\\model\\cube.obj");

	m_ShadowShader = new CShader("shadowRenderVS.cso", "shadowRenderPS.cso");
	m_DefaultShader = new CShader("shadowVS.cso", "shadowPS.cso");

	m_Position = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 10.0f, 1.0f);
}

void CPiller::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	delete m_ShadowShader;
	delete m_DefaultShader;
}

void CPiller::Update()
{
	//m_Rotation.y += 0.05f;
	//m_Rotation.x += 0.05f;
}

void CPiller::Draw()
{
	// モデルマトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	CRenderer::SetWorldMatrix(&world);

	ID3D11ShaderResourceView* shadowDepthTexture = CRenderer::GetSRV(1);
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);

	m_Model->Draw();
}

void CPiller::ShadowShaderDraw()
{
	m_ShadowShader->ShaderDraw();
}

void CPiller::DefaultShaderDraw()
{
	m_DefaultShader->ShaderDraw();
}