#pragma once

#include "BsScriptEnginePrerequisites.h"
#include "BsScriptObject.h"
#include "BsProfilerOverlay.h"

namespace BansheeEngine
{
	/**
	 * @brief	Interop class between C++ & CLR for ProfilerOverlayInternal.
	 */
	class BS_SCR_BE_EXPORT ScriptProfilerOverlayInternal : public ScriptObject < ScriptProfilerOverlayInternal >
	{
	public:
		SCRIPT_OBJ(ENGINE_ASSEMBLY, "BansheeEngine", "ProfilerOverlayInternal")

		/**
		 * @brief	Returns the native profiler overlay instance.
		 */
		ProfilerOverlayInternal* getInternal() const { return mProfilerOverlayInternal; }

	private:
		ScriptProfilerOverlayInternal(MonoObject* managedInstance, const SPtr<CameraHandler>& camera);
		~ScriptProfilerOverlayInternal();

		ProfilerOverlayInternal* mProfilerOverlayInternal;

		/************************************************************************/
		/* 								CLR HOOKS						   		*/
		/************************************************************************/
		static void internal_CreateInstance(MonoObject* instance, ScriptCameraHandler* camera);
		static void internal_SetType(ScriptProfilerOverlayInternal* thisPtr, ProfilerOverlayType type);
		static void internal_Update(ScriptProfilerOverlayInternal* thisPtr);
		static void internal_DestroyInstance(ScriptProfilerOverlayInternal* thisPtr);
	};
}