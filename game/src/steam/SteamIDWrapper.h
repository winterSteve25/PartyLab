#pragma once
#include <steamclientpublic.h>

class CSteamID;

class SteamIDWrapper
{
public:
    SteamIDWrapper(const CSteamID& steamId);
    
    operator CSteamID() const;
    bool operator==(const CSteamID& steamId) const;
    
    uint32 unAccountId;
private:
    unsigned int m_unAccountInstance;
    unsigned int m_EAccountType;
    EUniverse m_EUniverse;
};
