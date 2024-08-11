#include "networkhook.h"

#include "raylib.h"
#include "core/Core.h"
#include "network/GameLobby.h"
#include "network/NetworkTarget.h"
#include "steam/SteamIDWrapper.h"

template <typename Func>
static void AddCppFunc(sol::state* state, const std::string& name, Func&& fn)
{
    state->set_function("cpp_" + name, fn);
}

void lua_network_hook::AddCppTypes(sol::state* state, bool privileged, const std::string& modId)
{
    state->new_usertype<MemoryWriter>(
        "MemoryWriter",
        sol::no_constructor,
        "writeUI8", [](MemoryWriter& writer, const uint8_t& num)
        {
            writer.WriteGeneric<uint8_t>(num);
        },
        "writeUI16", [](MemoryWriter& writer, const uint16_t& num)
        {
            writer.WriteGeneric<uint16_t>(num);
        },
        "writeUI32", [](MemoryWriter& writer, const uint32_t& num)
        {
            writer.WriteGeneric<uint32_t>(num);
        },
        "writeI8", [](MemoryWriter& writer, const int8_t& num)
        {
            writer.WriteGeneric<int8_t>(num);
        },
        "writeI16", [](MemoryWriter& writer, const int16_t& num)
        {
            writer.WriteGeneric<int16_t>(num);
        },
        "writeI32", [](MemoryWriter& writer, const int32_t& num)
        {
            writer.WriteGeneric<int32_t>(num);
        },
        "writeFloat", [](MemoryWriter& writer, const float& num)
        {
            writer.WriteGeneric<float>(num);
        },
        "writeDouble", [](MemoryWriter& writer, const double& num)
        {
            writer.WriteGeneric<double>(num);
        },
        "writeString", [](MemoryWriter& writer, const std::string& str)
        {
            writer.WriteString(str);
        },
        "writeBool", [](MemoryWriter& writer, const bool& b)
        {
            writer.WriteBool(b);
        },
        "writeObject", [](MemoryWriter& writer, const sol::object& b)
        {
            writer.WriteObject(b);
        },
        "writeSteamID", [](MemoryWriter& writer, const SteamIDWrapper& id)
        {
            CSteamID cid = id;
            writer.WriteGeneric<uint64>(cid.ConvertToUint64());
        }
    );

    state->new_usertype<MemoryReader>(
        "MemoryReader",
        sol::no_constructor,
        "readUI8", [](MemoryReader& readr)
        {
            readr.ReadGeneric<uint8_t>();
        },
        "readUI16", [](MemoryReader& readr)
        {
            readr.ReadGeneric<uint16_t>();
        },
        "readUI32", [](MemoryReader& readr)
        {
            readr.ReadGeneric<uint32_t>();
        },
        "readI8", [](MemoryReader& readr)
        {
            readr.ReadGeneric<int8_t>();
        },
        "readI16", [](MemoryReader& readr)
        {
            readr.ReadGeneric<int16_t>();
        },
        "readI32", [](MemoryReader& readr)
        {
            readr.ReadGeneric<int32_t>();
        },
        "readFloat", [](MemoryReader& readr)
        {
            readr.ReadGeneric<float>();
        },
        "readDouble", [](MemoryReader& readr)
        {
            readr.ReadGeneric<double>();
        },
        "readString", [](MemoryReader& readr)
        {
            readr.ReadString();
        },
        "readBool", [](MemoryReader& readr)
        {
            readr.ReadBool();
        },
        "readObject", [](sol::this_state state_view, MemoryReader& readr)
        {
            readr.ReadObject(state_view.lua_state());
        },
        "readSteamID", [](MemoryReader& reader)
        {
            const CSteamID id = reader.ReadGeneric<uint64>();
            return SteamIDWrapper(id);
        }
    );

    state->new_usertype<SyncedVar>(
        "SyncedVar",
        sol::call_constructor,
        sol::factories([modId](const std::string& id, const bool& hostOnly, const sol::object& initialValue)
        {
            return Core::INSTANCE->syncManager.NewVar(modId, id, initialValue, hostOnly);
        }),
        "set", &SyncedVar::Set,
        "get", &SyncedVar::Get,
        "update", &SyncedVar::Update,
        "subscribe", &SyncedVar::Subscribe,
        "unsubscribe", &SyncedVar::Unsubscribe,
        "clearSubscribers", &SyncedVar::RemoveSubscribers
    );

    state->new_usertype<SyncedList>(
        "SyncedList",
        sol::call_constructor,
        sol::factories([modId](const sol::this_state& state, const std::string& id, const bool& hostOnly)
        {
            return Core::INSTANCE->syncManager.NewList(modId, id, sol::table::create(state.lua_state()), hostOnly);
        }),
        "set", &SyncedVar::Set,
        "get", &SyncedVar::Get,
        "add", &SyncedList::Add,
        "remove", &SyncedList::Remove,
        "update", &SyncedVar::Update,
        "subscribe", &SyncedVar::Subscribe,
        "unsubscribe", &SyncedVar::Unsubscribe,
        "clearSubscribers", &SyncedVar::RemoveSubscribers,
        "subscribeAdd", &SyncedList::SubscribeAdd,
        "unsubscribeAdd", &SyncedList::UnsubscribeAdd,
        "clearAddSubscribers", &SyncedList::RemoveAddSubscribers,
        "subscribeRemove", &SyncedList::SubscribeRemove,
        "unsubscribeRemove", &SyncedList::UnsubscribeRemove,
        "clearRemoveSubscribers", &SyncedList::RemoveRemoveSubscribers
    );
}

void lua_network_hook::AddCppFuncs(sol::state* state, bool privilege, const std::filesystem::path& modDir)
{
    AddCppFunc(state, "sendPacket", [](const int& target, const int& type, const sol::object& data, const int& flags)
    {
        Core::INSTANCE->networkManager.SendMessage(static_cast<NetworkTarget>(target),
                                                   std::optional<SteamIDWrapper>(std::nullopt), type, data, flags);
    });

    AddCppFunc(state, "sendPacketTo",
               [](const SteamIDWrapper& target, const int& type, const sol::object& data, const int& flags)
               {
                   Core::INSTANCE->networkManager.SendMessage(PLAYER, target, type, data, flags);
               });
}
