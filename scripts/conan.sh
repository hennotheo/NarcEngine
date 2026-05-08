#!/usr/bin/env bash

set -euo pipefail

usage() {
  cat <<'EOF'
Usage:
  ./scripts/conan.sh [debug|release|tests|coverage] [terminal|clion]

Modes:
  debug     Prepare Conan files for the `linux-debug` preset
  release   Prepare Conan files for the `linux-release` preset
  tests     Prepare Conan files for the `linux-tests` preset
  coverage  Prepare Conan files for the `linux-coverage` preset

Layouts:
  terminal  Generate files under build/linux/... for shell usage
  clion     Generate files under build/clion/linux/... for CLion usage
EOF
}

MODE="${1:-debug}"
MODE="${MODE,,}"
LAYOUT="${2:-terminal}"
LAYOUT="${LAYOUT,,}"

case "${LAYOUT}" in
  terminal)
    BUILD_ROOT="build/linux"
    PRESET_PREFIX="linux"
    ;;
  clion)
    BUILD_ROOT="build/clion/linux"
    PRESET_PREFIX="linux-clion"
    ;;
  *)
    echo "Invalid layout: ${LAYOUT}"
    usage
    exit 1
    ;;
esac

case "${MODE}" in
  debug)
    BUILD_TYPE="Debug"
    PRESET_NAME="${PRESET_PREFIX}-debug"
    BUILD_DIR="${BUILD_ROOT}/debug"
    CMAKE_BINARY_DIR="${BUILD_DIR}/build/Debug"
    ENABLE_TESTS="False"
    ENABLE_COVERAGE="False"
    ;;
  release)
    BUILD_TYPE="Release"
    PRESET_NAME="${PRESET_PREFIX}-release"
    BUILD_DIR="${BUILD_ROOT}/release"
    CMAKE_BINARY_DIR="${BUILD_DIR}/build/Release"
    ENABLE_TESTS="False"
    ENABLE_COVERAGE="False"
    ;;
  tests)
    BUILD_TYPE="Debug"
    PRESET_NAME="${PRESET_PREFIX}-tests"
    BUILD_DIR="${BUILD_ROOT}/tests"
    CMAKE_BINARY_DIR="${BUILD_DIR}/build/Debug"
    ENABLE_TESTS="True"
    ENABLE_COVERAGE="False"
    ;;
  coverage)
    BUILD_TYPE="Debug"
    PRESET_NAME="${PRESET_PREFIX}-coverage"
    BUILD_DIR="${BUILD_ROOT}/coverage"
    CMAKE_BINARY_DIR="${BUILD_DIR}/build/Debug"
    ENABLE_TESTS="True"
    ENABLE_COVERAGE="True"
    ;;
  *)
    echo "Invalid mode: ${MODE}"
    usage
    exit 1
    ;;
esac

# check conan
if ! command -v conan >/dev/null 2>&1; then
  echo "Conan not found in PATH. Install conan (pipx recommended) and retry."
  exit 2
fi

conan profile detect --exist-ok >/dev/null

mkdir -p "${BUILD_DIR}"

CMD=(
  conan install .
  --output-folder "${BUILD_DIR}"
  --build=missing
  --profile:build default
  --profile:host default
  -c tools.cmake.cmaketoolchain:generator="Unix Makefiles"
  -s:h build_type="${BUILD_TYPE}"
  -s:h compiler.cppstd=gnu23
  -o build_tests="${ENABLE_TESTS}"
  -o coverage="${ENABLE_COVERAGE}"
  -c tools.system.package_manager:mode=install
  -c tools.system.package_manager:sudo=True
)

echo "Running: ${CMD[*]}"
"${CMD[@]}"

echo
echo "Conan install finished. Generated files are in: ${CMAKE_BINARY_DIR}/generators"
echo "Next steps:"
echo "  - Configure with the repo-owned preset for ${LAYOUT}:"
echo "      cmake --preset ${PRESET_NAME}"
echo "  - Build:"
echo "      cmake --build --preset ${PRESET_NAME}"
if [[ "${ENABLE_TESTS}" == "True" ]]; then
  echo "  - Run tests:"
  echo "      ctest --preset ${PRESET_NAME}"
fi
if [[ "${ENABLE_COVERAGE}" == "True" ]]; then
  echo "  - Generate the coverage report target:"
  echo "      cmake --build --preset ${PRESET_NAME} --target NarcCoverage"
fi
echo
