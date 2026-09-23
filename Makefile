PROJECT_NAME = game

# Where the engine lives. This project never writes inside it: the engine
# sources are compiled from here into this project's build directory.
# engine.mk brings the toolchain, the engine sources, every asset found
# under assets/ with its converter, the importers and the ROM. What is
# named here is only the demo's own.
ENGINE_DIR ?= ../engine64

# Code layout from MipsFit (see engine.mk). Regenerated from a CPU trace of the
# gameplay scene; ld/ holds only the linker scripts. MIPSFIT_LAYOUT_SCRIPT=
# on the command line links with libdragon's stock n64.ld instead.
MIPSFIT_LAYOUT_SCRIPT ?= $(CURDIR)/ld/layout-01.ld

# The demo's own sources.
src = $(wildcard src/*.cpp) \
      $(wildcard src/*/*.cpp) \
      $(wildcard src/*/*/*.cpp)

# --asset-path: the importer looks every material's texture up by FILE NAME,
# recursively, under this path, and builds the path left in the model by
# replacing this prefix with "rom:/". A PNG in assets/textures/x.png gives
# rom:/textures/x.sprite, which is where the build leaves the sprite.
#
# --skin-strips: stripify the chunks of a model with a skeleton too. Upstream
# skips them, so a character's triangles go out one command each: 578 of them
# per frame here, against 34 with the flag on. Needs the engine's own importer.
GLTF_FLAGS = '--asset-path=assets' --skin-strips

# Models with a collision mesh, declared one per line.
assets_collision = filesystem/collision/room.collision \
                   filesystem/collision/brew_flag.collision \
                   filesystem/collision/water.collision

# The audio is left uncompressed.
AUDIOCONV_FLAGS = --wav-compress 0

# Xolonium is wide: --display squeezes the glyphs to 75% without touching
# their height. The five Xolonium .ttf are the same typeface, since mkfont
# takes the output name from the input file: one copy is needed per size.
XOLONIUM_FLAGS = --display 320x240,2:1

filesystem/fonts/DroidSans.font64:  MKFONT_FLAGS += --size 10
filesystem/fonts/Xolonium10.font64: MKFONT_FLAGS += --size 10 $(XOLONIUM_FLAGS)
filesystem/fonts/Xolonium14.font64: MKFONT_FLAGS += --size 14 $(XOLONIUM_FLAGS)
filesystem/fonts/Xolonium20.font64: MKFONT_FLAGS += --size 20 $(XOLONIUM_FLAGS)
filesystem/fonts/Xolonium40.font64: MKFONT_FLAGS += --size 43 $(XOLONIUM_FLAGS)
filesystem/fonts/Xolonium60.font64: MKFONT_FLAGS += --size 60 $(XOLONIUM_FLAGS)

# The 2D characters' frames are palette art: CI8 keeps them at a quarter of
# the RGBA32 size in RAM.
filesystem/sprites/characters/%.sprite: MKSPRITE_FLAGS += --format CI8

# Diagnostic: the stage tiles as RGBA16 instead of the CI4 mksprite picks
# for their palette PNGs, to take the 4-bit load path out of the frame.
filesystem/stages/%.sprite: MKSPRITE_FLAGS += --format RGBA16

include $(ENGINE_DIR)/engine.mk

# After the include: n64.mk, pulled in by engine.mk, starts N64_CXXFLAGS over.
# include/ goes first on purpose: a header of this project shadows the engine
# one of the same name. engine.h is forced into every unit so the engine
# header macro is always defined.
N64_CXXFLAGS := -Iinclude -include include/engine.h $(N64_CXXFLAGS)

$(PROJECT_NAME).z64: N64_ROM_TITLE = "engine64 demo"
