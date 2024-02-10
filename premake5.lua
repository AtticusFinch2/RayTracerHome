-- premake5.lua
workspace "BradyTracer"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "BradyTracer"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "BradyTracer"