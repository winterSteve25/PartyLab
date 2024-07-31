#pragma once
#include "SteamEvents.h"

class SteamIDWrapper
{
public:
    SteamIDWrapper(const CSteamID& steamId);
    
    operator CSteamID() const
    {
        return CSteamID(m_unAccountID, m_unAccountInstance, m_EUniverse, static_cast<EAccountType>(m_EAccountType));
    }
private:
    uint32 m_unAccountID;
    unsigned int m_unAccountInstance;
    unsigned int m_EAccountType;
    EUniverse m_EUniverse;
};
