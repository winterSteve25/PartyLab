#pragma once

#include "MemoryReader.h"
#include "MemoryWriter.h"
#include "sol/sol.hpp"
#include "steam/SteamEvents.h"

class PacketType
{
public:
    PacketType(uint32_t id, const sol::table& table);
    void Handle(const CSteamID& sender, MemoryReader& reader) const;
    void Serialize(MemoryWriter& memory, const sol::object& data) const;
    void RawHandle(const SteamIDWrapper& sender, const sol::object& data) const;
private:
    uint32_t m_id;
    sol::optional<sol::protected_function> m_serializer;
    sol::optional<sol::protected_function> m_deserializer;
    sol::protected_function m_handler;
};
