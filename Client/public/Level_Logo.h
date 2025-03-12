#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
private:
	CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	HRESULT Ready_Layer_BackGround();
	HRESULT Ready_Layer_Menu();

private:


public:
	static CLevel_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END