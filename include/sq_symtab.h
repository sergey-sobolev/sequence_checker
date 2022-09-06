/** 
 * @file
 * @brief Symbol table related types definitions and functions prototypes.
 */

#ifndef SQ_SYMTAB_HDR
#define SQ_SYMTAB_HDR


/// Record structure in the symbol table
typedef struct symbol SYMBOL;
struct symbol
{
   char *name;           //!< variable or function name
   int   type;           //!< type: BLTIN, UNDEF
   int   id;             //!< unique identifier for variables
   union
   {
      double  val;       //!< variable value( VAR )
      char   *str;       //!< string
   } u;

   struct symbol *next;  //!< link to the next record
};



typedef struct _mem_list_ MEM_LIST;
struct _mem_list_
{
  char              *ptr;
  struct _mem_list_ *prev;
};

extern SYMBOL *sq_sym_install( char *s, int t, double d );
extern SYMBOL *sq_sym_lookup( char *s );
extern unsigned int allocated;
extern unsigned int freed;


void init_mem_list(void);
void free_mem_list(void);

extern char *emalloc( unsigned n ); 

char* sq_id_to_str( int type );

#endif // SQ_SYMTAB_HDR
