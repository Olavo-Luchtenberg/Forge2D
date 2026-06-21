@echo off
echo Empacotando engine...

:: cria pasta de distribuicao
if exist dist rmdir /s /q dist
mkdir dist
mkdir dist\assets

:: compila em release
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
cmake --build .
cd ..

:: copia executavel
copy build\engine.exe dist\

:: copia DLLs
copy build\SDL2.dll dist\
copy build\SDL2_mixer.dll dist\
copy build\libgcc_s_seh-1.dll dist\
copy build\libstdc++-6.dll dist\
copy build\libwinpthread-1.dll dist\
copy build\libogg.dll dist\
copy build\libvorbis.dll dist\
copy build\libvorbisfile.dll dist\
copy build\libwavpack-1.dll dist\

:: copia assets
xcopy assets dist\assets /E /I /Y

echo Pronto! Pasta dist\ esta pronta para distribuicao.
pause