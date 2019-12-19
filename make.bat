flex cvathon.l
bison -d -v cvathon.y
gcc -w -o cvathon.exe cvathon.tab.c lex.yy.c display.c symbaltable.c interlang.c utils.c