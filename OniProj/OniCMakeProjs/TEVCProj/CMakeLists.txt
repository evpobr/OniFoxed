project(TEVCProj)

if (WIN32)
    add_executable(TEVCProj
        ${Oni_SOURCE_DIR}/BungieFrameWork/BFW_ToolSource/Common/TemplateExtractor/TemplateExtractor.c
        ${Oni_SOURCE_DIR}/BungieFrameWork/BFW_ToolSource/Common/TemplateExtractor/TE_Extract.c
        ${Oni_SOURCE_DIR}/BungieFrameWork/BFW_ToolSource/Common/TemplateExtractor/TE_Parser.c
        ${Oni_SOURCE_DIR}/BungieFrameWork/BFW_ToolSource/Common/TemplateExtractor/TE_Symbol.c
        ${Oni_SOURCE_DIR}/BungieFrameWork/BFW_Source/BFW_DebuggerSymbols/BFW_DebuggerSymbols_Win32.c
        ${Oni_SOURCE_DIR}/BungieFrameWork/BFW_Source/BFW_FileManager/Platform_Win32/BFW_FileManager_Win32.c
        ${Oni_SOURCE_DIR}/BungieFrameWork/BFW_Source/BFW_Utility/Platform_Win32/BFW_Platform_Win32.c
        ${Oni_SOURCE_DIR}/OniProj/OniWin32Projs/ImpConsole/templatechecksum.c)

    target_link_libraries(TEVCProj PRIVATE BFW)
    target_include_directories(TEVCProj
        PRIVATE
            ${Oni_SOURCE_DIR}/OniProj/OniGameSource
            ${Oni_SOURCE_DIR}/OniProj/OniGameSource/Oni_AI
            ${Oni_SOURCE_DIR}/OniProj/OniGameSource/Oni_Networking
            ${Oni_SOURCE_DIR}/OniProj/OniGameSource/Oni_Windows
            ${Oni_SOURCE_DIR}/OniProj/OniGameSource/Oni_Object
            ${Oni_SOURCE_DIR}/OniProj/OniGameSource/Oni_BinaryData
            ${Oni_SOURCE_DIR}/OniProj/OniGameSource/Oni_OutGameUI
            ${Oni_SOURCE_DIR}/OniProj/OniGameSource/Platform_Win32
            ${Oni_SOURCE_DIR}/BungieFrameWork/BFW_ToolSource/Common/Imp
            ${Oni_SOURCE_DIR}/BungieFrameWork/BFW_Source/BFW_Motoko/Engines/DrawEngine/OpenGL)
endif ()

target_precompile_headers(TEVCProj REUSE_FROM BFW)
