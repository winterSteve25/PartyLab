#include "SteamIDWrapper.h"

SteamIDWrapper::SteamIDWrapper(const CSteamID& steamId):
    unAccountId(steamId.GetAccountID()),
    m_unAccountInstance(steamId.GetUnAccountInstance()),
    m_EAccountType(steamId.GetEAccountType()),
    m_EUniverse(steamId.GetEUniverse())
{
}

SteamIDWrapper::operator CSteamID() const
{
    return CSteamID(unAccountId, m_unAccountInstance, m_EUniverse, static_cast<EAccountType>(m_EAccountType));
}

bool SteamIDWrapper::operator==(const CSteamID& steamId) const
{
    return unAccountId == steamId.GetAccountID()
        && m_unAccountInstance == steamId.GetUnAccountInstance()
        && m_EAccountType == steamId.GetEAccountType()
        && m_EUniverse == steamId.GetEUniverse();
}
