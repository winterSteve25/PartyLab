#pragma once
#include "network/IPacketType.h"

class SyncListPacket : public IPacketType
{
public:
    ~SyncListPacket() override;
    void Handle(const CSteamID& sender, MemoryReader& reader) const override;
    void Serialize(MemoryWriter& memory, const sol::object& data) const override;
    void RawHandle(const SteamIDWrapper& sender, const sol::object& data) const override;
};
