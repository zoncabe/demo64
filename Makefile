BUILD_DIR = build

# Which engine builds the demo: engine64 (tiny3d) or volcano64 (magma). The
# two differ in header prefix, model format and converter; include/engine.h
# resolves the code side from ENGINE_VOLCANO64. Switching needs a clean
# build: make does not see the flag change in the objects.
ENGINE ?= volcano64

# Where the engine lives. This project never writes inside it: the engine
# sources are compiled from here into this project's build directory.
ENGINE_DIR ?= ../$(ENGINE)

include $(N64_INST)/include/n64.mk

ifeq ($(ENGINE),engine64)
include $(T3D_INST)/t3d.mk
MODEL_EXT = t3dm
else
N64_CFLAGS += -DENGINE_VOLCANO64
MODEL_EXT = model
# The engine's own model importer (host tool, built on demand).
MODEL_IMPORTER = $(ENGINE_DIR)/tools/model_importer/gltf_to_model
endif

# include/ goes first on purpose: a header of this project shadows the engine
# one of the same name. $(ENGINE_DIR)/src is there to pull an engine unit in
# with <module/file.c> for a partial override. engine.h is forced into every
# unit so the engine header macro is always defined.
N64_CFLAGS += -std=gnu2x -Iinclude -I$(ENGINE_DIR)/include -I$(ENGINE_DIR)/src \
              -include include/engine.h

# --asset-path: the importer looks every material's texture up by FILE NAME,
# recursively, under this path, and builds the path left in the model by
# replacing this prefix with "rom:/". A PNG in assets/textures/x.png gives
# rom:/textures/x.sprite, which is where this Makefile leaves the sprite.
GLTF_FLAGS = '--base-scale=1' '--asset-path=assets'

PROJECT_NAME = game

# --- sources ------------------------------------------------------------------
# The engine publishes its own source list, order and compile rule; this
# project replaces none of its units.
ENGINE_SKIP =

include $(ENGINE_DIR)/engine.mk

# The demo's own.
src = $(wildcard src/*.c) \
      $(wildcard src/*/*.c) \
      $(wildcard src/*/*/*.c)

objects = $(engine_src:%.c=$(BUILD_DIR)/engine/%.o) \
          $(src:%.c=$(BUILD_DIR)/%.o)

# --- assets -----------------------------------------------------------------

assets_png  = $(wildcard assets/textures/*.png)
assets_gltf = $(wildcard assets/models/*.glb)
assets_ttf  = $(wildcard assets/fonts/*.ttf)
assets_wav  = $(wildcard assets/audio/*.wav)

# Models with a collision mesh (declared one per line)
assets_col = filesystem/collision/room.collision \
             filesystem/collision/brew_flag.collision \
             filesystem/collision/water.collision

assets_conv = $(addprefix filesystem/textures/,$(notdir $(assets_png:%.png=%.sprite))) \
              $(addprefix filesystem/models/,$(notdir $(assets_gltf:%.glb=%.$(MODEL_EXT)))) \
              $(assets_col) \
              $(addprefix filesystem/fonts/,$(notdir $(assets_ttf:%.ttf=%.font64))) \
              $(addprefix filesystem/audio/,$(notdir $(assets_wav:%.wav=%.wav64)))


all: $(PROJECT_NAME).z64

filesystem/textures/%.sprite: assets/textures/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem/textures "$<"

filesystem/models/%.t3dm: assets/models/%.glb
	@mkdir -p $(dir $@)
	@echo "    [T3D-MODEL] $@"
	$(T3D_GLTF_TO_3D) $(GLTF_FLAGS) "$<" $@
	$(N64_BINDIR)/mkasset -c 2 -o filesystem/models $@

$(MODEL_IMPORTER):
	$(MAKE) -C $(ENGINE_DIR)/tools/model_importer

filesystem/models/%.model: assets/models/%.glb $(MODEL_IMPORTER)
	@mkdir -p $(dir $@)
	@echo "    [MODEL] $@"
	$(MODEL_IMPORTER) $(GLTF_FLAGS) "$<" $@
	$(N64_BINDIR)/mkasset -c 2 -o filesystem/models $@

# The importer comes from the engine and is compiled into build/, the only
# place this project writes to.
COLLISION_IMPORTER = $(BUILD_DIR)/tools/collision_importer

$(COLLISION_IMPORTER): $(ENGINE_DIR)/tools/collision_importer/main.c
	@mkdir -p $(dir $@)
	@echo "    [CC] $<"
	gcc -O2 -o $@ $< -I$(T3D_INST)/tools/gltf_importer/src/lib -lm

filesystem/collision/%.collision: assets/models/%.glb $(COLLISION_IMPORTER)
	@mkdir -p $(dir $@)
	@echo "    [COLLISION] $@"
	$(COLLISION_IMPORTER) "$<" $@ $(COL_MESHES)
	$(N64_BINDIR)/mkasset -c 1 -o filesystem/collision $@

filesystem/fonts/%.font64: assets/fonts/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	$(N64_MKFONT) $(MKFONT_FLAGS) -o filesystem/fonts "$<"

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

filesystem/audio/%.wav64: assets/audio/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) --wav-compress 0 -o filesystem/audio $<

$(BUILD_DIR)/$(PROJECT_NAME).dfs: $(assets_conv)
$(BUILD_DIR)/$(PROJECT_NAME).elf: $(objects)

$(PROJECT_NAME).z64: N64_ROM_TITLE="engine64 demo"
$(PROJECT_NAME).z64: $(BUILD_DIR)/$(PROJECT_NAME).dfs

clean:
	rm -rf $(BUILD_DIR) *.z64
	rm -rf filesystem

-include $(objects:%.o=%.d)

.PHONY: all clean
