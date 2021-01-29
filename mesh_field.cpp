#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "shader.h"
#include "myimGui.h"

#include "mesh_field.h"

//(22 * 2) * 20 - 2
#define INDEX_NUM ((MESH_Z + 1) * 2) * (MESH_X - 1) - 2

//float g_FieldHeight[MESH_X][MESH_Z] =
//{
//	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -3.0f, -1.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -3.0f, -4.0f, -3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 0.0f, -2.0f, -2.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 5.0f, 3.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 5.0f, 5.0f, 5.0f, 3.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 5.0f, 5.0f, 6.0f, 5.0f, 5.0f, 3.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 5.0f, 5.0f, 6.0f, 7.0f, 6.0f, 5.0f, 5.0f, 3.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 1.0f, 2.0f, 3.0f, 5.0f, 5.0f, 6.0f, 7.0f, 8.0f, 7.0f, 6.0f, 5.0f, 5.0f, 3.0f, 2.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 5.0f, 5.0f, 6.0f, 7.0f, 6.0f, 5.0f, 5.0f, 3.0f, 2.0f, 1.0f, 0.0f, 2.0f, 2.0f, 2.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 5.0f, 5.0f, 6.0f, 5.0f, 5.0f, 3.0f, 2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 3.0f, 1.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 5.0f, 5.0f, 5.0f, 3.0f, 2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 3.0f, 4.0f, 3.0f, 0.0f, 0.0f },
//	{0.0f, 1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 2.0f, 3.0f, 5.0f, 3.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 3.0f, 1.0f, 0.0f, 0.0f },
//	{0.0f, 1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 3.0f, 1.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 1.0f, 2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f },
//	{0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
//};
static bool g_wire_frame = false;

//===========================================
//初期化
//===========================================
void CMeshField::Init()
{
	m_Name = "field";
	m_ShadowShader = new CShader("shadowRenderVS.cso", "shadowRenderPS.cso");
	m_DefaultShader = new CShader("shadowVS.cso", "shadowPS.cso");
	
	g_wire_frame = false;

	//頂点バッファ
	{
		//頂点座標
		for (int x = 0; x <= MESH_X - 1; x++)//<= 20
		{
			for (int z = 0; z <= MESH_Z - 1; z++)//<= 20
			{
				//float y = g_FieldHeight[z][x];
				float y = 0.0f;
				m_Vertex[x][z].Position = D3DXVECTOR3((x - (MESH_X - 1) / 2) * 5.0f, y, (z - (MESH_Z - 1) / 2) * -5.0f);
				m_Vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				m_Vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[x][z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
				//m_Vertex[x][z].TexCoord = D3DXVECTOR2(1.0f * x / MESH_X, 1.0f * z / MESH_Z);
			}
		}

		//法線ベクトル
		for (int x = 1; x <= MESH_X - 2; x++)
		{
			for (int z = 1; z <= MESH_Z - 2; z++)
			{
				D3DXVECTOR3 vx, vz, vn;

				vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;
				vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;

				D3DXVec3Cross(&vn, &vz, &vx);
				D3DXVec3Normalize(&vn, &vn);
				m_Vertex[x][z].Normal = vn;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * MESH_X * MESH_Z;	//21 * 21
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	//インデックスバッファ
	{
		unsigned int index[INDEX_NUM];//(22 * 2) * 20 - 2

		int i = 0;
		for (int x = 0; x < MESH_X - 1; x++)//20
		{
			for (int z = 0; z < MESH_Z; z++)//21
			{
				index[i] = x * MESH_Z + z;//21
				i++;

				index[i] = (x + 1) * MESH_Z + z;//21
				i++;
			}
			if (x == MESH_X - 2)//19
			{
				break;
			}
			index[i] = (x + 1) * MESH_Z + (MESH_Z - 1);//21 + 20
			i++;

			index[i] = (x + 1) * MESH_Z;//21
			i++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * INDEX_NUM;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);

	}

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/filed.jpg",//filed.jpg, pattern_tile.png
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);

	//3D情報設定
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

//===========================================
//終了
//===========================================
void CMeshField::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	m_Texture->Release();

	delete m_ShadowShader;
	delete m_DefaultShader;
}

//===========================================
//更新
//===========================================
void CMeshField::Update()
{

}

//===========================================
//描画
//===========================================
void CMeshField::Draw()
{
	//マトリクス設定
	D3DXMATRIX mtxWorld, mtxScale, mtxRot, mtxTrans;
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&mtxTrans, m_Position.x, m_Position.y, m_Position.z);
	mtxWorld = mtxScale * mtxRot * mtxTrans;
	CRenderer::SetWorldMatrix(&mtxWorld);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//インデックスバッファ設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	ID3D11ShaderResourceView* shadowDepthTexture = CRenderer::GetSRV(1);
	CRenderer::GetDeviceContext()->PSSetShaderResources(1, 1, &shadowDepthTexture);

	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	CRenderer::GetDeviceContext()->DrawIndexed(INDEX_NUM, 0, 0);

	//ワイヤーフレーム解除
	CRenderer::ChangeFillMode(false);
}

void CMeshField::ShadowShaderDraw()
{
	m_ShadowShader->ShaderDraw();
}

void CMeshField::DefaultShaderDraw()
{
	m_DefaultShader->ShaderDraw();
}

//===========================================
//高さ取得
//===========================================
float CMeshField::GetHeight(D3DXVECTOR3 Position)
{
	int x, z;

	//ブロック番号算出
	x = Position.x / 5.0f + (MESH_X - 1) / 2;
	z = Position.z / -5.0f + (MESH_Z - 1) / 2;

	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;

	D3DXVECTOR3 v12, v1p, c;

	v12 = pos2 - pos1;
	v1p = Position - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	D3DXVECTOR3 n;

	if (c.y > 0.0f)
	{
		//左上ポリゴン
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else
	{
		//右下ポリゴン
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}

	//高さ取得
	py = -((Position.x - pos1.x) * n.x + (Position.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}