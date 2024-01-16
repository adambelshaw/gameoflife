REM create screen.tap from screen.scr (referenced from screen.asm)
.\utils\pasmo.exe --tap --name screen screen.asm screen.tap

REM compile programme using pasmo
.\utils\pasmo.exe --name ZXLife --tap main.asm main.tap

REM create basic loader .tap
.\utils\bas2tap.exe loader.bas -a

REM copy combined loader, screen, and file into final .tap
copy /b loader.tap+screen.tap+main.tap zx-life.tap

REM tidy up
del screen.tap
del main.tap
del loader.tap