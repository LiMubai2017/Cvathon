enum node_kind {ID_NODE,INTEGER_NODE,LPRP_NODE,PLUS_NODE,MINUS_NODE,STAR_NODE,DIV_NODE,UMINUS_NODE,ASSIGN_NODE,
				DECLARE_SUB_NODE,INT_NODE,FLOAT_NODE,CHAR_NODE,
				INC_PREFIX_NODE,INC_SUFFIX_NODE,DEC_PREFIX_NODE,DEC_SUFFIX_NODE,
				IF_NODE,ELSE_NODE,CONTINUE_NODE,BREAK_NODE,WHILE_NODE,RETURN_NODE,
				GREATER_NODE,LESS_NODE,EQUAL_NODE,GREATER_EQUAL_NODE,LESS_EQUAL_NODE,
				ID_ARRAY_NODE,FOR_NODE,FUNCTION_DECLARE_NODE,FUNCTION_FIRE_NODE,
				VALUE_LIST_NODE,CONDITION_LIST_NODE};
enum function_type {INT_FUNCTION,CHAR_FUNCTION,FLOAT_FUNCTION,VOID_FUNCTION};
enum return_type {RETURN_VOID,RETURN_EXP};
enum link_type {LINK_AND,LINK_OR};
typedef struct Exp {
	enum node_kind kind;
	union {
		struct {
			enum function_type returnType;
			struct Exp *pExp;
		}function;
		struct {
			char type_id[33]; 
			struct Exp *valueList;
		}fire;
		struct {
			enum link_type type;
			struct Exp *pExp1;
			struct Exp *pExp2;
		}condition_list;
		struct {
			char type_id[33];             
			int dimension;	
			int index1;		
			int index2;		
			int index3;		
		}id;
		int type_integer;            
		struct {
			struct Exp *pExp1;
			struct Exp *pExp2;
			}ptr;              
		struct {
			struct Exp *p1;
			struct Exp *p2;
			struct Exp *p3;
		}for_exp;
		struct {
			enum return_type returnType;
			struct Exp *pExp;
		}return_exp;
	};
} *PEXP;

enum array_kind {LP_ARRAY,RP_ARRAY,BLP_ARRAY,BRP_ARRAY,PLUS_ARRAY,MINUS_ARRAY,STAR_ARRAY,DIV_ARRAY,INTEGER_ARRAY,ID_ARRAY,
				 ASSIGNOP_ARRAY,INT_ARRAY,FLOAT_ARRAY,CHAR_ARRAY,GREATER_ARRAY,LESS_ARRAY,EQUAL_ARRAY,GREATER_EQUAL_ARRAY,
				 LESS_EQUAL_ARRAY,IF_ARRAY,ELSE_ARRAY,WHILE_ARRAY,CONTINUE_ARRAY,BREAK_ARRAY,INC_ARRAY,DEC_ARRAY,
				 MLP_ARRAY,MRP_ARRAY,FOR_ARRAY};
				 
typedef struct Array {
	enum array_kind kind;
	char value[30];
	struct Array *next;
} *ARRAYP;

