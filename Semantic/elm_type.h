#ifndef ELM_TYPE_H
#define ELM_TYPE_H

/* ID Type */
typedef enum elm_type
{
	Real,
	Integer,
	Void 	// for function returned type
} elm_type;

static const char* elm_types_names[] = 
{
	"Real",
	"Integer",
	"Void"
};

#endif