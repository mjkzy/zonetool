gsc_tool = {
    source = path.join(dependencies.basePath, "gsc-tool")
}

function gsc_tool:include()
	includedirs {
		path.join(gsc_tool.source, "include"),
	}
end

function gsc_tool:link()
	links {
		"xsk-gsc-iw5", "xsk-gsc-utils"
	}
	self:include()
end

function gsc_tool:project()
	project "xsk-gsc-utils"
		location "%{wks.location}/dep"
		kind "StaticLib"
		language "C++"
		warnings "off"

		files {
			path.join(gsc_tool.source, "include/xsk/utils/*.hpp"),
			path.join(gsc_tool.source, "src/utils/*.cpp"),
		}

		includedirs {
			path.join(gsc_tool.source, "include"),
		}
		
		zlib:include()

	project "xsk-gsc-iw5"
		location "%{wks.location}/dep"
		kind "StaticLib"
		language "C++"
		warnings "off"

		filter "action:vs*"
			buildoptions "/Zc:__cplusplus"
		filter {}

		files { 
			path.join(gsc_tool.source, "include/xsk/stdinc.hpp"),

			path.join(gsc_tool.source, "include/xsk/gsc/engine/iw5_pc.hpp"),
			path.join(gsc_tool.source, "src/gsc/engine/iw5_pc.cpp"),

			path.join(gsc_tool.source, "src/gsc/engine/iw5_pc_code.cpp"),
			path.join(gsc_tool.source, "src/gsc/engine/iw5_pc_func.cpp"),
			path.join(gsc_tool.source, "src/gsc/engine/iw5_pc_meth.cpp"),
			path.join(gsc_tool.source, "src/gsc/engine/iw5_pc_token.cpp"),

			path.join(gsc_tool.source, "src/gsc/*.cpp"),

			path.join(gsc_tool.source, "src/gsc/common/*.cpp"),
			path.join(gsc_tool.source, "include/xsk/gsc/common/*.hpp"),
		}
		
		includedirs {
			path.join(gsc_tool.source, "include"),
		}
end
