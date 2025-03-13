#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer { pGraphic_Device }
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer& Prototype)
	: CVIBuffer{ Prototype }
{
}

HRESULT CVIBuffer_Cube::Initialize_Prototype()
{

	m_iNumVertices = 8;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1/* | D3DFVF_TEXCOORDSIZE2(0)*/;
	m_iNumPritimive = 12;

	m_iIndexStride = 2;
	m_iNumIndices = 36;
	m_eIndexFormat = D3DFMT_INDEX16; 

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };


	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	// ��
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);		

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);
	
	// �޸� (����: (0, 0, 1))
	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 1.f);
	pVertices[4].vTexcoord = _float2(0.f, 0.f);

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 1.f);
	pVertices[5].vTexcoord = _float2(1.f, 0.f);

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 1.f);
	pVertices[6].vTexcoord = _float2(1.f, 1.f);

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 1.f);
	pVertices[7].vTexcoord = _float2(0.f, 1.f);


	m_pVB->Unlock();
#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	_uint iIndex = 0;
	// ��
	pIndices[iIndex++] = 0;
	pIndices[iIndex++] = 1;
	pIndices[iIndex++] = 2;

	pIndices[iIndex++] = 0;
	pIndices[iIndex++] = 2;
	pIndices[iIndex++] = 3;

	// ������
	pIndices[iIndex++] = 1;
	pIndices[iIndex++] = 5;
	pIndices[iIndex++] = 6;
			 
	pIndices[iIndex++] = 1;
	pIndices[iIndex++] = 6;
	pIndices[iIndex++] = 2;

	// ����
	pIndices[iIndex++] = 4;
	pIndices[iIndex++] = 0;
	pIndices[iIndex++] = 3;

	pIndices[iIndex++] = 4;
	pIndices[iIndex++] = 3;
	pIndices[iIndex++] = 7;

	// ��
	pIndices[iIndex++] = 5;
	pIndices[iIndex++] = 4;
	pIndices[iIndex++] = 7;

	pIndices[iIndex++] = 5;
	pIndices[iIndex++] = 7;
	pIndices[iIndex++] = 6;

	// �Ʒ�
	pIndices[iIndex++] = 3;
	pIndices[iIndex++] = 2;
	pIndices[iIndex++] = 6;

	pIndices[iIndex++] = 3;
	pIndices[iIndex++] = 6;
	pIndices[iIndex++] = 7;

	// ��
	pIndices[iIndex++] = 4;
	pIndices[iIndex++] = 5;
	pIndices[iIndex++] = 1;

	pIndices[iIndex++] = 4;
	pIndices[iIndex++] = 1;
	pIndices[iIndex++] = 0;

	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}
//HRESULT CVIBuffer_Cube::Initialize_Prototype()
//{
//    // ť���� �� ���� 4���� ���ؽ��� �ʿ��ϹǷ� (6�� x 4���ؽ� = 24)
//    m_iNumVertices = 24;
//    m_iVertexStride = sizeof(VTXPOSTEX); 
//    m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
//    m_iNumPritimive = 12;  // 12���� �ﰢ�� (6�� x 2�ﰢ��)
//    m_iIndexStride = 2;
//    m_iNumIndices = 36;    // 12���� �ﰢ�� x 3���� �ε���
//    m_eIndexFormat = D3DFMT_INDEX16;
//
//#pragma region VERTEX_BUFFER
//    if (FAILED(__super::Create_VertexBuffer()))
//        return E_FAIL;
//
//    VTXPOSTEX* pVertices = { nullptr };
//    m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
//
//    // �ո� (�븻: 0, 0, -1) - ���� 0-3
//    pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);   // �»��
//.
//    pVertices[0].vTexcoord = _float2(0.f, 0.f);
//
//    pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);    // ����
//    pVertices[1].vTexcoord = _float2(1.f, 0.f);
//
//    pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);   // ���ϴ�
//    pVertices[2].vTexcoord = _float2(1.f, 1.f);
//
//    pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);  // ���ϴ�
//    pVertices[3].vTexcoord = _float2(0.f, 1.f);
//
//    // �޸� (�븻: 0, 0, 1) - ���� 4-7
//    pVertices[4].vPosition = _float3(0.5f, 0.5f, 0.5f);    // ����
//    pVertices[4].vTexcoord = _float2(0.f, 0.f);
//
//    pVertices[5].vPosition = _float3(-0.5f, 0.5f, 0.5f);   // �»��
//    pVertices[5].vNormal = _float3(0.0f, 0.0f, 1.0f);
//    pVertices[5].vTexcoord = _float2(1.f, 0.f);
//
//    pVertices[6].vPosition = _float3(-0.5f, -0.5f, 0.5f);  // ���ϴ�
//    pVertices[6].vNormal = _float3(0.0f, 0.0f, 1.0f);
//    pVertices[6].vTexcoord = _float2(1.f, 1.f);
//
//    pVertices[7].vPosition = _float3(0.5f, -0.5f, 0.5f);   // ���ϴ�
//    pVertices[7].vNormal = _float3(0.0f, 0.0f, 1.0f);
//    pVertices[7].vTexcoord = _float2(0.f, 1.f);
//
//    // ������ �� (�븻: 1, 0, 0) - ���� 8-11
//    pVertices[8].vPosition = _float3(0.5f, 0.5f, -0.5f);    // �»��
//    pVertices[8].vNormal = _float3(1.0f, 0.0f, 0.0f);
//    pVertices[8].vTexcoord = _float2(0.f, 0.f);
//
//    pVertices[9].vPosition = _float3(0.5f, 0.5f, 0.5f);    // ����
//    pVertices[9].vNormal = _float3(1.0f, 0.0f, 0.0f);
//    pVertices[9].vTexcoord = _float2(1.f, 0.f);
//
//    pVertices[10].vPosition = _float3(0.5f, -0.5f, 0.5f);  // ���ϴ�
//    pVertices[10].vNormal = _float3(1.0f, 0.0f, 0.0f);
//    pVertices[10].vTexcoord = _float2(1.f, 1.f);
//
//    pVertices[11].vPosition = _float3(0.5f, -0.5f, -0.5f);  // ���ϴ�
//    pVertices[11].vNormal = _float3(1.0f, 0.0f, 0.0f);
//    pVertices[11].vTexcoord = _float2(0.f, 1.f);
//
//    // ���� �� (�븻: -1, 0, 0) - ���� 12-15
//    pVertices[12].vPosition = _float3(-0.5f, 0.5f, 0.5f);  // �»��
//    pVertices[12].vNormal = _float3(-1.0f, 0.0f, 0.0f);
//    pVertices[12].vTexcoord = _float2(0.f, 0.f);
//
//    pVertices[13].vPosition = _float3(-0.5f, 0.5f, -0.5f);  // ����
//    pVertices[13].vNormal = _float3(-1.0f, 0.0f, 0.0f);
//    pVertices[13].vTexcoord = _float2(1.f, 0.f);
//
//    pVertices[14].vPosition = _float3(-0.5f, -0.5f, -0.5f); // ���ϴ�
//    pVertices[14].vNormal = _float3(-1.0f, 0.0f, 0.0f);
//    pVertices[14].vTexcoord = _float2(1.f, 1.f);
//
//    pVertices[15].vPosition = _float3(-0.5f, -0.5f, 0.5f); // ���ϴ�
//    pVertices[15].vNormal = _float3(-1.0f, 0.0f, 0.0f);
//    pVertices[15].vTexcoord = _float2(0.f, 1.f);
//
//    // ���� �� (�븻: 0, 1, 0) - ���� 16-19
//    pVertices[16].vPosition = _float3(-0.5f, 0.5f, 0.5f);  // �»��
//    pVertices[16].vNormal = _float3(0.0f, 1.0f, 0.0f);
//    pVertices[16].vTexcoord = _float2(0.f, 0.f);
//
//    pVertices[17].vPosition = _float3(0.5f, 0.5f, 0.5f);   // ����
//    pVertices[17].vNormal = _float3(0.0f, 1.0f, 0.0f);
//    pVertices[17].vTexcoord = _float2(1.f, 0.f);
//
//    pVertices[18].vPosition = _float3(0.5f, 0.5f, -0.5f);   // ���ϴ�
//    pVertices[18].vNormal = _float3(0.0f, 1.0f, 0.0f);
//    pVertices[18].vTexcoord = _float2(1.f, 1.f);
//
//    pVertices[19].vPosition = _float3(-0.5f, 0.5f, -0.5f);  // ���ϴ�
//    pVertices[19].vNormal = _float3(0.0f, 1.0f, 0.0f);
//    pVertices[19].vTexcoord = _float2(0.f, 1.f);
//
//    // �Ʒ��� �� (�븻: 0, -1, 0) - ���� 20-23
//    pVertices[20].vPosition = _float3(-0.5f, -0.5f, -0.5f); // �»��
//    pVertices[20].vNormal = _float3(0.0f, -1.0f, 0.0f);
//    pVertices[20].vTexcoord = _float2(0.f, 0.f);
//
//    pVertices[21].vPosition = _float3(0.5f, -0.5f, -0.5f);  // ����
//    pVertices[21].vNormal = _float3(0.0f, -1.0f, 0.0f);
//    pVertices[21].vTexcoord = _float2(1.f, 0.f);
//
//    pVertices[22].vPosition = _float3(0.5f, -0.5f, 0.5f);  // ���ϴ�
//    pVertices[22].vNormal = _float3(0.0f, -1.0f, 0.0f);
//    pVertices[22].vTexcoord = _float2(1.f, 1.f);
//
//    pVertices[23].vPosition = _float3(-0.5f, -0.5f, 0.5f); // ���ϴ�
//    pVertices[23].vNormal = _float3(0.0f, -1.0f, 0.0f);
//    pVertices[23].vTexcoord = _float2(0.f, 1.f);
//
//    m_pVB->Unlock();
//#pragma endregion
//
//#pragma region INDEX_BUFFER
//    if (FAILED(__super::Create_IndexBuffer()))
//        return E_FAIL;
//
//    _ushort* pIndices = { nullptr };
//    m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);
//    _uint iIndex = 0;
//
//    // �ո� (0-3)
//    pIndices[iIndex++] = 0;
//    pIndices[iIndex++] = 1;
//    pIndices[iIndex++] = 2;
//
//    pIndices[iIndex++] = 0;
//    pIndices[iIndex++] = 2;
//    pIndices[iIndex++] = 3;
//
//    // �޸� (4-7)
//    pIndices[iIndex++] = 4;
//    pIndices[iIndex++] = 5;
//    pIndices[iIndex++] = 6;
//
//    pIndices[iIndex++] = 4;
//    pIndices[iIndex++] = 6;
//    pIndices[iIndex++] = 7;
//
//    // ������ �� (8-11)
//    pIndices[iIndex++] = 8;
//    pIndices[iIndex++] = 9;
//    pIndices[iIndex++] = 10;
//
//    pIndices[iIndex++] = 8;
//    pIndices[iIndex++] = 10;
//    pIndices[iIndex++] = 11;
//
//    // ���� �� (12-15)
//    pIndices[iIndex++] = 12;
//    pIndices[iIndex++] = 13;
//    pIndices[iIndex++] = 14;
//
//    pIndices[iIndex++] = 12;
//    pIndices[iIndex++] = 14;
//    pIndices[iIndex++] = 15;
//
//    // ���� �� (16-19)
//    pIndices[iIndex++] = 16;
//    pIndices[iIndex++] = 17;
//    pIndices[iIndex++] = 18;
//
//    pIndices[iIndex++] = 16;
//    pIndices[iIndex++] = 18;
//    pIndices[iIndex++] = 19;
//
//    // �Ʒ��� �� (20-23)
//    pIndices[iIndex++] = 20;
//    pIndices[iIndex++] = 21;
//    pIndices[iIndex++] = 22;
//
//    pIndices[iIndex++] = 20;
//    pIndices[iIndex++] = 22;
//    pIndices[iIndex++] = 23;
//
//    m_pIB->Unlock();
//#pragma endregion
//
//    return S_OK;
//}
HRESULT CVIBuffer_Cube::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Cube::Clone(void* pArg)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();


}
