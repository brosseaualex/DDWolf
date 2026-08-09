<!--<p align="center">
  <img src="images/wolf4sdl_logo.png">
</p>-->

<h1 align="center"><em>DDWolf</em></h1>

DDWolf aims at bringing modern improvements to the Wolf4SDL engine while still preserving the game as pure and close to the original as possible.

DDWolf is based on KS-Presto's Wolf4SDL.

KS-Presto's Wolf4SDL is available at: https://github.com/KS-Presto/Wolf4SDL

<h2><em>Wolf4SDL</em></h2>

<p align="center">
  <img src="images/wolf4sdl_logo.png">
</p>

Wolf4SDL is an open-source port of id Software's classic first-person shooter Wolfenstein 3D to the cross-platform multimedia library "Simple DirectMedia Layer (SDL)" (http://www.libsdl.org).

The overall work to get to where we are would not be possible without the following people, more credits at the end of the readme.

- Original Wolfenstein 3D by id Software (http://www.idsoftware.com)
- Wolf4SDL by Moritz "Ripper" Kroll (http://www.chaos-software.de.vu - OFFLINE)
- Modifications to r262 by Andy_Nonymous and others (http://diehardwolfers.areyep.com/viewtopic.php?t=6693)

<h2><em>DDWolf Main features</em></h2>

* HD Resolution support:
  - The game will automatically detect your main monitor's resolution and be set accordingly.
    - No more screen resolution change on launch!
    - Can still manually specify resolution using launch parameter.
* AlumiuN's Advanced Sound Manager:
  - Some modifications by WSJ.
  - No sounds included.
  - Disabled by default.
* Modern control scheme:
  - WASD controls.
  - Can disable mouse Y axis.
  - #MODERN_CONTROL flag in version.h to enable or disable.
    - Original control scheme available.
* Better controller support:
  - Left analog stick : Move/Strafe
  - Right analog stick : Rotate
  - Left / Right shoulders : Previous and Next weapon
  - Back/Start Button
  - 4 Action Buttons (Can be remapped)
    - A : Fire
    - B : Strafe (mapped to key but not useful for now with controller)
    - Y : Run
    - X : Open door
* Aspect ration correction:
  - Aspect ration is automatically corrected for 4:3.
    - Can be disabled using a launch parameter.
* Set of 10 customizable "actions" and menu to assign/remap those buttons:
  - For modders to add their own actions more easily.
  - Code changes required to implement custom actions.

<h2><em>DDWolf Additional Feature Flags</em></h2>

```
#USE_MODERN_CONTROLS
#VIEASM                               AlumiuN's Advanced Sound Manager
#SEAMLESSLEVELS                       Disables the Episode selection screen on Wolf3D
#USE_READTHIS                         Enables help text in the main menu
#USE_RAND                             Enables the C++ random number generator
#WSJ_MESSAGE                          WSJ's message feature, shows on-screen messages 
                                      for pickups, secrets and locked doors
#SAVE_GAME_SCREENSHOT                 Shows a screenshot in the Save and Load screens
```

<h2><em>Wolf4SDL Additional features</em></h2>

* High-resolution texture support:
  - Wolf4SDL supports textures up to 256x256.
  - See version.h for details.
* Two additional view sizes:
  - Wolf4SDL supports one view size using the full width of the screen and showing the status bar, like in Mac-enstein, and one view size filling the whole screen (press TAB to see the status bar).
* (Nearly) unlimited sound and song lengths:
  - Mod developers are not restricted to 64kB for digitized sounds and IMF songs anymore, so longer songs and digitized sounds with better quality are possible.
* Resuming ingame music:
  - When you come back to the game from the menu or load a save game, the music will be resumed where it was suspended rather than started from the beginning.
* Freely movable pushwalls: 
  - Moving pushwalls can be viewed from all sides, allowing mod developers to place them with fewer restrictions. The player can also follow the pushwall directly instead of having to wait until the pushwall has left a whole tile.
* Optional integrated features for mod developers:
  - Wolf4SDL already contains the shading, directional 3D sprites, floor and ceiling textures, high resolution textures/sprites, parallax sky, cloud sky and outside atmosphere features, which can be easily activated in version.h.
* AdLib sounds and music:
  - This port includes the OPL2 emulator from MAME, so you can not only hear the AdLib sounds but also music without any AdLib-compatible soundcard in near to perfect quality!
* Multichannel digitized sounds:
  - Digitized sounds play on 8 channels! So in a fire fight you will always hear, when a guard opens the door behind you ;)
* Higher screen resolutions:
  - Aside from the original 320x200 resolution, Wolf4SDL currently supports any resolutions being multiples of 320x200 or 320x240, the default being 640x400.
  - Unlike some other ports, Wolf4SDL does NOT apply any bilinear or similar filtering, so the graphics are NOT blurred but pixelated just as we love it.

The following versions of Wolfenstein 3D data files are currently supported by the source code (choose the version by commenting/uncommenting lines in version.h as described in that file):

```
- Wolfenstein 3D v1.1 full Apogee
- Wolfenstein 3D v1.4 full Apogee
- Wolfenstein 3D v1.4 full GT/ID/Activision
- Wolfenstein 3D v1.4 full Imagineer (Japanese)
- Wolfenstein 3D v1.0 shareware Apogee
- Wolfenstein 3D v1.1 shareware Apogee
- Wolfenstein 3D v1.2 shareware Apogee
- Wolfenstein 3D v1.4 shareware
- Spear of Destiny full
- Spear of Destiny demo
- Spear of Destiny - Mission 2: Return to Danger
- Spear of Destiny - Mission 3: Ultimate Challenge
```

<h2><em>Operating system support</em></h2>

Currently supported operating systems : 
```
- Windows 98
- Windows ME
- Windows 2000
- Windows XP
- Windows Vista (32 and 64 bits)
- Windows 7 (32 and 64 bits)
- Windows 10 (32 and 64 bits)
- Windows 11 (32 and 64 bits)
- Linux*

*Build system untested.
```

<h2><em>How to play</em></h2>

<p>To play Wolfenstein 3D with Wolf4SDL, you just have to copy the original data files (e.g. *.WL6) into the same directory as the Wolf4SDL executable.</p>

<p>Please make sure, that you use the correct version of the executable with the according data files version as the differences are hardcoded into the binary!</p>

<p>You also need to have SDL2.dll (2.0.18) and SDL_Mixer.dll (2.0.4) in the same directory as the EXE.</p>

<p>If you play in windowed mode (--windowed parameter), press SCROLLLOCK or F12 to grab the mouse. Press it again to release the mouse.</p>

<h2><em>Usage</em></h2>

DDWolf supports the following command line options :
```
 --help                           This help page
 --tedlevel <level>               Starts the game in the given level
 --baby                           Sets the difficulty to baby for tedlevel
 --easy                           Sets the difficulty to easy for tedlevel
 --normal                         Sets the difficulty to normal for tedlevel
 --hard                           Sets the difficulty to hard for tedlevel
 --nowait                         Skips intro screens
 --windowed[-mouse]               Starts the game in a window [and grabs mouse]
 --disablehdres                   Disables resolution detection, only works in fullscreen
 --disableratiofix                Disables the 4:3 aspect ratio correction
 --res <width> <height>           Sets the screen resolution (must be multiple of 320x200 or 320x240)
 --resf <w> <h>                   Sets any screen resolution >= 320x200 (which may result in graphic errors)
 --bits <b>                       Sets the screen color depth (Use this when you have palette/fading problem or perhaps to optimize speed on old systems.)
                                  Allowed: 8, 16, 24, 32, default: "best" depth.
 --nodblbuf                       Don't use SDL's double buffering
 --extravbls <vbls>               Sets a delay after each frame, which may help to reduce flickering (SDL does not support vsync...)
                                  (unit is currently 8 ms, default: 0)
 --joystick <index>               Use the index-th joystick if available
 --joystickhat <index>            Enables movement with the given coolie hat
 --samplerate <rate>              Sets the sound sample rate (given in Hz)
 --audiobuffer <size>             Sets the size of the audio buffer (-> sound latency, given in bytes)
 --ignorenumchunks                Ignores the number of chunks in VGAHEAD.* (may be useful for some broken mods)
 --configdir <dir>                Directory where config file and save games are stored (Windows default: current directory, others: $HOME/.wolf4sdl)

= Additional launch parameters =

AlumiuN's Advanced Sound Manager
--nosound                         Turns off sound
--8bitsound                       Sets the sound to 8 bits (default 16 bits)

Spear of Destiny
--mission <mission>               Mission number to play (1-3)
--goodtimes                       Disable copy protection quiz
```

<h2><em>Compiling from source code</em></h2>

The current version of the source code is available on GitHub at: https://github.com/brosseaualex/DDWolf

**_What you need:_**

- C++ Compiler
- SDL 2 Libraries

**_Preferred methods for compiling the source code:_**

- Code::Blocks 20.03 - ***Supported***
  - DDWolf_x86.cbp (Requires 32-bits compiler)
  - DDWolf_x64.cbp (Requires 64-bits compiler)
    - README-codeblocks.txt
- Visual Studio C++ (2019/2022) - ***Supported***
  - [VS 2022] - DDWolf.VC2022.sln
  - [VS 2019] - DDWolf.VC2019.sln
    - README-VC.txt
- [CLion] CMakeList (Tested with MingW) - ***No support provided***
- [Outdated] Makefile (for Linux, BSD variants and MinGW/MSYS) - ***No support provided***

<h2><em>SDL Libraries</em></h2>

Batch files that automatically downloads and places the correct SDL2 and SDL2_Mixer libraries in the required folders are included in the repository.

You only need to run the script and open the project you want to use.

<h4>WARNING</h4>

*This script **DOES NOT WORK** with admin privileges (Run as administrator), make sure to clone the repository somewhere where elevation is **NOT** required.*

*Tested in a directory under 'C:\Users\Username'.*

<h4>Script files</h4>

```
- _get-libs-mingw.bat (Code::Blocks/MingW libraries)
- _get-libs-vc.bat (Visual Studio libraries)
```

The SDL and SDL_Mixer versions used in this project are the following :
- SDL2 2.30.7 (https://www.libsdl.org/release/)
- SDL2_mixer 2.8.0 (https://www.libsdl.org/projects/SDL_mixer/release/)

<h2><em>Important</em></h2>

Do not forget to take care of version.h!

By default it compiles for "Wolfenstein 3D 1.4 full GT/ID/Activision"!

If _CONFIG.WL6_ exists while you switch between the 'USE_MODERN_CONTROLS' flag, **_you have to delete the file so the game can re-create it._**

**_There will be issues with controls if this step is not done._**

<h2><em>Troubleshooting</em></h2>

- Issues with controls after flag change
  - Delete CONFIG.WL6
- Low frame rate
  - Consider using the original screen resolution (--res 320 200) or lowering the sound quality (--samplerate 22050)

<h2><em>Credits</em></h2>

- Special thanks to id Software! Without the source code we would still have to pelt Wolfenstein 3D with hex editors and disassemblers ;D
- Special thanks to the DOSBox team for providing a GPL'ed OPL2/3 emulator!
- Special thanks to the MAME developer team for providing the source code of the OPL2 emulator!
- Many thanks to "Der Tron" for hosting the svn repository, making Wolf4SDL FreeBSD compatible, testing, bugfixing and cleaning up the code!
- Thanks to Chris Chokan for his improvements on Wolf4GW (base of Wolf4SDL)!
- Thanks to Pickle for the GP2X support and help on 320x240 support!
- Thanks to fackue for the Dreamcast support!
- Thanks to Chris Ballinger for the Mac OS X support!
- Thanks to Xilinx, Inc. for providing a list of maximum-length LFSR counters used for higher resolutions of fizzle fade!

<h2><em>Licenses</em></h2>

- The original source code of Wolfenstein 3D (at your choice) :
  - license-id.txt
  - license-gpl.txt
- SDL
  - license-sdl.txt
- SDL_Mixer
  - license-sdl_mixer.txt
- The OPL2 emulator (at your choice) :
  - license-mame.txt (fmopl.cpp)
  - license-gpl.txt (dbopl.cpp, USE_GPL define in version.h or set GPL=1 for Makefile)