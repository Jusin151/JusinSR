#include "..\Public\CustomFont.h"

CCustomFont::CCustomFont(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device{ pGraphic_Device }
{
	Safe_AddRef(m_pGraphic_Device);
	ZeroMemory(&m_FontDesc, sizeof(D3DXFONT_DESC));
}

HRESULT CCustomFont::Initialize(const wstring& strFontFilePath)
{
	// ��Ʈ ���� ��ο��� ��Ʈ �̸� ���� �Ǵ� �⺻ ��Ʈ ���
	// DX9������ TTF ������ ���� �ε��ϴ� ��� �ý��� ��Ʈ�� ���

	m_FontDesc.Height = 16;                  // ��Ʈ ����
	m_FontDesc.Width = 0;                    // ��Ʈ �ʺ� (0�� �ڵ�)
	m_FontDesc.Weight = FW_NORMAL;           // ��Ʈ �β�
	m_FontDesc.MipLevels = 1;
	m_FontDesc.Italic = FALSE;
	m_FontDesc.CharSet = DEFAULT_CHARSET;
	m_FontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	m_FontDesc.Quality = DEFAULT_QUALITY;
	m_FontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy_s(m_FontDesc.FaceName, L"����");  // �⺻ ��Ʈ �̸�

	// ��Ʈ ���� �̸��� ������ ��� ���
	if (!strFontFilePath.empty())
	{
		// ������ ��: ���� �̸����� .ttf Ȯ���� ����
		wstring strFontName = strFontFilePath;
		size_t pos = strFontName.rfind(L'\\');
		if (pos != wstring::npos)
			strFontName = strFontName.substr(pos + 1);

		pos = strFontName.rfind(L'.');
		if (pos != wstring::npos)
			strFontName = strFontName.substr(0, pos);

		if (!strFontName.empty() && strFontName.length() < LF_FACESIZE)
			wcscpy_s(m_FontDesc.FaceName, strFontName.c_str());
	}

	// D3DX ��Ʈ ����
	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &m_FontDesc, &m_pFont)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCustomFont::Render(const wstring& strText, const _float2& vPosition, _float3 vColor)
{
	if (nullptr == m_pFont)
		return E_FAIL;

	// �ؽ�Ʈ ������ ���� ����
	RECT rc;
	SetRect(&rc, (int)vPosition.x, (int)vPosition.y, 0, 0);

	// D3DCOLOR�� ���� ��ȯ
	D3DCOLOR fontColor = D3DCOLOR_XRGB(
		(int)(vColor.x * 255.0f),
		(int)(vColor.y * 255.0f),
		(int)(vColor.z * 255.0f));

	// �ؽ�Ʈ �׸���
	m_pFont->DrawTextW(
		NULL,
		strText.c_str(),
		-1,
		&rc,
		DT_NOCLIP,
		fontColor);

	return S_OK;
}

CCustomFont* CCustomFont::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const wstring& strFontFilePath)
{
	CCustomFont* pInstance = new CCustomFont(pGraphic_Device);
	if (FAILED(pInstance->Initialize(strFontFilePath)))
	{
		MSG_BOX("Failed to Created : CCustomFont");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCustomFont::Free()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pGraphic_Device);
}