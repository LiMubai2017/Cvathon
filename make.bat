flex cvathon.l
bison -d cvathon.y
gcc -w -o cvathon.exe cvathon.tab.c lex.yy.c display.c