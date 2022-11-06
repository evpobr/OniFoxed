project(BFW)

add_library(BFW STATIC
    BFW_Headers/BFW.h
    BFW_Headers/BFW_Akira.h
    BFW_Headers/BFW_AppUtilities.h
    BFW_Headers/BFW_BinaryData.h
    BFW_Headers/BFW_Bink.h
    BFW_Headers/BFW_BitVector.h
    BFW_Headers/BFW_Collision.h
    BFW_Headers/BFW_CommandLine.h
    BFW_Headers/BFW_Console.h
    BFW_Headers/BFW_DebuggerSymbols.h
    BFW_Headers/BFW_DialogManager.h
    BFW_Headers/BFW_Doors.h
    BFW_Headers/BFW_Effect.h
    BFW_Headers/BFW_EnvParticle.h
    BFW_Headers/BFW_ErrorCodes.h
    BFW_Headers/BFW_FileFormat.h
    BFW_Headers/BFW_FileManager.h
    BFW_Headers/BFW_Group.h
    BFW_Headers/BFW_Image.h
    BFW_Headers/BFW_LocalInput.h
    BFW_Headers/BFW_MasterHeader.h
    BFW_Headers/BFW_Materials.h
    BFW_Headers/BFW_MathLib.h
    BFW_Headers/BFW_Motoko.h
    BFW_Headers/BFW_Noise.h
    BFW_Headers/BFW_Object.h
    BFW_Headers/BFW_Particle3.h
    BFW_Headers/BFW_Path.h
    BFW_Headers/BFW_Physics.h
    BFW_Headers/BFW_ScriptLang.h
    BFW_Headers/BFW_SoundSystem2.h
    BFW_Headers/BFW_TemplateManager.h
    BFW_Headers/BFW_TextSystem.h
    BFW_Headers/BFW_Timer.h
    BFW_Headers/BFW_Totoro.h
    BFW_Headers/BFW_Types.h
    BFW_Headers/BFW_WindowManager.h

    BFW_Source/BFW_FileManager/BFW_FileManager_Common.c

    BFW_Source/BFW_LocalInput/BFW_LI_Platform.h
    BFW_Source/BFW_LocalInput/BFW_LI_Private.h
    BFW_Source/BFW_LocalInput/BFW_LI_Translators.c
    BFW_Source/BFW_LocalInput/BFW_LocalInput.c
    BFW_Source/BFW_LocalInput/BFW_LocalInput_Template.c

    BFW_Source/BFW_Motoko/Engines/DrawEngine/OpenGL/gl_code_version.h
    BFW_Source/BFW_Motoko/Engines/DrawEngine/OpenGL/gl_engine.c
    BFW_Source/BFW_Motoko/Engines/DrawEngine/OpenGL/gl_engine.h
    BFW_Source/BFW_Motoko/Engines/DrawEngine/OpenGL/gl_geometry_draw_method.c
    BFW_Source/BFW_Motoko/Engines/DrawEngine/OpenGL/gl_utility.c
    BFW_Source/BFW_Motoko/Engines/DrawEngine/OpenGL/glext.h

    BFW_Source/BFW_Motoko/Engines/DrawEngine/Software/MS_DrawEngine_Method.h

    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Env_Clip.c
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Env_Clip.h
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_Env.c
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_Env.h
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_Frame.c
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_Frame.h
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_Geometry.c
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_Geometry.h
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_Light.c
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_Light.h
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_Matrix.c
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_Matrix.h
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_Pick.c
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_Pick.h
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_State.c
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Method_State.h
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GC_Private.h
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_Geom_Clip.c
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_Geom_Clip.h
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_Geom_Shade.c
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_Geom_Shade.h
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_Geom_Transform.c
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_Geom_Transform.h
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GeomEngine_Method.c
    BFW_Source/BFW_Motoko/Engines/GeomEngine/Software/MS_GeomEngine_Method.h

    BFW_Source/BFW_Motoko/Manager/Motoko_DefaultFunctions.c
    BFW_Source/BFW_Motoko/Manager/Motoko_DefaultFunctions.h
    BFW_Source/BFW_Motoko/Manager/Motoko_Draw.c
    BFW_Source/BFW_Motoko/Manager/Motoko_Draw.h
    BFW_Source/BFW_Motoko/Manager/Motoko_Geom.c
    BFW_Source/BFW_Motoko/Manager/Motoko_Geom_Camera.c
    BFW_Source/BFW_Motoko/Manager/Motoko_Geom_Matrix.c
    BFW_Source/BFW_Motoko/Manager/Motoko_Manager.c
    BFW_Source/BFW_Motoko/Manager/Motoko_Manager.h
    BFW_Source/BFW_Motoko/Manager/Motoko_Private.h
    BFW_Source/BFW_Motoko/Manager/Motoko_Sort.c
    BFW_Source/BFW_Motoko/Manager/Motoko_Sort.h
    BFW_Source/BFW_Motoko/Manager/Motoko_State_Draw.c
    BFW_Source/BFW_Motoko/Manager/Motoko_State_Geom.c
    BFW_Source/BFW_Motoko/Manager/Motoko_Texture.c
    BFW_Source/BFW_Motoko/Manager/Motoko_Utility.c
    BFW_Source/BFW_Motoko/Manager/Motoko_Verify.c
    BFW_Source/BFW_Motoko/Manager/Motoko_Verify.h

    BFW_Source/BFW_Motoko/Manager/BFW_Motoko_Template.c

    BFW_Source/BFW_TemplateManager/BFW_TM_Common.c
    BFW_Source/BFW_TemplateManager/BFW_TM_Construction.c
    BFW_Source/BFW_TemplateManager/BFW_TM_Construction.h
    BFW_Source/BFW_TemplateManager/BFW_TM_Game.c
    BFW_Source/BFW_TemplateManager/BFW_TM_Private.h

    BFW_Source/BFW_Utility/BFW_BitVector.c
    BFW_Source/BFW_Motoko/Engines/DrawEngine/3DFX_Glide/bfw_cseries.h
    BFW_Source/BFW_Utility/BFW_Error.c
    BFW_Source/BFW_Utility/BFW_Error.h
    BFW_Source/BFW_Utility/BFW_Memory.c
    BFW_Source/BFW_Utility/BFW_Memory.h
    BFW_Source/BFW_Utility/BFW_Noise.c
    BFW_Source/BFW_Utility/BFW_Platform.h
    BFW_Source/BFW_Utility/BFW_String.c
    BFW_Source/BFW_Utility/BFW_Timer.c
    BFW_Source/BFW_Utility/BFW_Utility.c
    BFW_Source/BFW_Motoko/Engines/DrawEngine/3DFX_Glide/lrar_cache.h
    BFW_Source/BFW_Motoko/Engines/DrawEngine/3DFX_Glide/lrar_cache2.c

    BFW_Source/BFW_Totoro/BFW_Totoro.c
    BFW_Source/BFW_Totoro/BFW_Totoro_Private.h
    BFW_Source/BFW_Totoro/BFW_Totoro_Template.c

    BFW_Source/BFW_TextSystem/BFW_TextSystem.c
    BFW_Source/BFW_TextSystem/BFW_TextSystem_Template.c
    BFW_Source/BFW_TextSystem/BFW_TS_Private.h

    BFW_Source/BFW_Console/BFW_Console.c

    BFW_Source/BFW_Group/BFW_Group.c

    BFW_Source/BFW_MathLib/bfw_math.c
    BFW_Source/BFW_MathLib/bfw_math.h
    BFW_Source/BFW_MathLib/bfw_math_3dnow.c
    BFW_Source/BFW_MathLib/bfw_math_stdc.c
    BFW_Source/BFW_MathLib/BFW_MathLib.c
    BFW_Source/BFW_MathLib/BFW_MathLib_Matrix.c
    BFW_Source/BFW_MathLib/Decompose.c
    BFW_Source/BFW_MathLib/Decompose.h
    BFW_Source/BFW_MathLib/EulerAngles.c
    BFW_Source/BFW_MathLib/EulerAngles.h

    BFW_Source/BFW_AppUtilities/BFW_AppUtilities.c

    BFW_Source/BFW_Object/BFW_Doors.c
    BFW_Source/BFW_Object/BFW_Object.c
    BFW_Source/BFW_Object/BFW_Object_Templates.h
    BFW_Source/BFW_Object/BFW_Physics.c

    BFW_Source/BFW_Path/BFW_Path.c

    BFW_Source/BFW_Collision/BFW_Collision.c

    BFW_Source/BFW_CommandLine/BFW_CommandLine.c

    BFW_Source/BFW_Akira/Akira_Private.h
    BFW_Source/BFW_Akira/BFW_Akira.c
    BFW_Source/BFW_Akira/BFW_Akira_Collision.c
    BFW_Source/BFW_Akira/BFW_Akira_LightMap.c
    BFW_Source/BFW_Akira/BFW_Akira_Render.c
    BFW_Source/BFW_Akira/BFW_Akira_Template.c

    BFW_Source/BFW_FileFormat/BMP/BFW_FF_BMP.c
    BFW_Source/BFW_FileFormat/BMP/BFW_FF_BMP.h
    BFW_Source/BFW_FileFormat/BMP/BFW_FF_BMP_Priv.h

    BFW_Source/BFW_FileFormat/PSD/BFW_FF_PSD.c
    BFW_Source/BFW_FileFormat/PSD/BFW_FF_PSD.h
    BFW_Source/BFW_FileFormat/PSD/BFW_FF_PSD_Priv.h

    BFW_Source/BFW_FileFormat/DDS/BFW_FF_DDS.c
    BFW_Source/BFW_FileFormat/DDS/BFW_FF_DDS.h
    BFW_Source/BFW_FileFormat/DDS/BFW_FF_DDS_Priv.h

    BFW_Source/BFW_FileFormat/BFW_FileFormat.c

    BFW_Source/BFW_Image/BFW_Image.c
    BFW_Source/BFW_Image/BFW_Image_Dither.c
    BFW_Source/BFW_Image/BFW_Image_Draw.c
    BFW_Source/BFW_Image/BFW_Image_PixelConversion.c
    BFW_Source/BFW_Image/BFW_Image_Private.h
    BFW_Source/BFW_Image/BFW_Image_Scale_Box.c

    BFW_Source/BFW_Particle/BFW_Decal.c
    BFW_Source/BFW_Particle/BFW_EnvParticle.c
    BFW_Source/BFW_Particle/BFW_Particle3.c

    BFW_Source/BFW_Shared/BFW_Shared_Camera.h
    BFW_Source/BFW_Shared/BFW_Shared_Clip.c
    BFW_Source/BFW_Shared/BFW_Shared_Clip.h
    BFW_Source/BFW_Shared/BFW_Shared_ClipCompPoint_c.h
    BFW_Source/BFW_Shared/BFW_Shared_ClipPoly_c.h
    BFW_Source/BFW_Shared/BFW_Shared_Math.c
    BFW_Source/BFW_Shared/BFW_Shared_Math.h
    BFW_Source/BFW_Shared/BFW_Shared_TriRaster.h
    BFW_Source/BFW_Shared/BFW_Shared_TriRaster_c.h

    BFW_Source/BFW_Effect/BFW_Effect.c
    BFW_Source/BFW_Effect/BFW_Effect_Private.h

    BFW_Source/BFW_WindowManager/WM_Cursor/WM_Cursor.c
    BFW_Source/BFW_WindowManager/WM_Cursor/WM_Cursor.h

    BFW_Source/BFW_WindowManager/WM_Dialog/WM_Dialog.c
    BFW_Source/BFW_WindowManager/WM_Dialog/WM_Dialog.h

    BFW_Source/BFW_WindowManager/WM_DrawContext/WM_DrawContext.c
    BFW_Source/BFW_WindowManager/WM_DrawContext/WM_DrawContext.h

    BFW_Source/BFW_WindowManager/WM_PartSpecification/WM_PartSpecification.c
    BFW_Source/BFW_WindowManager/WM_PartSpecification/WM_PartSpecification.h

    BFW_Source/BFW_WindowManager/WM_Utilities/WM_Utilities.c
    BFW_Source/BFW_WindowManager/WM_Utilities/WM_Utilities.h

    BFW_Source/BFW_WindowManager/WM_Windows/WM_Box.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Box.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Button.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Button.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_CheckBox.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_CheckBox.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_EditField.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_EditField.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_ListBox.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_ListBox.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Menu.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Menu.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_MenuBar.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_MenuBar.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Picture.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Picture.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_PopupMenu.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_PopupMenu.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_ProgressBar.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_ProgressBar.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_RadioButton.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_RadioButton.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Scrollbar.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Scrollbar.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Slider.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Slider.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Text.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Text.h
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Window.c
    BFW_Source/BFW_WindowManager/WM_Windows/WM_Window.h

    BFW_Source/BFW_WindowManager/BFW_WindowManager.c
    BFW_Source/BFW_WindowManager/BFW_WindowManager_Private.h
    BFW_Source/BFW_WindowManager/BFW_WindowManager_Public.h

    BFW_Source/BFW_ScriptLang/BFW_ScriptLang.c
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Context.c
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Context.h
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Database.c
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Database.h
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Error.c
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Error.h
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Eval.c
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Eval.h
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Expr.c
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Expr.h
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Parse.c
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Parse.h
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Private.h
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Scheduler.c
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Scheduler.h
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Token.c
    BFW_Source/BFW_ScriptLang/BFW_ScriptLang_Token.h

    BFW_Source/BFW_BinaryData/BFW_BinaryData.c
    BFW_Source/BFW_BinaryData/BFW_BinaryData_Template.c

    BFW_Source/BFW_SoundSystem2/BFW_SoundSystem2.c
    BFW_Source/BFW_SoundSystem2/BFW_SS2_IMA.c
    BFW_Source/BFW_SoundSystem2/BFW_SS2_IMA.h
    BFW_Source/BFW_SoundSystem2/BFW_SS2_Platform.h
    BFW_Source/BFW_SoundSystem2/BFW_SS2_Private.h
    BFW_Source/BFW_SoundSystem2/BFW_SS2_RegisterTemplate.c

    BFW_Source/BFW_Materials/BFW_Materials.c

    BFW_Source/BFW_Bink/BinkHeaders/bink.h
    BFW_Source/BFW_Bink/BinkHeaders/Rad.h

    BFW_Source/BFW_Bink/BFW_Bink.c)
target_include_directories(BFW
    PUBLIC
        BFW_Headers
        BFW_Source/BFW_Akira
        BFW_Source/BFW_Object
        BFW_Source/BFW_Batou
        BFW_Source/BFW_Bink
        BFW_Source/BFW_Bink/BinkHeaders
        BFW_Source/BFW_Console
        BFW_Source/BFW_DialogManager
        BFW_Source/BFW_DialogManager/DM_Cursor
        BFW_Source/BFW_DialogManager/VM_Views
        BFW_Source/BFW_FileManager
        BFW_Source/BFW_FileFormat
        BFW_Source/BFW_FileFormat/BMP
        BFW_Source/BFW_FileFormat/PSD
        BFW_Source/BFW_FileFormat/DDS
        BFW_Source/BFW_Group
        BFW_Source/BFW_Effect
        BFW_Source/BFW_LocalInput
        BFW_Source/BFW_MathLib
        BFW_Source/BFW_Motoko
        BFW_Source/BFW_Motoko/Engines/OGL_Common
        BFW_Source/BFW_Motoko/Engines
        BFW_Source/BFW_Motoko/Engines/DrawEngine/Direct3D
        BFW_Source/BFW_Motoko/Engines/DrawEngine/Software
        BFW_Source/BFW_Motoko/Engines/DrawEngine/3DFX_Glide
        BFW_Source/BFW_Motoko/Engines/GeomEngine/Software
        BFW_Source/BFW_Motoko/Engines/GeomEngine/GeomOpenGL
        BFW_Source/BFW_Motoko/Manager
        BFW_Source/BFW_Motoko/Manager/Win32
        BFW_Source/BFW_NetworkManager
        BFW_Source/BFW_NetworkManager/NM_Queues
        BFW_Source/BFW_NetworkManager/NM_WinSock
        BFW_Source/BFW_RedPig
        BFW_Source/BFW_RemoteInput
        BFW_Source/BFW_Shared
        BFW_Source/BFW_SoundSystem
        BFW_Source/BFW_SoundSystem2
        BFW_Source/BFW_TemplateManager
        BFW_Source/BFW_TextSystem
        BFW_Source/BFW_Totoro
        BFW_Source/BFW_Utility
        BFW_Source/BFW_WindowManager
        BFW_Source/BFW_WindowManager/WM_Cursor
        BFW_Source/BFW_WindowManager/WM_Dialog
        BFW_Source/BFW_WindowManager/WM_DrawContext
        BFW_Source/BFW_WindowManager/WM_PartSpecification
        BFW_Source/BFW_WindowManager/WM_Utilities
        BFW_Source/BFW_WindowManager/WM_Windows
        BFW_ToolSource/Common/LSSolutionFileIO
        BFW_ToolSource/Common/LSSolutionFileIO/LSReader
        BFW_ToolSource/Common/TemplateExtractor
        BFW_ToolSource/Win32/ModelExporter
        BFW_Source/BFW_CommandLine
        BFW_Source/BFW_Motoko/Engines/DrawEngine/OpenGL
    PRIVATE
        ${Oni_SOURCE_DIR}/OniProj/OniGameSource
        ${Oni_SOURCE_DIR}/OniProj/OniGameSource/Oni_AI
        ${Oni_SOURCE_DIR}/OniProj/OniGameSource/Oni_Object)

if(WIN32)
    target_sources(BFW
        PRIVATE
            BFW_Source/BFW_CommandLine/Platform_Win32/BFW_CL_Platform_Win32.c
            BFW_Source/BFW_CommandLine/Platform_Win32/BFW_CL_Platform_Win32.h
            BFW_Source/BFW_DebuggerSymbols/BFW_DebuggerSymbols_Win32.c
            BFW_Source/BFW_FileManager/Platform_Win32/BFW_FileManager_Win32.c
            BFW_Source/BFW_LocalInput/Platform_Win32/BFW_LI_Platform_Win32.c
            BFW_Source/BFW_LocalInput/Platform_Win32/BFW_LI_Platform_Win32.h
            BFW_Source/BFW_Motoko/Engines/DrawEngine/OpenGL/gl_mswindows.c
            BFW_Source/BFW_Motoko/Manager/Win32/Motoko_Platform_Win32.c
            BFW_Source/BFW_Motoko/Manager/Win32/Motoko_Platform_Win32.h
            BFW_Source/BFW_Utility/Platform_Win32/BFW_Platform_Win32.c
            BFW_Source/BFW_SoundSystem2/BFW_SS2_MSADPCM.c
            BFW_Source/BFW_SoundSystem2/BFW_SS2_MSADPCM.h
            BFW_Source/BFW_SoundSystem2/Platform_Win32/BFW_SS2_Platform_Win32.c)
    target_include_directories(BFW
        PUBLIC
            BFW_Source/BFW_LocalInput/Platform_Win32
            BFW_Source/BFW_SoundSystem/Platform_Win32
            BFW_Source/BFW_SoundSystem2/Platform_Win32
            BFW_Source/BFW_CommandLine/Platform_Win32
        PRIVATE
        ${Oni_SOURCE_DIR}/OniProj/OniGameSource/Platform_Win32)
    target_link_libraries(BFW PRIVATE dinput8 dsound dxguid msacm32)
endif()

target_precompile_headers(BFW PRIVATE BFW_Headers/BFW_MasterHeader.h)
