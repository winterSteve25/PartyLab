#include "MemoryWriter.h"

#include "raylib.h"

MemoryWriter::MemoryWriter()
{
    m_buffer.reserve(128);
}

void MemoryWriter::Write(void* data, size_t size)
{
    const auto buffer = static_cast<std::byte*>(data);
    m_buffer.insert(m_buffer.end(), buffer, buffer + size);
}

void MemoryWriter::WriteString(const std::string& str)
{
    WriteGeneric<size_t>(str.length());
    void* buf = malloc(str.length());
    memcpy(buf, str.c_str(), str.length());
    Write(buf, str.length());
    free(buf);
}

void MemoryWriter::WriteBool(bool data)
{
    m_buffer.push_back(static_cast<std::byte>(data ? 1 : 0));
}

void MemoryWriter::WriteObject(const sol::object& object)
{
    /// All serializable lua objects have the following format
    /// 1 byte | type of object 0 - none, 1 - nil, 2 - string, 3 - number, 4 - boolean, 5 - table
    /// Unknown | object data
    
    sol::type type = object.get_type();
    switch (type) {
    case sol::type::thread:
        TraceLog(LOG_ERROR, "Serialization of type 'thread' is not allowed");
        return;
    case sol::type::function:
        TraceLog(LOG_ERROR, "Serialization of type 'function' is not allowed");
        return;
    case sol::type::userdata:
        TraceLog(LOG_ERROR, "Serialization of type 'userdata' is not allowed");
        return;
    case sol::type::lightuserdata:
        TraceLog(LOG_ERROR, "Serialization of type 'lightuserdata' is not allowed");
        return;
    case sol::type::poly:
        TraceLog(LOG_ERROR, "Serialization of type 'poly' is not allowed");
        return;
    case sol::type::none:
        m_buffer.push_back(static_cast<std::byte>(0));
        break;
    case sol::type::lua_nil:
        m_buffer.push_back(static_cast<std::byte>(1));
        break;
    case sol::type::string:
        m_buffer.push_back(static_cast<std::byte>(2));
        WriteString(object.as<std::string>());
        break;
    case sol::type::number:
        m_buffer.push_back(static_cast<std::byte>(3));
        WriteGeneric<double>(object.as<double>());
        break;
    case sol::type::boolean:
        m_buffer.push_back(static_cast<std::byte>(4));
        WriteBool(object.as<bool>());
        break;
    case sol::type::table:
        m_buffer.push_back(static_cast<std::byte>(5));
        auto table = object.as<sol::table>();

        /// TABLE BINARY FORMAT AS FOLLOWS
        /// 4 bytes | table size
        /// Unknown | table entries

        /// Each table entry has the following format
        /// 1 byte | Is key string
        /// Unknown | key - number or string
        /// Unknown | value - object

        uint32_t size = 0;
        table.for_each([&size](auto pair)
        {
            size++;
        });

        WriteGeneric<uint32_t>(size);
        table.for_each([this](auto pair)
        {
            sol::object first = pair.first;
            sol::object second = pair.second;

            bool isKeyStr = first.is<std::string>();
            WriteBool(isKeyStr);

            if (isKeyStr)
            {
                WriteString(first.as<std::string>());
            } else
            {
                WriteGeneric<uint32_t>(first.as<uint32_t>());
            }

            WriteObject(second);
        });
        
        break;
    }
}

const void* MemoryWriter::GetPtr() const
{
    return m_buffer.data();
}

size_t MemoryWriter::GetSize() const
{
    return m_buffer.size();
}

void MemoryWriter::Clear()
{
    m_buffer.clear();
}
