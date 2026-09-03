#ifndef DEMO_ENGINE_H
#define DEMO_ENGINE_H

/* Engine selection. The Makefile picks the engine (ENGINE=engine64 or
   ENGINE=volcano64) and defines ENGINE_VOLCANO64 for the second one; it also
   force-includes this header in every unit, so the switch lives here alone.

   ENGINE_HEADER(dir, name) builds an engine header path with the engine's
   file prefix: ENGINE_HEADER(scene3d, scene3d) reads "scene3d/v64_scene3d.h"
   on volcano64 and "scene3d/e64_scene3d.h" on engine64 (computed include,
   C11 6.10.2). ENGINE_MODEL_EXT is the extension of the converted models. */

#define ENGINE_STRINGIFY(x)     #x
#define ENGINE_PATH(dir, file)  ENGINE_STRINGIFY(dir/file)

#ifdef ENGINE_VOLCANO64
#define ENGINE_HEADER(dir, name) ENGINE_PATH(dir, v64_##name.h)
#define ENGINE_MODEL_EXT         ".model"
#else
#define ENGINE_HEADER(dir, name) ENGINE_PATH(dir, e64_##name.h)
#define ENGINE_MODEL_EXT         ".t3dm"
#endif

#endif
