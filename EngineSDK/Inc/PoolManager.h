#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
class CPool_Manager final : public CBase
{
private:
	CPool_Manager();
	virtual ~CPool_Manager() = default;

public:
	class CGameObject* Acquire_Object(_uint iPrototypeLevelIndex, const _wstring& strLayerTag);

	// 풀에 객체 반환하기
	HRESULT Return_Object(_uint iPrototypeLevelIndex, const _wstring& strLayerTag, CGameObject* pGameObject);

	// 어디 레벨에 있는지, 어떤 이름으로 있었는지, 나중에 불러올 때 사용할 태그, 몇개 만들지, 던져줘야할 인자
	HRESULT Reserve_Pool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strLayerTag,_uint iCount,void* pArg = nullptr);
	void Clear(_uint iLevelIndex);
private:
	class CGameInstance* m_pGameInstance = { nullptr }; // 프로토타입에서 가져오기 위함
	unordered_map<_uint,map<_wstring, queue<CGameObject*>>> m_Pools; // 레벨, 해당 레벨의 tag를 가진 오브젝트들
	unordered_map<_wstring, pair<_wstring,void*>> m_InitArgMap;  // 레이어 태그 -> (프로토타입 태그, 초기화 인자)

public:
	static CPool_Manager* Create();
	virtual void Free() override;
};

END