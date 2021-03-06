#ifndef TOKEN_H
#define TOKEN_H

#define tokens_ 40	//define how many tokens there are


/* enumeration of all the tokenIDs */
typedef enum tokenID {
	Eof_Tk, 
	Identifier_Tk,
	IntLiteral_Tk,	//2

	/* Keyword Tokens */
	Bgn_Tk, End_Tk, Strt_Tk, Stp_Tk, Iff_Tk, Loop_Tk,
	Void_Tk, Var_Tk, Int_Tk, Call_Tk, Return_Tk,
	Scan_Tk, Prnt_Tk, Prg_Tk,	//16

	/* Operator Tokens */
	Cln_Tk, DblEq_Tk, Plus_Tk, Minus_Tk, Ast_Tk, 
	BkSlash_Tk, Amper_Tk, Prcnt_Tk,		//24

	/* Relationsal Operators */
	Eq_Tk, Lt_Tk, Gt_Tk, DnEq_Tk, Lt_Eq_Tk, Gt_Eq_Tk,	//30

	/* Delimiters Tokens */
	Period_Tk, LftParen_Tk, RgtParen_Tk, Comma_Tk,
	LftBrace_Tk, RgtBrace_Tk, SemiColon_Tk, 
	LftBracket_Tk, RgtBracket_Tk	//39
} tokenID;

extern char *tokenString[];

/* structure of the token */
typedef struct tolken_struct {
	tokenID tk_Id;
	char *tk_inst;
	int line;

	/* these 2 last properties holds extra information on the token.
	if error set to 1, if waiting set to 1*/
	int error;	//if token has an error
	int wait;	//if token is waiting to be pieced together
} tlk;

#endif
