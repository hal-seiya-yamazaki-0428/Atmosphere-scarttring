
#include "main.h"
#include "renderer.h"
#include "light.h"

LIGHT CLight::m_Light;

void CLight::SetDefaultLight()
{
	//ÉâÉCÉgê›íË
	m_Light.Enable = true;
	m_Light.Direction = D3DXVECTOR4(0.0f, -1.0f, 1.0f, 1.0f);
	D3DXVec4Normalize(&m_Light.Direction, &m_Light.Direction);
	m_Light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetLight(m_Light);
}

void CLight::SetLightColor(D3DXCOLOR *ambient, D3DXCOLOR *diffuse)
{
	m_Light.Enable = true;
	m_Light.Direction = m_Light.Direction;
	D3DXVec4Normalize(&m_Light.Direction, &m_Light.Direction);
	m_Light.Ambient = *ambient;
	m_Light.Diffuse = *diffuse;
	CRenderer::SetLight(m_Light);
}

void CLight::SetLightOn(bool on)
{
	m_Light.Enable = on;
	CRenderer::SetLight(m_Light);
}