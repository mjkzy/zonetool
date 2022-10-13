H1 = {}

function H1:include()
    includedirs {
        path.join(ProjectFolder(), "H1")
    }
end

function H1:link()
    self:include()
	links {
		"H1"
	}
end

function H1:project()
    local folder = ProjectFolder();

    project "H1"
        kind "StaticLib"
        language "C++"
        
        pchheader "stdafx.hpp"
        pchsource(path.join(folder, "H1/stdafx.cpp"))

        files {
            path.join(folder, "H1/**.h"),
            path.join(folder, "H1/**.hpp"),
            path.join(folder, "H1/**.cpp")
        }

        self:include()
        ZoneUtils:include()

        zstd:include()
        zlib:include()
end