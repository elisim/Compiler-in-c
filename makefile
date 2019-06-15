all: lin.lex.yy.c Actions/actions.c Token/Token.c Parser/parser.c
	gcc lin.lex.yy.c Actions/actions.c Token/Token.c Parser/parser.c -o my_parser

lin.lex.yy.c:
	flex -o lin.lex.yy.c rules.lex

#tell make that "clean" is not a file name!
.PHONY: clean 

#Clean the build directory	
clean: 
	rm -vf my_parser lin.lex.yy.c
