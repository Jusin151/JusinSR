#pragma once

/* ������ Ŭ���̾�Ʈ ���� ��ũ�� ��Ȱ�� �����Ѵ�. */
/* ���� ���� �����ϴ� ������ �̱��� Ŭ�����̴�. */
/* ���� �����ڰ� Ŭ�󰳓T�ڿ��� �����ְ���� �Լ��� ... */
#include "Base.h"

BEGIN(Engine)


class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc);
	HRESULT Draw();	

private:
	class CGraphic_Device* m_pGraphic_Device = { nullptr };
	
	/*int		iArray[5]; ��

	for (size_t i = 0; i < 5; i++)
	{
		iArray[5] = i;

	}*/


public:
	virtual void Free() override;
};

END