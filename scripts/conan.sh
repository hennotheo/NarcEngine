set -euo pipefail

# Usage:
#   ./scripts/conan.sh [Debug|Release] [host_profile_path] [compiler_cppstd]
# Examples:
#   ./scripts/conan.sh                # Debug, no profile, no cppstd override
#   ./scripts/conan.sh Release       # Release, no profile
#   ./scripts/conan.sh Debug conan_profiles/host 20
#   ./scripts/conan.sh Debug "" gnu20

BUILD_TYPE="${1:-Debug}"
HOST_PROFILE="${2:-}"
CPPSTD="${3:-}"

# Normalize build type (accept any case)
BUILD_TYPE_LOWER="${BUILD_TYPE,,}"  # lowercase
if [[ "${BUILD_TYPE_LOWER}" == "debug" ]]; then
  BUILD_TYPE="Debug"
elif [[ "${BUILD_TYPE_LOWER}" == "release" ]]; then
  BUILD_TYPE="Release"
else
  echo "Invalid build type: ${BUILD_TYPE}"
  echo "Usage: $0 [Debug|Release] [host_profile_path] [compiler_cppstd]"
  exit 1
fi

#OUT_DIR="build/${BUILD_TYPE}"
OUT_DIR="./" # Use root dir to simplify CMake presets usage
CONAN_ARGS=(--build=missing)

# check conan
if ! command -v conan >/dev/null 2>&1; then
  echo "Conan not found in PATH. Install conan (pipx recommended) and retry."
  exit 2
fi

# build conan install command
#CMD=(conan install . --output-folder "${OUT_DIR}" -s build_type="${BUILD_TYPE}")
CMD=(conan install . -s build_type="${BUILD_TYPE}")

if [[ -n "${HOST_PROFILE}" ]]; then
  CMD+=(--profile:host "${HOST_PROFILE}")
fi

if [[ -n "${CPPSTD}" ]]; then
  CMD+=(-s compiler.cppstd="${CPPSTD}")
fi

# Run
echo "Running: ${CMD[*]}"
"${CMD[@]}"

echo
echo "Conan install finished. Generated files are in: ${OUT_DIR}/generators"
echo "Next steps:"
echo "  - Configure with CMake (example using Ninja):"
echo "      cmake -S . -B ${OUT_DIR} -G Ninja -DCMAKE_TOOLCHAIN_FILE=${OUT_DIR}/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE}"
echo "  - Or, if you have cmake >= 3.23 and Conan generated presets:"
echo "      cmake --preset conan-${BUILD_TYPE,,}"
echo