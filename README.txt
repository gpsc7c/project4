Invoked with statSem [file]
Author: Gregory Shelton
Started (DD/MM/YYYY): 28/03/2024
CLASS:   Code Translation
PROFESSOR: Mark Hauschild
PROJECT: Program Translation Project 3 Static Semantics
PURPOSE: Create a variable usage check using the scanner and parser from previous project designed to be used in a compiler.
HELP:	 Command is -h, or putting in more (or less) information than is necessary.
COMPILATION METHOD: Makefile
	 Compile with make, remove compiled files with make clean
	 invoke with "./scanner [filename]"
FILES:
	IDStack.h: The location of IDStack.S's function definitions and the 
		   type definitions for sStack and eElement.
	IDStack.c: The location of the find, push, and pop functions for the
		   stack.
	tree.h:    Has been expanded to include the new function definitions
		   for static semantics that utilize the parse tree.
	tree.c:    Has been expanded to include the new functions for static 
		   semantics that utilize the parse tree as well as a second
		   and frankly unnecessary reworked definition of the node 
		   titles to assuage otherwise unsolved gdb difficulties.
	p2t001,
	p3t011,
	p3t012:    All are tests for absolute most basic errors (001,011) and
		   a success state (012
	p3t013:    Test with far more depth that checks for all commands
		   using variables
	p3t014:    As above, but tests for declaration in a deeper loop (should
		   fail)
		



parser information
FILES:	 
	frontEnd.c: The file the program starts execution in.
	Makefile:   The make file.
	langscan.c: Completed functions for use by scanner.c includes FSA table
		    and several lengthy and reused functions.
	langscan.h: Function definitions for langscan.c made to improve make 
		    behavior.
	scanner.c:  Scanner lookahed, filter, driver functions.
	scanner.h:  Scanner function definitions.
	ttoken.h:   Definition of token struct, includes enumerations of 
		    several key variable sets.
	node.h:     Definition of node struct for the parse tree, and necessary
		    substructs.
	tree.h:     Definitions of functions for loading things onto the parse
		    tree, printing it, and deleting it
	parser.h:   Function definitions for parser.
	parser.c:   Actual parser, BNF structure, and error handling.
	tests folder:
	p2t001:     Test for most basic possible program
	p2t002:     Test for var declaration and cout, which is more complex 
		    than cin.
	p2t003:     Test for a more complex program w/ jumps, loops, and a func.
	p2t004:     Test for expression variety.
	p2t005:     Test for deeper loops/statement connections. 
	p2t006+:    Test for errors.
	


SCANNER INFORMATION

Option 3
langscan.h contains the FSA table
scanner.h function FSADriver is the scanner, scanner.h function filter is the filter that goes over the scanner.
filter requires row and column be passed in as pointers to integers
scanner.h function testScan is the scan tester
An Excel spreadsheet version of the FSA table can be found in FSAChart.xlsx, 
but would need to be removed from the server to be checked.
FILES:	 
	 scanner.c is the location of main, arg intake, and help function
	 scanner.h is the location of the scanner with lookahead, filter, and scanner testing function.
	  It also includes several error handlers and token creation/generation
	 ttoken.h holds the ttoken structure and the enumeration and strings for token IDs
	 langscan.h is the location of the FSA table and numerous setters for the FSADriver function 
	 FSAChart.xlsx is a spreadsheet format of the FSA table
	 .txt files
	 aside from readme, these are tests, but readme can also function as a test. 
	  One which should fail on line 19, character 15

