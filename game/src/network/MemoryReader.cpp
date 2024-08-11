#include "MemoryReader.h"

#include <steamtypes.h>

#include "raylib.h"
#include "steam/SteamIDWrapper.h"

MemoryReader::MemoryReader(const void* data, const size_t size):
    m_buffer(data),
    m_size(size),
    m_index(0)
{
}

const void* MemoryReader::Read(size_t size)
{
    const void* data = static_cast<void*>(((char*)m_buffer) + m_index);
    m_index += size;
    return data;
}

std::string MemoryReader::ReadString()
{
    const size_t length = ReadGeneric<size_t>();
    const auto ptr = static_cast<char*>(const_cast<void*>(Read(length)));
    std::string result(ptr, ptr + length);
    return result;
}

bool MemoryReader::ReadBool()
{
    std::byte* data = reinterpret_cast<std::byte*>(((char*)m_buffer) + m_index);
    m_index++;
    return static_cast<uint8_t>(*data) == 1;
}

sol::object MemoryReader::ReadObject(lua_State* L)
{
    auto data = reinterpret_cast<std::byte*>((((char*)m_buffer) + m_index));
    m_index++;
    uint8_t type = static_cast<uint8_t>(*data);

    sol::state_view stateView(L);

    if (type == 0)
    {
        return sol::lua_nil;
    }

    if (type == 1)
    {
        return sol::lua_nil;
    }

    if (type == 2)
    {
        std::string str = ReadString();
        return sol::make_object(L, str);
    }

    if (type == 3)
    {
        double d = ReadGeneric<double>();
        return sol::make_object(L, d);
    }

    if (type == 4)
    {
        bool b = ReadBool();
        return sol::make_object(L, b);
    }

    if (type == 5)
    {
        uint32_t size = ReadGeneric<uint32_t>();
        sol::table table = sol::table::create(L);
        
        for (uint32_t i = 0; i < size; i++)
        {
            bool isStr = ReadBool();
            sol::object key;
            
            if (isStr)
            {
                key = sol::make_object(L, ReadString());
            } else
            {
                key = sol::make_object(L, ReadGeneric<uint32_t>());
            }

            sol::object val = ReadObject(L);
            table[key] = val;
        }

        return table;
    }

    if (type == 10)
    {
        uint64 num = ReadGeneric<uint64>();
        return sol::make_object(L, SteamIDWrapper(num));
    }

    TraceLog(LOG_ERROR, "Lua object has a non serializable type");
    return sol::lua_nil;
}

void MemoryReader::SetData(const void* data, const size_t size)
{
    m_buffer = data;
    m_size = size;
    m_index = 0;
}
