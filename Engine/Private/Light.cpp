#include "Light.h"

CLight::CLight(PDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent { pGraphic_Device }
{
}

CLight::CLight(const CLight& Prototype)
	:	CComponent{ Prototype },
		m_tInfo{ Prototype.m_tInfo }
{
}

HRESULT CLight::Initialize_Prototype(LIGHTTYPE eType)
{
	m_tInfo.eType			= eType; // �� Ÿ���� �̹� �����Ǿ� �ִٰ� ����
	m_tInfo.fDiffuse		= { 1.0f, 1.0f, 1.0f, 1.0f }; // ��� Ȯ�걤
	m_tInfo.fAmbient		= { 0.2f, 0.2f, 0.2f, 1.0f }; // ���� �ֺ���
	m_tInfo.fSpecular		= { 1.0f, 1.0f, 1.0f, 1.0f }; // ��� �ݻ籤
	m_tInfo.fPosition		= { 0.0f, 10.0f, 0.0f }; // ���� ��ġ (������ �Ʒ��� ���ߴ� ������ ����)
	m_tInfo.fDirection		= { 0.0f, -1.0f, 0.0f }; // ���� ���� (�Ʒ���)
	m_tInfo.fAttenuation0	= 1.0f; // ���� ���
	m_tInfo.fAttenuation1	= 0.0f; // ���� ����
	m_tInfo.fAttenuation2	= 0.0f; // ���� 2��
	m_tInfo.fFalloff		= 1.0f; // ����Ʈ����Ʈ ����
	m_tInfo.fRange			= 100.0f; // ���� ����
	m_tInfo.fTheta			= D3DX_PI / 4; // ����Ʈ����Ʈ ���� ���� (45��)
	m_tInfo.fPhi			= D3DX_PI / 2; // ����Ʈ����Ʈ �ܺ� ���� (90��)
	return S_OK;
}

HRESULT CLight::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		LIGHT_DESC in = *reinterpret_cast<LIGHT_DESC*>(pArg);
		memcpy(&m_tInfo, &in, sizeof(LIGHT_DESC));
	}
	return S_OK;
}

HRESULT CLight::Bind_Resouce(_uint iIndex)
{
	D3DLIGHT9 light = {};
	light.Type = (D3DLIGHTTYPE)m_tInfo.eType;
	memcpy(&light.Diffuse,		&m_tInfo.fDiffuse,		sizeof(_float4));
	memcpy(&light.Ambient,		&m_tInfo.fAmbient,		sizeof(_float4));
	memcpy(&light.Specular,		&m_tInfo.fSpecular,		sizeof(_float4));
	memcpy(&light.Position,		&m_tInfo.fPosition,		sizeof(_float3));
	memcpy(&light.Direction,	&m_tInfo.fDirection,	sizeof(_float3));
	memcpy(&light.Attenuation0, &m_tInfo.fAttenuation0,	sizeof(_float));
	memcpy(&light.Attenuation1, &m_tInfo.fAttenuation1,	sizeof(_float));
	memcpy(&light.Attenuation2, &m_tInfo.fAttenuation2,	sizeof(_float));
	memcpy(&light.Falloff,		&m_tInfo.fFalloff,		sizeof(_float));
	memcpy(&light.Range,		&m_tInfo.fRange,		sizeof(_float));
	memcpy(&light.Theta,		&m_tInfo.fTheta,		sizeof(_float));
	memcpy(&light.Phi,			&m_tInfo.fPhi,			sizeof(_float));

	m_pGraphic_Device->SetLight(iIndex, &light);

	return S_OK;
}

CLight* CLight::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LIGHTTYPE eType)
{
	CLight* pInstance = new CLight(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(eType)))
	{
		MSG_BOX("Failed to Created : CLight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CLight* CLight::Clone(void* pArg)
{
	CLight* pInstance = new CLight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CLight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	__super::Free();
}
