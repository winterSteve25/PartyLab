#pragma once

#include "MemoryReader.h"
#include "MemoryWriter.h"
#include "sol/sol.hpp"
#include "steam/SteamIDWrapper.h"

class IPacketType
{
public:
    virtual ~IPacketType() {};
    virtual void Handle(const CSteamID& sender, MemoryReader& reader) const = 0;
    virtual void Serialize(MemoryWriter& memory, const sol::object& data) const = 0;
    virtual void RawHandle(const SteamIDWrapper& sender, const sol::object& data) const = 0;
};
