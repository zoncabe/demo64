#ifndef DEMO_ENGINE_H
#define DEMO_ENGINE_H

/* Engine selection. The Makefile picks the engine (ENGINE=engine64,
   ENGINE=volcano64 or ENGINE=ultra64) and defines ENGINE_VOLCANO64 or
   ENGINE_ULTRA64 for the last two; it also force-includes this header in
   every unit, so the switch lives here alone.

   ENGINE_HEADER(dir, name) builds an engine header path with the engine's
   file prefix: ENGINE_HEADER(scene3d, scene3d) reads "scene3d/v64_scene3d.h"
   on volcano64, "scene3d/u64_scene3d.h" on ultra64 and
   "scene3d/e64_scene3d.h" on engine64 (computed include, C11 6.10.2).
   ENGINE_MODEL_EXT is the extension of the converted models,
   ENGINE_FONT_EXT the one of the converted fonts and ENGINE_WAVE_EXT the
   one of the converted sounds. */

#define ENGINE_STRINGIFY(x) #x
#define ENGINE_PATH(dir, file) ENGINE_STRINGIFY(dir/file)

#if defined(ENGINE_VOLCANO64)
#define ENGINE_HEADER(dir, name) ENGINE_PATH(dir, v64_##name.h)
#define ENGINE_MODEL_EXT ".model"
#define ENGINE_FONT_EXT ".font64"
#define ENGINE_WAVE_EXT ".wav64"
#elif defined(ENGINE_ULTRA64)
#define ENGINE_HEADER(dir, name) ENGINE_PATH(dir, u64_##name.h)
#define ENGINE_MODEL_EXT ".model"
#define ENGINE_FONT_EXT ".font"
#define ENGINE_WAVE_EXT ".pcm"
#else
#define ENGINE_HEADER(dir, name) ENGINE_PATH(dir, e64_##name.h)
#define ENGINE_MODEL_EXT ".t3dm"
#define ENGINE_FONT_EXT ".font64"
#define ENGINE_WAVE_EXT ".wav64"
#endif

#endif
