rmdir /Q /s "build" & cmake . -A x64 -B build -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_TOOLCHAIN_FILE=.\vcpkg\scripts\buildsystems\vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug -DPLAYER_BUILD_LIBLCF=ON