#include "trpg_utilities.h"

std::wstring CharToWide(const char* str)
{
    return std::wstring(str, str + strlen(str));
}

std::string WideToStr(const std::wstring& wstr)
{
    int size_in_bytes = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_in_bytes, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_in_bytes, NULL, NULL);
    return strTo;
}

std::wstring PadNumbers(int num)
{
    if (num < 10)
        return L"0" + std::to_wstring(num);

    return std::to_wstring(num);
}

Item::ItemType ItemTypeFromString(const std::string& item_type)
{
    if (item_type == "Health")
        return Item::ItemType::HEALTH;
    else if (item_type == "Revive")
        return Item::ItemType::REVIVE;
    else if (item_type == "StatusAffect")
        return Item::ItemType::STATUS_AFFECT;
    else if (item_type == "Battle")
        return Item::ItemType::BATTLE;
    else if (item_type == "KeyItem")
        return Item::ItemType::KEY_ITEM;
    else
        return Item::ItemType::NO_TYPE;
}
