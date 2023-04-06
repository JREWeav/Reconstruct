echo "Starting"

cmake -B build

cmake --build build

START ./build/Reconstruct_artefacts/Debug/Standalone/"Reconstruct.exe"

PAUSE

TASKKILL /F /IM "Reconstruct.exe"