#pragma once
#include "SyncedVar.h"

class SyncedList : public SyncedVar
{
public:
    SyncedList(const std::string& modId, const std::string& id, const sol::table& initialValue, bool hostOnly);
    
    void Add(const sol::object& val);
    void AddNoNotify(const sol::object& val);

    void Remove(const int& index);
    void RemoveNoNotify(const int& index);
};
