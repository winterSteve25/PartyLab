import os

base_dir = "../resources/scripts/api/"
funcs = {
    "utils": [
        # raylib
        ("getScreenWidth", "number", []),
        ("getScreenHeight", "number", []),
        
        # ui helper
        ("measureText", "Vector2", [("string", "text"), ("number", "fontSize")]),
        ("getCenter", "Vector2", [("Vector2", "size"), ("Vector2", "totalSize")]),
        ("within", "boolean", [("Vector2", "x"), ("Vector2", "pos"), ("Vector2", "size")])
    ],
    "core": [
        ("transitionTo", "nil", [("number", "scene")]),
        ("exit", "nil", [])
    ],
    "steam": [
    ]
}

for key, funcs in funcs.items():
    luafile = base_dir + key + ".lua"
    with open(luafile, "w") as f:
        f.write("-- Lua generated by binding.py\n")
        f.write("local m = {}\n\n")
        
        existing = f"./{key}.lua"
        if (os.path.isfile(existing)):
            with open(existing, "r") as e:
                f.writelines(e.readlines())
                f.write("\n")

        for fn, r, args in funcs:
            for arg in args:
                f.write(f"---@param {arg[1]} {arg[0]}\n")
            f.write(f"---@return {r}\n")
            param_list = ",".join(map(lambda arg: arg[1], args))
            f.write(f"m.{fn} = function({param_list})\n")
            
            if r == "nil":
                f.write(f"\tcpp_{fn}({param_list})\n")
            else:
                f.write(f"\treturn cpp_{fn}({param_list})\n")
            f.write("end\n\n")

        f.write("\nreturn m")