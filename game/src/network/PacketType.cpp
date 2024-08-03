#include "PacketType.h"

#include <format>

#include "lua/lua_utils.h"
#include "steam/SteamIDWrapper.h"

PacketType::PacketType(uint32_t id, const sol::table& table):
    m_id(id),
    m_serializer(table.get<sol::optional<sol::protected_function>>("serializer")),
    m_deserializer(table.get<sol::optional<sol::protected_function>>("deserializer")),
    m_handler(table.get<sol::protected_function>("handler"))
{
}

void PacketType::Handle(const CSteamID& sender, MemoryReader& reader) const
{
    if (m_deserializer.has_value())
    {
        auto data = lua_utils::UnwrapResult<sol::object>(
            m_deserializer.value()(reader),
            std::format("Failed to deserialize packet with id {} with custom deserializer", m_id)
        );
        RawHandle(sender, data);
    }
    else
    {
        RawHandle(sender, reader.ReadObject(m_handler.lua_state()));
    }
}

void PacketType::Serialize(MemoryWriter& memory, const sol::object& data) const
{
    if (m_serializer.has_value())
    {
        lua_utils::UnwrapResult(
            m_serializer.value()(memory, data),
            std::format("Failed to serialize packet with id {} with custom serializer", m_id)
        );
    }
    else
    {
        memory.WriteObject(data);
    }
}

void PacketType::RawHandle(const SteamIDWrapper& sender, const sol::object& data) const
{
    lua_utils::UnwrapResult(m_handler(sender, data), std::format("Failed to handle packet of id {}", m_id));
}
