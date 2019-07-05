#ifndef ELM_TYPE_H
#define ELM_TYPE_H

/* ID Type */
typedef enum elm_type
{
	REAL_T,
	INTEGER_T,
	VOID_T 	// for function returned type
} elm_type;

static const char* elm_types_names[] = 
{
	"real",
	"integer",
	"void"
};

#endif