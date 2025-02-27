#pragma once

#include "Base.h"

/* 1. 원형객체를 보관한다. */
/* 1_1. 원형객체의 타입( CGameObject, CComponent )에 크게 영향을 받지 않는다. */
/* 1_2. 보관할때 레벨별( + 모든레벨) 로 구분하여 저장할거야. */

BEGIN(Engine)

class CPrototype_Manager final : public CBase
{
private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);
private:
	_uint										m_iNumLevels = {};
	vector<map<const _wstring, class CBase*>>			m_vecPrototypes; // 레벨별로 원형을 보관할 벡터
	typedef map<const _wstring, class CBase*>	PROTOTYPES;

private:
	class CBase* Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag);

public:
	static CPrototype_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END