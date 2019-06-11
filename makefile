all: lin.lex.yy.c Actions/actions.c Token/Token.c Parser/parser.c
	gcc lin.lex.yy.c Actions/actions.c Token/Token.c Parser/parser.c -o my_parser

#tell make that "clean" is not a file name!
.PHONY: clean 

#Clean the build directory	
clean: 
	rm -vf my_parser
