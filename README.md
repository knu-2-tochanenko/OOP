# knu-2nd-oop

### Contents
* [Graph and IP Adresses (lab 1)](https://github.com/tochanenko/knu-2nd-oop#graph-and-ip-adresses-lab-1)
* [Note App (lab 2)](https://github.com/tochanenko/knu-2nd-oop#note-app-lab-2)
* [JSON to PDF converting (lab 3)](https://github.com/tochanenko/knu-2nd-oop#json-to-pdf-converting-lab-3)

### Graph and IP Adresses (lab 1)
[Go to code](https://github.com/tochanenko/knu-2nd-oop/tree/master/Graph-IP-Addresses/Graph-IP-Addresses)\
The first university progect of the second year.\
The project has two major classes : Graph and IpAddress.\
The stage of it is mostly done. Please, report a bug, if you've found one.

### Note App (lab 2)
[Go to code](https://github.com/tochanenko/knu-2nd-oop/tree/master/Note-App)\
The second university project of the second year.\
This app has listed features:
* Moving notes to archive
* Notes type (work, personal)
* Tags
* Global hotkey
* Changable themes (white - dark - orange)
* Exporting to .txt and .pdf files (WIP)

### JSON to PDF converting (lab 3)
[Go to code](https://github.com/tochanenko/knu-2nd-oop/tree/master/JsonToPdf/JsonToPdf)\
This app will be used in the second university project as .exe file.\
The release version will be included in Note App directory.\
This project uses listed third-party libraries:
* [zlib](http://www.zlib.net/)
* [libPNG](http://www.libpng.org/)
* [libHARU](http://libharu.org/)
* [nlohmann/json](https://github.com/nlohmann/json)

#### How to install libraries :
1. Download zlib, libpng, libharu and unzip archives. Also rename all folders to contain only lib name (delete lib version or -master postfix)
2. Install zlib :
    - Run CMake for zlib :
        Generator for project : Visual Studio 2017\
        INSTALL_BIN_DIR : path_to_zlib_folder/bin\
        INSTALL_INC_DIR : path_to_zlib_folder/include\
        INSTALL_LIB_DIR : path_to_zlib_folder/lib\
        INSTALL_MAIN_DIR : path_to_zlib_folder/share/man\
        INSTALL_PKGCONFIG_DIR : path_to_zlib_folder/pkgconfig
    - Open zlib.sln and build library in Release / Win32
2. Install libPNG
    - Run CMake for libPNG :
        Generator for project : Visual Studio 2017\
        Set **Advanced** to true\
        ZLIB_INCLUDE_DIR : path_to_zlib_folder\
        ZLIB_LIBRARY : path_to_zlib_folder/Release/zlib.lib\
        ZLIB_LIBRARY_DEBUG : ZLIB_LIBRARY_DEBUG-NOTFOUND\
        ZLIB_LIBRARY_RELEASE : ZLIB_LIBRARY_RELEASE-NOTFOUND\
        Set to true these configuration parameters : PNG_SHARED, PNG_STATIC, PNG_TESTS, Id-version-script
    - Open libpng.sln and build library in Release / Win32
3. Install libHARU
    - Run CMake for libHARU :
        Generator for project : Visual Studio 2017
    - Open libharu.sln and build library in Release / Win32
4. To this project:
    - VC++ Directories -> Include Directories -> paths to zlib, libpng, libharu directories
    - VC++ Directories -> LIbrary Directories -> paths to zlib/Release, libpng/Release, libharu/Release directories
    - Linker -> Additional Dependencies -> zlib.lib;libpng16.lib;libhpdf.lib;
    - Download latest [json.hpp](https://github.com/nlohmann/json/releases) file
5. And now you are good to go!