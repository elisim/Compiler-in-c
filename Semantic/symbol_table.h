/*
Called when entering into a nested block in the program; 
creates symbol table for this block, and links it to the table of the current scope
*/
table_ptr make_table(table_ptr current_table);

/*
Called when exiting from a nested block; 
returns pointer to the enclosing block’s table (i.e. table of the exited block's "father")
*/
table_ptr pop_table(table_ptr current_table);

/*
Called when a variable declaration is processed. 
Inserts the variable name and type into symbol table of the current scope, and returns pointer to the new entry. 
Before insertion, checks whether the  id_name  already appears in the current table (for this, uses function lookup). 
If yes then returns NULL; this indicates to an error (duplicate declaration of the same name) that should be reported.
*/
table_entry insert(table_ptr current_table, char *id_name);     

/*
Searches for a variable name in symbol table of the current scope (only); 
returns pointer to the found entry, or NULL if the name is not found in that table
*/
table_entry lookup(table_ptr current_table, char *id_name);

/*
Called when variable use is found in assignment;  
allows to check whether the variable is declared. 
For this, performs hierarchical search of the variable name, starting from the symbol table of the current scope; 
returns the found entry pointer, or NULL (if the variable is undeclared). 
*/
table_entry find(table_ptr current_table, char* id_name);

/*
Called when id declaration is processed, to store its type in the symbol type.
*/
void set_id_type(table_entry id_entry, elm_type id_type);

/*
Called when assignment is processed, for variables that appear in its left and right sides. 
Returns variable's type (integer or real)
*/
elm_type get_id_type (table_entry id_entry);