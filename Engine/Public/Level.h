#pragma once

#include "Base.h"

/* Ŭ�� �����ڰ� ���� ����Ŭ�������� �θ� �Ǵ� Ŭ������. */

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel() = default;

public:
	HRESULT Initialize();
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	HRESULT Render();

protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = { nullptr };

public:	
	virtual void Free() override;

};

END