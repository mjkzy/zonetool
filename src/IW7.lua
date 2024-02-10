IW7 = {}

function IW7:include()
    includedirs {
        path.join(ProjectFolder(), "IW7")
    }
end

function IW7:link()
    self:include()
	links {
		"IW7"
	}
end

function IW7:project()
    local folder = ProjectFolder();

    project "IW7"
        kind "StaticLib"
        language "C++"
        
        pchheader "stdafx.hpp"
        pchsource(path.join(folder, "IW7/stdafx.cpp"))

        files {
            path.join(folder, "IW7/**.h"),
            path.join(folder, "IW7/**.hpp"),
            path.join(folder, "IW7/**.cpp")
        }

        self:include()
        ZoneUtils:include()

        zstd:include()
        zlib:include()

        X64:include()
end