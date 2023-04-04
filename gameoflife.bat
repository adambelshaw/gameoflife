REM create basic loader .tap so we can have a loading screen and banking
.\utils\bas2tap.exe loader.bas -a
REM compile source into binary
zcc +zx -vn -startup=1 -clib=sdcc_iy -SO3 --max-allocs-per-node200000 @zproject.lst -pragma-include:zpragma.inc -o main
REM create loading screen .tap file
z88dk-appmake +zx -b screen.scr --org 16384 --noloader --blockname screen -o screen.tap
REM create code .tap file
z88dk-appmake +zx -b main_CODE.bin --org 24500 --noloader --blockname code -o code.tap
REM cat .tap files to create loadable .tap
type loader.tap screen.tap code.tap > zx-life.tap
REM tidy up
del loader.tap
del screen.tap
del code.tap
del main_CODE.bin
del main