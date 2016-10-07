#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver.h"
#include "token.h"
#include "scanner.h"

#define Error -1
#define Final 1000
#define StringSize 8
#define BiggerSize 32

/* FSA table */
static int table[state_][keys_] = {
/*		Lttr 	Digi  	EOF   	WS		=		!		<		>		:		+		-		*		/		&		%		.		(		)		,		{		}		;		[		]	*/
/*s1*/	{s2, 	s3, 	1000, 	s1, 	s4 , 	-1, 	s11, 	s8, 	s14, 	s29, 	s15, 	s16, 	s17, 	s18, 	s19, 	s20, 	s21, 	s22, 	s23, 	s24, 	s25, 	s26, 	s27, 	s28},
/*s2*/	{s2, 	s2, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001, 	1001},
/*s3*/	{-1, 	s3, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002, 	1002},
/*s4*/	{1003, 	1003, 	1003, 	1003, 	s5, 	1003, 	1003, 	1003, 	1003,	1003, 	1003, 	1003, 	1003, 	1003, 	1003, 	1003, 	1003, 	1003, 	1003, 	1003, 	1003, 	1003, 	1003, 	1003},
/*s5*/	{1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004, 	1004},
/*s6*/	{-1, 	-1, 	-1, 	-1, 	s7, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1	},
/*s7*/	{1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005, 	1005},
/*s8*/	{1006, 	1006, 	1006, 	1006, 	s9, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006, 	1006},
/*s9*/	{-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	s10, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1,		-1, 	-1, 	-1	},
/*s10*/	{1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007, 	1007},
/*s11*/	{1008, 	1008, 	1008, 	1008, 	s12, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008, 	1008},
/*s12*/	{-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	s13, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1,	 	-1,	 	-1,	 	-1,	 	-1,	 	-1,	 	-1, 	-1, 	-1, 	-1, 	-1	},
/*s13*/	{1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009, 	1009},
/*s14*/	{1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010, 	1010},
/*s15*/	{1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011, 	1011},
/*s16*/	{1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012, 	1012},
/*s17*/	{1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013, 	1013},
/*s18*/	{1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014, 	1014},
/*s19*/	{1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015, 	1015},
/*s20*/	{1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016, 	1016},
/*s21*/	{1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017, 	1017},
/*s22*/	{1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018, 	1018},
/*s23*/	{1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019, 	1019},
/*s24*/	{1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020, 	1020},
/*s25*/	{1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021, 	1021},
/*s26*/	{1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022, 	1022},
/*s27*/	{1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023, 	1023},
/*s28*/	{1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024, 	1024},
/*s29*/	{1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025, 	1025},
/*s30*/	{-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1, 	-1	}	
};

static char strng[StringSize];	//build string to match keyword
static int indx = 0;
static char identName[BiggerSize];

/* Driver function for FSA table */
tlk FADriver(int currState, int keyPassed, char c) {

	tlk token;
	int nextState;

	nextState = table[currState][keyPassed];

	if (nextState <= Error) {
		fprintf(stderr,"error in nextState\n");
		token.error = 1;
		//elaborate more here
		return token;
	}

	else if (nextState >= Final) 
		
		switch(nextState) {	
			/* all the final cases */
			case 1000:
				token.tk_Id = Eof_Tk;
				token.tk_inst = tokenString[0];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			/* speacial case 1001, check if identifier string is keyword */
			case 1001:
				if (strncmp(strng, tokenString[3],5) == 0) {	//Begin KW
					token.tk_Id = Bgn_Tk;
					token.tk_inst = tokenString[3];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[4],3) == 0) {	//End KW
					token.tk_Id = End_Tk;
					token.tk_inst = tokenString[4];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[5],5) == 0) {	//Start KW
					token.tk_Id = Strt_Tk;
					token.tk_inst = tokenString[5];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[6],4) == 0) {	//Stop KW
					token.tk_Id = Stp_Tk;
					token.tk_inst = tokenString[6];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[7],3) == 0) {	//Iff KW
					token.tk_Id = Iff_Tk;
					token.tk_inst = tokenString[7];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[8],4) == 0) {	//Loop KW
					token.tk_Id = Loop_Tk;
					token.tk_inst = tokenString[8];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[9],4) == 0) {	//Void KW
					token.tk_Id = Void_Tk;
					token.tk_inst = tokenString[9];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[10],3) == 0) {	//Var KW
					token.tk_Id = Var_Tk;
					token.tk_inst = tokenString[10];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[11],3) == 0) {	//Int KW
					token.tk_Id = Int_Tk;
					token.tk_inst = tokenString[11];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[12],4) == 0) {	//Call KW
					token.tk_Id = Call_Tk;
					token.tk_inst = tokenString[12];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[13],6) == 0) {	//Return KW
					token.tk_Id = Return_Tk;
					token.tk_inst = tokenString[13];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[14],4) == 0) {	//Scan KW
					token.tk_Id = Scan_Tk;
					token.tk_inst = tokenString[14];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[15],5) == 0) {	//Print KW
					token.tk_Id = Prnt_Tk;
					token.tk_inst = tokenString[15];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else if (strncmp(strng, tokenString[16],7) == 0) {	//Program KW
					token.tk_Id = Prg_Tk;
					token.tk_inst = tokenString[16];
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

				else {	//Just an Identifier
					strcpy(identName,tokenString[1]);
					strcat(identName,strng);

					token.tk_Id = Identifier_Tk;
					token.tk_inst = identName;
					token.line = line_num;
					indx = 0; //reset string index
					state = 0; //reset the state
					token.wait=0;
					token.error=0;
					return token;
					break;
				}

			case 1002:
				token.tk_Id = IntLiteral_Tk;
				token.tk_inst = tokenString[2];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1003:
				token.tk_Id = Eq_Tk;
				token.tk_inst = tokenString[25];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1004:
				token.tk_Id = DblEq_Tk;
				token.tk_inst = tokenString[18];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1005:
				token.tk_Id = DnEq_Tk;
				token.tk_inst = tokenString[28];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;
				
			case 1006:
				token.tk_Id = Lt_Tk;
				token.tk_inst = tokenString[26];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1007:
				token.tk_Id = Lt_Eq_Tk;
				token.tk_inst = tokenString[29];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1008:
				token.tk_Id = Gt_Tk;
				token.tk_inst = tokenString[27];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1009:
				token.tk_Id = Gt_Eq_Tk;
				token.tk_inst = tokenString[30];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1010:
				token.tk_Id = Cln_Tk;
				token.tk_inst = tokenString[17];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1011:
				token.tk_Id = Minus_Tk;
				token.tk_inst = tokenString[20];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1012:
				token.tk_Id = Ast_Tk;
				token.tk_inst = tokenString[21];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1013:
				token.tk_Id = BkSlash_Tk;
				token.tk_inst = tokenString[22];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1014:
				token.tk_Id = Amper_Tk;
				token.tk_inst = tokenString[23];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1015:
				token.tk_Id = Prcnt_Tk;
				token.tk_inst = tokenString[24];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1016:
				token.tk_Id = Period_Tk;
				token.tk_inst = tokenString[31];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1017:
				token.tk_Id = LftParen_Tk;
				token.tk_inst = tokenString[32];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1018:
				token.tk_Id = RgtParen_Tk;
				token.tk_inst = tokenString[33];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1019:
				token.tk_Id = Comma_Tk;
				token.tk_inst = tokenString[34];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1020:
				token.tk_Id = LftBrace_Tk;
				token.tk_inst = tokenString[35];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1021:
				token.tk_Id = RgtBrace_Tk;
				token.tk_inst = tokenString[36];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1022:
				token.tk_Id = SemiColon_Tk;
				token.tk_inst = tokenString[37];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1023:
				token.tk_Id = LftBracket_Tk;
				token.tk_inst = tokenString[38];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1024:
				token.tk_Id = RgtBracket_Tk;
				token.tk_inst = tokenString[39];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			case 1025:
				token.tk_Id = Plus_Tk;
				token.tk_inst = tokenString[19];
				token.line = line_num;
				state = 0; //reset the state
				token.wait=0;
				token.error=0;
				return token;
				break;

			default:
				fprintf(stderr,"No Final Code Found\n");
				state = 0; //reset the state
				break;
		}

	else {

		state = nextState;
		token.wait = 1;	//token is in a state waiting for more info (lookahead)
		
		/* if character is not WS add into string array buffer to use 
			as comparison for identifiers and keywords */
		if (c != ' ') {
			strng[indx] = c;
			indx++;	
		}
		
		return token;
	}

	return token;
}
