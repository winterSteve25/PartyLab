# Packets
A packet is an RPC style way to send information from the client to the server, and vice versa.

A packet is defined by:
1. A method
2. A payload

In order to be sent and received, they must be registered.

Here is an example of a packet being defined:
```lua
procs = {
    addToList = function(payload)
        -- idk
    end
}
```

When sending packets, the method must be written as a string.

```lua
sendPacket("addToList")
sendPacket("exampleMod.addToList") -- Explicit mod name, allows for mods to communicate with eachother
```