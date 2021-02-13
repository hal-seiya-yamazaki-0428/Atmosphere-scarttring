
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "shadow.h"

ID3D11RenderTargetView* CShadow::m_RenderTarget = NULL;
ID3D11ShaderResourceView* CShadow::m_ShaderResourceView = NULL;

//===========================================
//������
//===========================================
void CShadow::Init()
{
	{
		// 2�����e�N�X�`���̐ݒ�
		ID3D11Texture2D *TexCoord;
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = SCREEN_WIDTH;
		td.Height = SCREEN_HEIGHT;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R16G16B16A16_TYPELESS;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		memset(&rtvDesc, 0, sizeof(rtvDesc));
		rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		// �V�F�[�_���\�[�X�r���[�̐ݒ�
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		memset(&srvDesc, 0, sizeof(srvDesc));
		srvDesc.Format = rtvDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		CRenderer::GetDevice()->CreateTexture2D(&td, NULL, &TexCoord);
		CRenderer::GetDevice()->CreateRenderTargetView(TexCoord, &rtvDesc, &m_RenderTarget);
		CRenderer::GetDevice()->CreateShaderResourceView(TexCoord, &srvDesc, &m_ShaderResourceView);
		TexCoord->Release();
	}

}

//===========================================
//������
//===========================================
void CShadow::Uninit()
{
	m_RenderTarget->Release();
	m_ShaderResourceView->Release();
}

//===========================================
//�X�V
//===========================================
void CShadow::Update()
{

}

//===========================================
//�`��
//===========================================
void CShadow::Create()
{
	auto depth = CRenderer::GetDepthStencilView();

	CRenderer::GetDeviceContext()->OMSetRenderTargets(1, &m_RenderTarget, depth);

	// �o�b�N�o�b�t�@�N���A
	float ClearColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
	CRenderer::GetDeviceContext()->ClearRenderTargetView(m_RenderTarget, ClearColor);
	CRenderer::GetDeviceContext()->ClearDepthStencilView(depth, D3D11_CLEAR_DEPTH, 1.0f, 0);
}