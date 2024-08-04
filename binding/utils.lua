m.info = function(info)
    cpp_info(tostring(info))
end

m.error = function(error)
    cpp_error(tostring(error))
end

m.warning = function(warning)
    cpp_warning(tostring(warning))
end

---@param t table
---@return table A shallow copy of t
m.shallowCopyTable = function(t)
    local tt = {}

    for k, v in pairs(t) do
        tt[k] = v
    end

    return tt
end

---@param t1 table
---@param t2 table
---@return table
---Merges 2 tables, uses t1 as the base and overrides anything in t1 with t2
m.mergeTables = function(t1, t2)
    local t = m.shallowCopyTable(t1)

    for k, v in pairs(t2) do
        if type(t[k]) == "table" and type(t2[k]) == "table" then
            t[k] = m.mergeTables(t[k], t2[k])
        else
            t[k] = v
        end
    end

    return t
end

---@generic T
---@param t table
---@param key string | number
---@param default T
---@return T
m.getOr = function(t, key, default)
    local val = t[key]

    if val == nil then
        return default
    end

    return val
end

---@param t table
---@param predicate fun(t: any): boolean
---@return boolean
m.getFirst = function(t, predicate, default)
    for k, v in pairs(t) do
        if predicate(k) then
            return v
        end
    end
    
    return default
end

---@param num number
---@return number Returns -1 of the number is negative, 1 if positive, 0 if 0
m.signof = function(num)
    if num < 0 then
        return -1
    end

    if num > 0 then
        return 1
    end
    
    return 0
end