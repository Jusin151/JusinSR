#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)
class ENGINE_DLL ISerializable
{	public:
	virtual json Serialize() = 0;
	virtual void Deserialize(const json& j) = 0;

    // ���̵� ���ڿ��� UTF-8�� ��ȯ (std::string ��ȯ�ؾ� ��)
    static string WideToUtf8(const wchar_t* wstr)
    {
        if (!wstr) return "";

        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
        std::string strUtf8(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &strUtf8[0], size_needed, NULL, NULL);
        strUtf8.resize(strUtf8.length() - 1); // �� ������ ����

        return strUtf8;
    }

    // UTF-8�� ���̵� ���ڿ��� ��ȯ (�ݴ� ���� ��ȯ�� �ʿ���)
    static wstring Utf8ToWide(const string& str)
    {
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
        wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstrTo[0], size_needed);
        wstrTo.resize(wstrTo.length() - 1); // �� ������ ����

        return wstrTo;
    }
};
END
