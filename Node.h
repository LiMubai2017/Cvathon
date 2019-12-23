enum node_kind {ID_NODE,INTEGER_NODE,LPRP_NODE,PLUS_NODE,MINUS_NODE,STAR_NODE,DIV_NODE,UMINUS_NODE,ASSIGN_NODE,
				DECLARE_SUB_NODE,INT_NODE,FLOAT_NODE,CHAR_NODE,
				INC_PREFIX_NODE,INC_SUFFIX_NODE,DEC_PREFIX_NODE,DEC_SUFFIX_NODE,
				IF_NODE,ELSE_NODE,CONTINUE_NODE,BREAK_NODE,WHILE_NODE,RETURN_NODE,
				GREATER_NODE,LESS_NODE,EQUAL_NODE,GREATER_EQUAL_NODE,LESS_EQUAL_NODE,
				ID_ARRAY_NODE,FOR_NODE,FUNCTION_DECLARE_NODE,FUNCTION_FIRE_NODE,
				VALUE_LIST_NODE,CONDITION_LIST_NODE,FUNCTION_PARAM_NODE,ARRAY_NODE};
enum function_type {INT_FUNCTION,CHAR_FUNCTION,FLOAT_FUNCTION,VOID_FUNCTION};
enum return_type {RETURN_VOID,RETURN_EXP};
enum link_type {LINK_AND,LINK_OR,LINK_NOT};
enum BlockType{IF_BLOCK,WHILE_BLOCK,FOR_BLOCK,FUNCTION_BLOCK,OTHER_BLOCK};
enum ParamType{INT_PARAM,CHAR_PARAM,FLOAT_PARAM};
typedef struct Exp {
	enum node_kind kind;
	union {
		struct {
			char function_name[33];
			enum function_type returnType;
			struct Exp *pExp;
		}function;
		struct {
			char param_name[33];
			enum ParamType param_type;
			struct Exp *pExp;
		}function_param;
		struct {
			char fire_id[33]; 
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
		struct {
			char array_id[33];             
			int dimension;	
			struct Exp *pExp1;		
			struct Exp *pExp2;		
			struct Exp *pExp3;		
		}array;
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

enum Operation
{
    OP_LABEL,
    OP_FUNCTION,
    OP_ASSIGN,
    OP_PLUS,
    OP_MINUS,
    OP_STAR,
    OP_DIV,
    OP_GOTO,
    OP_RETURN,
    OP_ARG,
    OP_CALL,
    OP_PARAM,
    OP_GREATER,
    OP_LESS,
    OP_GE,
    OP_LE,
    OP_EQUAL,
    OP_UE,
    OP_READ,
    OP_WRITE,
    OP_DEC,
	OP_BREAK,
	OP_CONTINUE,
};

typedef struct CodeNode
{
    enum Operation op;
    char opn1[10], opn2[10], result[10];
    struct CodeNode *next, *pre;
} * CodeNodeP;