
#include "main.h"
#include "renderer.h"
#include "manager.h"

#include "shader.h"

//===========================================
//�O���[�o���ϐ�
//===========================================
ID3D11ShaderResourceView *CShader::m_ToonMap = NULL;

//===========================================
//������
//===========================================
CShader::CShader(const char *vs_file, const char *ps_file)
{
	CRenderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, vs_file);
	CRenderer::CreatePixelShader(&m_PixelShader, ps_file);
}

//===========================================
//�I��
//===========================================
CShader::~CShader()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

//===========================================
//�`�掞�ݒ�
//===========================================
void CShader::ShaderDraw(bool toon_texture)
{
	//�g�D�[���}�b�v�̐ݒ�
	if (toon_texture)
	{
		CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_ToonMap);
	}
	//���̓��C�A�E�g
	CRenderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	//�V�F�[�_�[�ݒ�
	CRenderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	CRenderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
}

//===========================================
//�g�D�[���}�b�v����
//===========================================
void CShader::ToonTextureLoad()
{
	//�g�D�[���e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/toon_map.png",
		NULL,
		NULL,
		&m_ToonMap,
		NULL);

	assert(m_ToonMap);
}

//===========================================
//�g�D�[���}�b�v���
//===========================================
void CShader::ToonTextureUnload()
{
	m_ToonMap->Release();
}