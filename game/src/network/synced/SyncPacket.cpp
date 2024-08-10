#include "SyncPacket.h"

#include "raylib.h"
#include "SyncedVar.h"
#include "core/Core.h"
#include "network/GameLobby.h"

SyncPacket::~SyncPacket()
{
}

void SyncPacket::Handle(const CSteamID& sender, MemoryReader& reader) const
{
    bool hostOnly = reader.ReadBool();
    std::string id = reader.ReadString();
    std::string modId = reader.ReadString();
    LuaMod* mod = Core::INSTANCE->modManager.GetModWithIdNullable(modId);

    if (mod == nullptr)
    {
        TraceLog(LOG_ERROR, "Received message to sync a synced var from a mod that is not loaded");
        return;
    }
    
    sol::object val = reader.ReadObject(mod->GetState().lua_state());
    Core::INSTANCE->syncManager.Update(modId, id, val, hostOnly);
}

void SyncPacket::Serialize(MemoryWriter& memory, const sol::object& data) const
{
    if (!data.is<sol::table>())
    {
        TraceLog(LOG_ERROR, "Tried to sync a non synced var");
        return;
    }

    SyncedVar* syncedVar = data.as<sol::table>()["obj"];
    
    memory.WriteBool(syncedVar->hostOnly);
    memory.WriteString(syncedVar->id);
    memory.WriteString(syncedVar->modId);
    memory.WriteObject(syncedVar->val);
}

void SyncPacket::RawHandle(const SteamIDWrapper& sender, const sol::object& data) const
{
}
