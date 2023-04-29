# CG_Engine

Сборка assimp-5.2.5 под mingw64

Скачиваем assimp - https://github.com/assimp/assimp/releases
Скачиваем cmake - https://cmake.org/download/

Распаковываем assimp и cmake в однку папку. Рядом создаём папку build.
В консоли (переходим в директорую с нашими папками)
```shell
cd build
cmake.exe assimp-5.2.5 -G "MinGW Makefiles"
```

```shell
mingw32-make.exe
```

Теперь в папке "assimp-5.2.5" удаляем все кроме папки "include".

В папке "build" переменовываем "bin" в "lib" и переносим в "assimp-5.2.5".
Копируем из "lib" файл "libassimp-5.dll" в cmake-build-debug

Из build копируем папку include в assimp-5.2.5

> Если не прописан path для cmake и mingw64 (aka 'make' is not recognized as an internal or external command...)
> Найдите где у вас находится mingw32-make.exe (в моем случае C:\mingw64\bin) и cmake.exe (в моем случае C:\cmake-3.26.3-windows-x86_64\bin)
> Перетащите exe прямо в консоль. Должно получится следующее:

```shell
cd build
C:\cmake-3.26.3-windows-x86_64\bin\cmake.exe assimp-5.2.5 -G "MinGW Makefiles"
```

```shell
C:\mingw64\bin\mingw32-make.exe
```
