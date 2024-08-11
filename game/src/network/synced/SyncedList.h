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

    uint32_t SubscribeAdd(const sol::protected_function& callback);
    void UnsubscribeAdd(const uint32_t& index);
    void RemoveAddSubscribers();
    
    uint32_t SubscribeRemove(const sol::protected_function& callback);
    void UnsubscribeRemove(const uint32_t& index);
    void RemoveRemoveSubscribers();

    void SetNoNotify(const sol::object& val) override;

private:
    static uint32_t m_nextAddId;
    static uint32_t m_nextRemoveId;
    std::map<uint32_t, sol::protected_function> m_addSubscribers;
    std::map<uint32_t, sol::protected_function> m_removeSubscribers;
    uint32_t m_len;
    
    void UpdateLen();
};
