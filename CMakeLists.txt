cmake_minimum_required(VERSION 3.16.0)

project(Oni LANGUAGES C)

include(CheckCSourceCompiles)
include(CheckIncludeFile)
include(CMakeDependentOption)
cmake_dependent_option(ONI_BINK_VIDEO "Build with Bink Video support" OFF "WIN32;CMAKE_SIZEOF_VOID_P EQUAL 4" OFF)
cmake_dependent_option(ONI_MAP_FILE "Generate MAP file" OFF "MSVC;WIN32;CMAKE_SIZEOF_VOID_P EQUAL 4" OFF)
cmake_dependent_option(ONI_BUILD_TEVCPROJ "Build TEVCProj utility" ON "WIN32" ON)

# MSVC 19 finally
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(BungieFrameWork)

if(WIN32)
    set(ONI_APP_TARGET_PARAMS WIN32)
else()
    set(ONI_APP_TARGET_PARAMS "")
endif()

add_executable(Oni ${ONI_APP_TARGET_PARAMS}
    OniProj/OniWin32Projs/ImpConsole/templatechecksum.c
    OniProj/OniGameSource/Oni_AI/Oni_AI.c
    OniProj/OniGameSource/Oni_AI/Oni_AI.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Alarm.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Alarm.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Alert.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Alert.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Combat.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Combat.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Error.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Error.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Executor.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Executor.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Fight.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Fight.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Guard.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Guard.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Idle.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Idle.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Knowledge.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Knowledge.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_LocalPath.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_LocalPath.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Maneuver.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Maneuver.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Melee.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Melee.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_MeleeProfile.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Movement.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Movement.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_MovementState.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_MovementState.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_MovementStub.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_MovementStub.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Neutral.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Neutral.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Panic.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Panic.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Path.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Path.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Patrol.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Patrol.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Pursuit.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Pursuit.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Script.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Script.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Targeting.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_Targeting.h
    OniProj/OniGameSource/Oni_AI/Oni_AI2_TeamBattle.c
    OniProj/OniGameSource/Oni_AI/Oni_AI2_TeamBattle.h
    OniProj/OniGameSource/Oni_AI/Oni_AI_Action.h
    OniProj/OniGameSource/Oni_AI/Oni_AI_Activity.h
    OniProj/OniGameSource/Oni_AI/Oni_AI_Behaviour.h
    OniProj/OniGameSource/Oni_AI/Oni_AI_Group.h
    OniProj/OniGameSource/Oni_AI/Oni_AI_Profile.h
    OniProj/OniGameSource/Oni_AI/Oni_AI_Script.c
    OniProj/OniGameSource/Oni_AI/Oni_AI_Script.h
    OniProj/OniGameSource/Oni_AI/Oni_AI_Setup.h
    OniProj/OniGameSource/Oni_AI/Oni_AStar.c
    OniProj/OniGameSource/Oni_AI/Oni_AStar.h
    OniProj/OniGameSource/Oni_Windows/Oni_Win_AI.c
    OniProj/OniGameSource/Oni_Windows/Oni_Win_AI.h
    OniProj/OniGameSource/Oni_Windows/Oni_Win_AI_Character.c
    OniProj/OniGameSource/Oni_Windows/Oni_Win_AI_Combat.c
    OniProj/OniGameSource/Oni_Windows/Oni_Win_AI_Combat.h
    OniProj/OniGameSource/Oni_Windows/Oni_Win_AI_Melee.c
    OniProj/OniGameSource/Oni_Windows/Oni_Win_AI_Melee.h
    OniProj/OniGameSource/Oni_Windows/Oni_Win_AI_Neutral.c
    OniProj/OniGameSource/Oni_Windows/Oni_Win_AI_Neutral.h
    OniProj/OniGameSource/Oni_Windows/Oni_Win_AI_Path.c
    OniProj/OniGameSource/Oni_Windows/Oni_Win_ImpactEffects.c
    OniProj/OniGameSource/Oni_Windows/Oni_Win_ImpactEffects.h
    OniProj/OniGameSource/Oni_Windows/Oni_Win_Particle.c
    OniProj/OniGameSource/Oni_Windows/Oni_Win_Particle.h
    OniProj/OniGameSource/Oni_Windows/Oni_Win_Settings.c
    OniProj/OniGameSource/Oni_Windows/Oni_Win_Sound2.c
    OniProj/OniGameSource/Oni_Windows/Oni_Win_Sound2.h
    OniProj/OniGameSource/Oni_Windows/Oni_Win_TextureMaterials.c
    OniProj/OniGameSource/Oni_Windows/Oni_Win_TextureMaterials.h
    OniProj/OniGameSource/Oni_Windows/Oni_Win_Tools.c
    OniProj/OniGameSource/Oni_Windows/Oni_Win_Tools.h
    OniProj/OniGameSource/Oni_Windows.c
    OniProj/OniGameSource/Oni_Windows.h
    OniProj/OniGameSource/Oni_Windows2.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Character.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Combat.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Console.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Door.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Flags.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Furniture.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Melee.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Neutral.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Particle.c
    OniProj/OniGameSource/Oni_Object/OT_PatrolPoint.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_PowerUps.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Sound.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Trigger.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_TriggerVolume.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Turret.c
    OniProj/OniGameSource/Oni_Object/object_types/OT_Weapon.c
    OniProj/OniGameSource/Oni_Object/Oni_Object.c
    OniProj/OniGameSource/Oni_Object/Oni_Object.h
    OniProj/OniGameSource/Oni_Object/Oni_Object_Private.h
    OniProj/OniGameSource/Oni_Object/Oni_Object_RegisterTemplates.c
    OniProj/OniGameSource/Oni_Object/Oni_Object_Utils.c
    OniProj/OniGameSource/Oni_BinaryData/Oni_BinaryData.c
    OniProj/OniGameSource/Oni_BinaryData/Oni_BinaryData.h
    OniProj/OniGameSource/Oni_OutGameUI/Oni_OutGameUI.c
    OniProj/OniGameSource/Oni_OutGameUI/Oni_OutGameUI.h
    OniProj/OniGameSource/Oni.c
    OniProj/OniGameSource/Oni.h
    OniProj/OniGameSource/Oni_Aiming.c
    OniProj/OniGameSource/Oni_Aiming.h
    OniProj/OniGameSource/Oni_Bink.c
    OniProj/OniGameSource/Oni_Bink.h
    OniProj/OniGameSource/Oni_Camera.c
    OniProj/OniGameSource/Oni_Camera.h
    OniProj/OniGameSource/Oni_Camera_Templates.c
    OniProj/OniGameSource/Oni_Character.c
    OniProj/OniGameSource/Oni_Character.h
    OniProj/OniGameSource/Oni_Character_Animation.h
    OniProj/OniGameSource/Oni_Cinematics.c
    OniProj/OniGameSource/Oni_Cinematics.h
    OniProj/OniGameSource/Oni_Combos.c
    OniProj/OniGameSource/Oni_Combos.h
    OniProj/OniGameSource/Oni_Event.c
    OniProj/OniGameSource/Oni_Event.h
    OniProj/OniGameSource/Oni_Film.c
    OniProj/OniGameSource/Oni_Film.h
    OniProj/OniGameSource/Oni_FX.c
    OniProj/OniGameSource/Oni_FX.h
    OniProj/OniGameSource/Oni_GameSettings.c
    OniProj/OniGameSource/Oni_GameSettings.h
    OniProj/OniGameSource/Oni_GameState.c
    OniProj/OniGameSource/Oni_GameState.h
    OniProj/OniGameSource/Oni_GameStatePrivate.h
    OniProj/OniGameSource/Oni_ImpactEffect.c
    OniProj/OniGameSource/Oni_ImpactEffect.h
    OniProj/OniGameSource/Oni_InGameUI.c
    OniProj/OniGameSource/Oni_InGameUI.h
    OniProj/OniGameSource/Oni_KeyBindings.c
    OniProj/OniGameSource/Oni_KeyBindings.h
    OniProj/OniGameSource/Oni_Level.c
    OniProj/OniGameSource/Oni_Level.h
    OniProj/OniGameSource/Oni_Mechanics.c
    OniProj/OniGameSource/Oni_Mechanics.h
    OniProj/OniGameSource/Oni_Motoko.c
    OniProj/OniGameSource/Oni_Motoko.h
    OniProj/OniGameSource/Oni_Particle3.c
    OniProj/OniGameSource/Oni_Particle3.h
    OniProj/OniGameSource/Oni_Path.c
    OniProj/OniGameSource/Oni_Path.h
    OniProj/OniGameSource/Oni_Persistance.c
    OniProj/OniGameSource/Oni_Persistance.h
    OniProj/OniGameSource/Oni_Platform.h
    OniProj/OniGameSource/Oni_Script.c
    OniProj/OniGameSource/Oni_Script.h
    OniProj/OniGameSource/Oni_Sky.c
    OniProj/OniGameSource/Oni_Sky.h
    OniProj/OniGameSource/Oni_Sound2.c
    OniProj/OniGameSource/Oni_Sound2.h
    OniProj/OniGameSource/Oni_Sound_Animation.c
    OniProj/OniGameSource/Oni_Sound_Animation.h
    OniProj/OniGameSource/Oni_Sound_Private.h
    OniProj/OniGameSource/Oni_Speech.c
    OniProj/OniGameSource/Oni_Speech.h
    OniProj/OniGameSource/Oni_Templates.c
    OniProj/OniGameSource/Oni_Templates.h
    OniProj/OniGameSource/Oni_TextureMaterials.c
    OniProj/OniGameSource/Oni_TextureMaterials.h
    OniProj/OniGameSource/Oni_Weapon.c
    OniProj/OniGameSource/Oni_Weapon.h)

target_include_directories(Oni
    PRIVATE
        OniProj/OniGameSource
        OniProj/OniGameSource/Oni_AI
        OniProj/OniGameSource/Oni_Networking
        OniProj/OniGameSource/Oni_Windows
        OniProj/OniGameSource/Oni_Object
        OniProj/OniGameSource/Oni_BinaryData
        OniProj/OniGameSource/Oni_OutGameUI
        BungieFrameWork/BFW_ToolSource/Common/Imp
        BungieFrameWork/BFW_Source/BFW_Motoko/Engines/DrawEngine/OpenGL)

target_link_libraries(Oni PRIVATE BFW)

if(WIN32)
    target_sources(Oni
        PRIVATE
            OniProj/OniGameSource/Platform_Win32/Oni_Platform_Win32.c
            OniProj/OniGameSource/Platform_Win32/Oni_Platform_Win32.h
            OniProj/OniResources/OniResources.rc
            OniProj/OniResources/resource.h)
    target_include_directories(Oni
        PRIVATE
            OniProj/OniGameSource/Platform_Win32)
    target_link_libraries(Oni
        PRIVATE
            winmm
            ws2_32
            dinput8
            dsound
            dxguid
            odbc32
            odbccp32
            imagehlp)
elseif(APPLE)
    target_sources(Oni
        PRIVATE
            BungieFrameWork/BFW_Source/BFW_CommandLine/Platform_MacOS/BFW_CL_Platform_MacOS.c
            BungieFrameWork/BFW_Source/BFW_CommandLine/Platform_MacOS/BFW_CL_Platform_MacOS.h
            BungieFrameWork/BFW_Source/BFW_DebuggerSymbols/BFW_DebuggerSymbols_MacOS.c
            BungieFrameWork/BFW_Source/BFW_FileManager/Platform_MacOS/BFW_FileManager_MacOS.c
            BungieFrameWork/BFW_Source/BFW_LocalInput/Platform_MacOS/BFW_LI_Platform_MacOS.c
            BungieFrameWork/BFW_Source/BFW_LocalInput/Platform_MacOS/BFW_LI_Platform_MacOS.h
            BungieFrameWork/BFW_Source/BFW_Motoko/Engines/DrawEngine/OpenGL/gl_macos.c
            BungieFrameWork/BFW_Source/BFW_Motoko/Manager/AltiVec/Motoko_SIMDCache_AltiVec.c
            BungieFrameWork/BFW_Source/BFW_Motoko/Manager/AltiVec/Motoko_SIMDCache_AltiVec.h
            BungieFrameWork/BFW_Headers/BFW_Platform_AltiVec.h
            BungieFrameWork/BFW_Source/BFW_Utility/Platform_MacOS/BFW_Platform_AltiVec.c
            BungieFrameWork/BFW_Source/BFW_Utility/Platform_MacOS/BFW_Platform_MacOS.c
            BungieFrameWork/BFW_Source/BFW_SoundSystem2/Platform_MacOS/BFW_SS2_Platform_MacOS.c
            OniProj/OniGameSource/Platform_MacOS/Oni_Mac_Keys.h
            OniProj/OniGameSource/Platform_MacOS/Oni_Platform_Mac.c
            OniProj/OniGameSource/Platform_MacOS/Oni_Platform_Mac.h)
    target_include_directories(Oni
        PRIVATE
            BungieFrameWork/BFW_Source/BFW_LocalInput/Platform_MacOS
            BungieFrameWork/BFW_Source/BFW_SoundSystem/Platform_MacOS
            BungieFrameWork/BFW_Source/BFW_SoundSystem2/Platform_MacOS
            BungieFrameWork/BFW_Source/BFW_CommandLine/Platform_MacOS
            OniProj/OniGameSource/Platform_MacOS)
endif()

if(ONI_MAP_FILE)
    set(ONI_MAP_PATH $<TARGET_FILE_DIR:Oni>/$<TARGET_FILE_NAME:Oni>.map)
    target_compile_definitions(Oni PRIVATE ONI_MAP_FILE="${ONI_MAP_PATH}")
    target_sources(Oni
        PRIVATE
            BungieFrameWork/BFW_Source/BFW_DebuggerSymbols/stack_walk_windows.c
            BungieFrameWork/BFW_Source/BFW_DebuggerSymbols/stack_walk_windows.h)
    target_link_options(Oni PRIVATE "/MAP:${ONI_MAP_PATH}")
endif()

target_precompile_headers(Oni REUSE_FROM BFW)

if(ONI_BINK_VIDEO)
    add_subdirectory(OniProj/OniCMakeProjs/BinkProj)
    target_compile_definitions(Oni PRIVATE BINK_VIDEO=${ONI_BINK_VIDEO})
    target_link_libraries(Oni PRIVATE bink)
endif()

if(ONI_BUILD_TEVCPROJ)
    add_subdirectory(OniProj/OniCMakeProjs/TEVCProj)
endif()
