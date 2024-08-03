#pragma once
#include <string>
#include <vector>

#include "sol/sol.hpp"

class MemoryWriter
{
public:
    MemoryWriter();

    void Write(void* data, size_t size);
    void WriteString(const std::string& str);
    void WriteBool(bool data);
    void WriteObject(const sol::object& object);

    template <typename T>
    void WriteGeneric(T val)
    {
        std::byte buf[sizeof(T)];
        memcpy(buf, &val, sizeof(T));
        Write(buf, sizeof(T));
    }
    
    const void* GetPtr() const;
    size_t GetSize() const;
    
    void Clear();
private:
    std::vector<std::byte> m_buffer;
};
