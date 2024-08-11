#include "SyncListPacket.h"

#include "SyncedList.h"
#include "core/Core.h"

SyncListPacket::~SyncListPacket()
{
}

void SyncListPacket::Handle(const CSteamID& sender, MemoryReader& reader) const
{
    bool hostOnly = reader.ReadBool();
    std::string id = reader.ReadString();
    std::string modId = reader.ReadString();
    uint32_t op = reader.ReadGeneric<uint32_t>();
    LuaMod* mod = Core::INSTANCE->modManager.GetModWithIdNullable(modId);

    if (mod == nullptr)
    {
        TraceLog(LOG_ERROR, "Received message to sync a synced var from a mod that is not loaded");
        return;
    }

    sol::object val = reader.ReadObject(mod->GetStateRef());
    std::weak_ptr<SyncedList> l = Core::INSTANCE->syncManager.GetList(mod->GetStateRef(), modId, id, hostOnly);

    if (auto list = l.lock())
    {
        if (op == 0)
        {
            // add
            list->AddNoNotify(val);
        }
        else if (op == 1)
        {
            // remove by index
            list->RemoveNoNotify(val.as<int>());
        }
        else if (op == 2)
        {
            // remove by indices
            val.as<sol::table>().for_each([&list](auto pair)
            {
                list->RemoveNoNotify(pair.second.as<int>());
            });
        }
    }
}

void SyncListPacket::Serialize(MemoryWriter& memory, const sol::object& data) const
{
    if (!data.is<sol::table>())
    {
        TraceLog(LOG_ERROR, "Tried to sync a non synced var");
        return;
    }

    sol::table table = data.as<sol::table>();

    SyncedList* syncedVar = table["obj"];
    int op = table["op"];
    sol::object val = table["val"];

    memory.WriteBool(syncedVar->hostOnly);
    memory.WriteString(syncedVar->id);
    memory.WriteString(syncedVar->modId);
    memory.WriteGeneric<uint32_t>(op);
    memory.WriteObject(val);
}

void SyncListPacket::RawHandle(const SteamIDWrapper& sender, const sol::object& data) const
{
}
