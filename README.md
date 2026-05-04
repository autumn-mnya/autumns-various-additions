# **[Deprecation Notice]**

This project is now deprecated, and while you can still use it (and for new modders, or modders using AutPI, it may be recommended as it adds some useful features) it will only be updated to have bug fixes in the future if absolutely neccessary. Or moving features to separate dlls, if I really feel like it. Or outright removing features and putting them as lua script examples in the autpi repo. Nothing will be added though, more than likely.

__Ensure you use the GitHub issues system, or pull requests if you have bug fixes for Autumns Various Additions__.

# Cave Story Mod Loader ~ Autumn's Various Additions
This is a mod that adds many features to Cave Story freeware, through [Clownacy's Mod Loader](https://github.com/Clownacy/Cave-Story-Mod-Loader/releases).

# Building

The recommended method of compiling this mod is to use [CMake](https://cmake.org/).

For windows users, I would recommend installing CMake, alongside __Visual Studio 2019__ or above. Install both, open "cmake-gui" if inexperienced, open the source code folder, and set a build directory. Configure it, and set the platform to "Win32". Generate after, and open the new `.sln` file in the build folder, and build!

# License

Any of my code currently does not have a license, however some CSE2 code was used in the creation of this dll. Because of that, a license will be shipped next to the dll when compiled, or when downloading a release.

# Credits

Brayconn - .cpxe file loading in editor and game

Periwinkle - Many code changes and patches

Quotation - Custom npcs / walljumping / double jump code

Bionicobot - Original <BKG code used

# Compatibility

This dll should work for Windows 7+, but not XP or below.