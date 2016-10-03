#ifndef TOKEN_H
#define TOKEN_H

#define tokens_ 40	//define how many tokens there are


/* enumeration of all the tokenIDs */
typedef enum tokenID {
	Eof_Tk, 
	Identifier_Tk,
	IntLiteral_Tk,

	/* Keyword Tokens */
	Bgn_Tk, End_Tk, Strt_Tk, Stp_Tk, Iff_Tk, Loop_Tk,
	Void_Tk, Var_Tk, Int_Tk, Call_Tk, Return_Tk,
	Scan_Tk, Prnt_Tk, Prg_Tk,

	/* Operator Tokens */
	Cln_Tk, DblEq_Tk, Plus_Tk, Minus_Tk, Ast_Tk, 
	BkSlash_Tk, Amper_Tk, Prcnt_Tk,

	/* Relationsal Operators */
	Eq_Tk, Lt_tk, Gt_tk, DnEq_Tk, Lt_Eq_Tk, Gt_Eq_Tk,

	/* Delimiters Tokens */
	Period_Tk, LftParen_Tk, RgtParen_Tk, Comma_Tk,
	LftBrace_Tk, RgtBrace_Tk, SemiColon_Tk, 
	LftBracket_Tk, RgtBracket_Tk
} tokenID;

extern char *tokenNames[tokens_] = {"End of File", "Identifier", 
	"Numbers", 

	"Begin Keyword", "End Keyword", "Start Keyword",
	"Stop Keyword", "Iff Keyword", "Loop Keyword", 
	"Void Keyword", "Var Keyword", "Int Keyword", 
	"Call Keyword", "Return Keyword", "Scan Keyword",
	"Print Keyword", "Program Keyword", 

	"Colon Operator", "Double Equal Operator", "Plus Operator",
	"Minus Operator", "Asterisk Operator", "Backslash Operator",
	"Ampersand Operator", "Percent Operator",

	"Equal", "Less Than", "Greater Than", "Does Not Equal",
	"Less Than Or Equal", "Greater Than Or Equal",

	"Period", "Left Parentheses", "Right Parentheses", 
	"Comma", "Left Brace", "Right Brace", "Semicolon",
	"Left Bracket", "Right Bracket"};


/* structure of the token */
typedef struct tolken_struct {
	tokenID tk_Id;
	char *tk_inst;
	int line_num;
} tk;

#endif

