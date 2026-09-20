#!/bin/sh
# Corre la demo en el ares local: ares busca sus fuentes, Firmware y overlays
# junto al ejecutable, asi que se lo invoca donde lo deja el build.
set -e
cd "$(dirname "$0")"
ARES=${ARES:-../librerias/ares-64/build/desktop-ui/ares}
exec "$ARES" game.z64 "$@"
