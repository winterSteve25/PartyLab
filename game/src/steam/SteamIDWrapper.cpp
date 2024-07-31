#include "SteamIDWrapper.h"

SteamIDWrapper::SteamIDWrapper(const CSteamID& steamId):
    m_unAccountID(steamId.GetAccountID()),
    m_unAccountInstance(steamId.GetUnAccountInstance()),
    m_EAccountType(steamId.GetEAccountType()),
    m_EUniverse(steamId.GetEUniverse())
{
}
