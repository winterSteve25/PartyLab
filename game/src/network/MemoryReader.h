#pragma once
#include <string>

#include "sol/sol.hpp"

class MemoryReader
{
public:
    MemoryReader(const void* data, size_t size);
    
    const void* Read(size_t size);
    std::string ReadString();
    bool ReadBool();
    sol::object ReadObject(lua_State* L);

    template <typename T>
    T ReadGeneric()
    {
        const void* ptr = Read(sizeof(T));
        T x;
        memcpy(&x, ptr, sizeof(T));
        return x;
    }

    void SetData(const void* data, size_t size);

private:
    const void* m_buffer;
    size_t m_size;
    size_t m_index;
};
