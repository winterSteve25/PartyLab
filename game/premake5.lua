-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event 
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial 
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you 
--  wrote the original software. If you use this software in a product, an acknowledgment 
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

local function link_steam()
    includedirs { "../steam/include" }
    includedirs { "steam/include" }

    filter { "system:windows", "platforms:x64" }
        libdirs { "../steam/redistributable_bin/win64" }
        links { "steam_api64" }
        postbuildcommands { "{COPYFILE} \"../steam/redistributable_bin/win64/steam_api64.dll\" \"../bin/%{cfg.buildcfg}\"" }
    
    filter { "system:windows", "platforms:x86" }
        libdirs { "../steam/redistributable_bin" }
        links { "steam_api" }
        postbuildcommands { "{COPYFILE} \"../steam/redistributable_bin/steam_api.dll\" \"../bin/%{cfg.buildcfg}\"" }

    filter { "system:linux", "platforms:x64" }
        libdirs { "../steam/redistributable_bin/linux64" }
        links { "steam_api" }
        
    filter { "system:linux", "platforms:x86" }
        libdirs { "../steam/redistributable_bin/linux32" }
        links { "steam_api" }

    filter "system:macosx"
        libdirs { "../steam/redistributable_bin/osx" }
        links { "steam_api" }
    
    filter {}
end

local function link_luajit() 
    includedirs { "../luajit/include", "luajit/include" }
    libdirs { "../luajit", "luajit" }
    
    filter { "system:windows", "platforms:x86" }
        links { "lua51-x86", "luajit-x86" }
    	postbuildcommands { "{COPYFILE} \"../luajit/lua51.dll\" \"../bin/%{cfg.buildcfg}\"" }
    
    filter { "system:windows", "platforms:x64" }
        links { "lua51", "luajit" }
    	postbuildcommands { "{COPYFILE} \"../luajit/lua51.dll\" \"../bin/%{cfg.buildcfg}\"" }
    
    -- filter { "system:linux", "platforms:x64" }
    -- 	links { "lua5.1", "luajit" }

    filter {}
    
end

baseName = path.getbasename(os.getcwd());
project (workspaceName)
    kind "ConsoleApp"
    location "./"
    targetdir "../bin/%{cfg.buildcfg}"

    filter "configurations:Release"
        kind "WindowedApp"
        entrypoint "mainCRTStartup"

    filter "action:vs*"
        debugdir "$(SolutionDir)"
        flags { "MultiProcessorCompile" }

    filter {"action:vs*", "configurations:Release"}
        kind "WindowedApp"
        entrypoint "mainCRTStartup"

    filter { "action:vs*", "configurations:Debug" }
        sanitize { "Address", "Fuzzer" }
        editandcontinue "Off"

    filter {}

    vpaths 
    {
        ["Header Files/*"] = { "include/**.h",  "include/**.hpp", "src/**.h", "src/**.hpp", "**.h", "**.hpp"},
        ["Source Files/*"] = {"src/**.c", "src/**.cpp","**.c", "**.cpp", "include/**.tcc" },
    }

    -- defines { "SOL_LUAJIT=1" }
    files {"**.c", "**.cpp", "**.h", "**.hpp", "**.tcc" }

    link_luajit()

    includedirs { "./" }
    includedirs { "src" }
    includedirs { "include" }

    link_steam()
    link_raylib()

    postbuildcommands { "{COPYDIR} \"../resources\" \"../bin/%{cfg.buildcfg}/resources/\"" }
