
--------------------------------------------------------------------------------

workspace "ippc"
startproject "ippc"

location "build"
filename "workspace"

configurations {
  "debug", "release"
}

staticruntime "off"

filter { "configurations:debug" }
defines "BUILD_DEBUG"
symbols "full"
runtime "debug"
filter { }

filter { "configurations:release" }
defines "BUILD_RELEASE"
optimize "on"
runtime "release"
filter { }

--------------------------------------------------------------------------------

project "ippc"
kind "ConsoleApp"
language "C++"
cppdialect "C++17"

targetdir "build/%{cfg.buildcfg}/bin/"
objdir "build/%{cfg.buildcfg}/obj/"

includedirs { "." }
files { "*.hpp", "*.h", "*.cpp" }

--------------------------------------------------------------------------------
