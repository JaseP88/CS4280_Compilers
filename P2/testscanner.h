#ifndef TESTSCANNER_H
#define TESTSCANNER_H

#include "token.h"
#include "driver.h"
#include "scanner.h"
#include "node.h"


void testScan(char *filename);
void testScanner (char *filename);
void testScan2();
node_t *testRecursion(char *filename, tlk *tk);


#endif