@echo Building...

REM compiling music
tools\mod2gbt assets/music/template.mod song 2

REM some code standards regarding ANSI C to SDCC
REM    https://github.com/gamefolk/gamefolk.github.io/blob/master/guide.md
REM makefile docs http://gbdk.sourceforge.net/doc/html/c01.html
REM https://gbdev.gg8.se/wiki/articles/GBDK#Examples_of_lcc_usage
REM https://gbdev.gg8.se/wiki/articles/The_Cartridge_Header#0147_-_Cartridge_Type
REM Wl-yt = cart type, 0x11 = Use MBC3
REM Wl-yo = num banks, 8 = 8 banks

REM compile the main file object for BANK 0
REM you'll need to add any c file not explicitly included in main here
..\..\bin\lcc -c -o bin/main.o src/main.c
..\..\bin\lcc -c -o bin/input.o src/input.c
..\..\bin\lcc -c -o bin/utils_1.o src/utils_1.c
..\..\bin\lcc -c -o bin/player.o src/player.c
..\..\bin\lcc -c -o bin/collision.o src/collision.c
..\..\bin\lcc -c -o bin/levels.o src/levels.c
..\..\bin\lcc -c -o bin/scrolling.o src/scrolling.c
..\..\bin\lcc -c -o bin/level_loading.o src/level_loading.c
..\..\bin\lcc -c -o bin/gbt_player.o src/gbt_player.s
REM compile the bank file objects
..\..\bin\lcc -Wf-bo1 -c -o bin/bank_1.o src/gbt_player_bank1.s
..\..\bin\lcc -Wf-bo2 -c -o bin/bank_2.o output.c
..\..\bin\lcc -Wf-bo3 -c -o bin/bank_3.o src/bank_3.c
..\..\bin\lcc -Wf-bo4 -c -o bin/bank_4.o src/bank_4.c
..\..\bin\lcc -Wf-bo5 -c -o bin/bank_5.o src/bank_5.c
..\..\bin\lcc -Wf-bo6 -c -o bin/bank_6.o src/bank_6.c
..\..\bin\lcc -Wf-bo7 -c -o bin/bank_7.o src/bank_7.c

REM compile the rom from all the compiled .o files
..\..\bin\lcc -Wl-m -Wl-j -Wl-yt1 -Wl-yo8 -o game.gb bin/*.o

REM move the built gb file to the build folder
move /y *.gb build

REM remove excess files (map file can be useful for rom stats)
del *.map
del *.noi
del *.ihx

REM start the game in BGB
start ..\BGB\bgb.exe build\game.gb
pause
