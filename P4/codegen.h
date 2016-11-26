#ifndef CODEGEN_H
#define CODEGEN_H

void targetPush (char *filename);
void targetPop (char *filename);
void targetInstructAlpha (char *filename, char *instruction, char *alpha);
void targetInstructNum (char *filename, char *instruction, int num);
void targetStop(char *filename);
void targetNoop(char *filename);
void targetLabel(char *filename, char *label);
void targetVarInit(char *filename, char *var, int num);







void targetStackR (char *filename, int pos);
void targetStackW (char *filename, int pos);
void targetLoadAL (char *filename, char *alpha);
void targetLoadNUM (char *filename, int num);
void targetStoreAL (char *filename, char *alpha);
void targetStoreNUM (char *filename, int num);

#endif