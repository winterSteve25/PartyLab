---@type Scene
local m = {}

local num = SyncedVar("num", true, 0)

m.ui = function(data)
    return
    {
        {
            "Complete the sentence: ",
        },
        {
            type = "text",
            id = "num",
            text = tostring(num:get()),
            onClick = function()
                num:set(num:get() + 1)
                data.query("num").setText(tostring(num:get()))
            end
        }
    }
end

return m