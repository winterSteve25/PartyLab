#pragma once

#include "IPacketType.h"
#include "MemoryReader.h"
#include "MemoryWriter.h"
#include "sol/sol.hpp"
#include "steam/SteamIDWrapper.h"

class LuaPacketType : public IPacketType
{
public:
    LuaPacketType(uint32_t id, const sol::table& table);
    ~LuaPacketType() override;
    void Handle(const CSteamID& sender, MemoryReader& reader) const override;
    void Serialize(MemoryWriter& memory, const sol::object& data) const override;
    void RawHandle(const SteamIDWrapper& sender, const sol::object& data) const override;
private:
    uint32_t m_id;
    sol::optional<sol::protected_function> m_serializer;
    sol::optional<sol::protected_function> m_deserializer;
    sol::protected_function m_handler;
};
