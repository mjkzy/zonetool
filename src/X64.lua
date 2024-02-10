X64 = {}

function X64:include()
    includedirs {
        path.join(ProjectFolder(), "X64")
    }
end

function X64:link()
    self:include()
	links {
		"X64"
	}
end

function X64:project()
    local folder = ProjectFolder();

    project "X64"
        kind "StaticLib"
        language "C++"
        
        pchheader "stdafx.hpp"
        pchsource(path.join(folder, "X64/stdafx.cpp"))

        files {
            path.join(folder, "X64/**.h"),
            path.join(folder, "X64/**.hpp"),
            path.join(folder, "X64/**.cpp")
        }
        
        self:include()

        libtommath:include()
        libtomcrypt:include()
        zstd:include()
        zlib:include()
end