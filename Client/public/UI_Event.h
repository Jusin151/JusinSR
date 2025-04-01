#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "UI_Base.h"
#include "Observer.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;  // UI�� �簢������
END

BEGIN(Client)


class CUI_Event final : public CUI_Base, public CObserver
{
public:
	struct EventText
	{
		wstring wstr;
		_float ftime;
	};

	vector<EventText> m_vecEventTexts;

public:
	enum HP_STATE { Default, Heated };
private:
	CUI_Event(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUI_Event(const CUI_Event& Prototype);
	virtual ~CUI_Event() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	static CUI_Event* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
public:

	void ShowEventText(_int value, const wstring& tag)
	{
		wstring text;

		if (tag == L"Exp")
			text = L"����ġ ȹ��!  +" + to_wstring(value);
		else if (tag == L"Hp")
			text = L"+HP " + to_wstring(value);
		else if (tag == L"Ammo")
			text = L"����� �ణ�� ź���� ������ϴ�! +" + to_wstring(value);
		else if (tag == L"LevelUp")
			text = L"������!";

		m_vecEventTexts.push_back({ text, 1.5f }); // 1.5�ʰ� ����
	}



private:
	_uint m_iType{};
	wstring m_strText = L"";
	_float m_fTextDuration = { 0.f }; // �� �� ���Ҵ���



	// CObserver��(��) ���� ��ӵ�
	void OnNotify(void* pArg, const wstring& type) override;

};
END


