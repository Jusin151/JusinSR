﻿#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Sphere final : public CVIBuffer
{
private:
	CVIBuffer_Sphere(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Sphere(const CVIBuffer_Sphere& Prototype);
	virtual ~CVIBuffer_Sphere() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

private:
	_float	m_fRadius = {};	// 구의 반지름
	_int	m_iSlices = {};	// 경도 방향 분할 수 (수평)
	_int	m_iStacks = {};	// 위도 방향 분할 수 (수직)

public:
	static CVIBuffer_Sphere* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END