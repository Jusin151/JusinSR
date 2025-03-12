#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer { pGraphic_Device }
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype)
	: CVIBuffer{ Prototype }
	, m_iNumVerticesX { Prototype.m_iNumVerticesX }
	, m_iNumVerticesZ { Prototype.m_iNumVerticesZ }
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	//체인지레벨에 클리어 함수들 만드셈 ㅋ
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1/* | D3DFVF_TEXCOORDSIZE2(0)*/;
	m_iNumPritimive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndices = m_iNumPritimive * 3;
	m_iIndexStride = 4;
	m_eIndexFormat = D3DFMT_INDEX32;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };


	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			if (j > m_iNumVerticesX / 2)
			{
				pVertices[iIndex].vPosition = _float3(j, 10.f, i);
			} else
			{
				pVertices[iIndex].vPosition = _float3(j, 0.f, i);
			}
			
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}



	m_pVB->Unlock();
#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_uint* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	_uint		iNumIndices = { 0 };

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			_uint			iIndices[4] = {
				iIndex + m_iNumVerticesX, 
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1, 
				iIndex
			};

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];
		}
	}

	m_pIB->Unlock();

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();


}
