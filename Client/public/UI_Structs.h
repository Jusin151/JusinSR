#pragma once

#include "Engine_Defines.h"

enum Button_type // �� ��ư�� ��� �޴�â�� ��ư����
{
    Episode = 0, Point_Shop = 1, Spell_Shop = 2, Upgrade_Weapon = 3
};
enum Level_01_Type // �� ��ư�� ��� �̹����� ����Ұ�����
{
    Level_ICon_Defaul, Level_ICon_Selected,
    Stage_01_Color_Default, Stage_01_Color_Selected,
    Stage_02_Gray_Default, Stage_02_Gray_Selected, Stage_02_Color_Default, Stage_02_Color_Selected,
    Stage_03_Gray_Default, Stage_03_Gray_Selected, Stage_03_Color_Default, Stage_03_Color_Selected,
    Stage_04_Gray_Default, Stage_04_Gray_Selected, Stage_04_Color_Default, Stage_04_Color_Selected,
    Stage_05_Gray_Default, Stage_05_Gray_Selected, Stage_05_Color_Default, Stage_05_Color_Selected,
    Episode_Display
};
typedef struct tagEpisode_Type //���Ǽҵ� ��ư Ÿ��
{
    _bool bLevel_Icon_Button_Flag{};
    _bool bLevel_01_Stage_Button_Flag{};
    _bool bLevel_02_Stage_Button_Flag{};
    _bool bLevel_03_Stage_Button_Flag{};
    _bool bLevel_04_Stage_Button_Flag{};
    _bool bLevel_05_Stage_Button_Flag{};


}Episode_Button_Type_Desc;
