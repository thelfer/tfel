# Visual Studio generator: ccache, copied as cl.exe by setup-ccache, replaces the
# compiler (see https://github.com/ccache/ccache/wiki/MS-Visual-Studio)
set(CMAKE_VS_GLOBALS
  "CLToolExe=cl.exe"
  "CLToolPath=$ENV{CCACHE_CL_DIR}"
  "UseMultiToolTask=true")
