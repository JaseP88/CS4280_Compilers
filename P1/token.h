#ifndef TOKEN_H
#define TOKEN_H

#define keyword_ 14	//define how many keywords there are
#define relationOp_ 6
#define otherOp_ 8
#define delim_ 9
#define token_ 6


/* enumeration of all the tokenIDs */
enum tokenID {
	EofTk,
	keywordTk,
	idenTk,
	relationTk,
	delimTk,
	//digiTk,	//digit token Id
	opTk	//operator token
};

char *tk_str[token_] = {"End Of File", "Keyword", 
	"Identifier", "Relational", "Delimiter" "Operator"};
	


/* lower level depth of tokenID for keywords */
enum keywordtk_inst {
	Bgn,
	End,
	Strt,
	Stp,
	Iff,
	Loop,
	Void,
	Var,
	Int,
	Call,
	Return,
	Scan,
	Prnt,
	Prg
};

/* string array to pair keyword instance */
char *keywordtk_Str[keyword_] = {"Begin", "End", "Start",
	"Stop", "Iff", "Loop", "Void", "Var", "Int", "Call", 
	"Return", "Scan", "Print", "Program"};






enum relationtk_inst {
	Eqtk, 
	Lttk,
	Gttk,
	Dnetk,
	Lt_Eqtk,
	Gt_Eqtk
};

char *relationtk_Str[relationOp_] = {"Equal", "LessThan", 
	"GreaterThan", "DoesNotExist", "LessOrEqual",
	"GreatOrEqual"};






enum optk_inst {
	DblEq,
	Col,
	Plus,
	Minus,
	Ast,
	BkSlash,
	Amper,
	Prcnt
};

char *optk_Str[otherOp_] = {"DoubleEqual", "Colon", "Plus",
	"Minus", "Asterisk", "BackSlash", "Ampersand", "Percent"};





enum delimtk_inst {
	Prd,
	Lparen,
	Rparen,
	Com,
	LBrkt,
	RBrkt,
	Semi,
	LBrc,
	RBrc
};

char *delimtk_Str[delim_] = {"Period", "LeftParentheses",
	"RightParentheses", "Comma", "LeftBracket",
	"RightBracket", "Semicolon", "LeftBrace", "RightBrace"};








/* structure of the token */
typedef struct tolken_struct {
	tokenID tk_Id;
	char *tk_inst;
	int line_num;
} tk;

#endif

