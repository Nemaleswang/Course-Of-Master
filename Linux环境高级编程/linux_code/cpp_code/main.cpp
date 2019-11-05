#include "../include/bpt.h"
#include "../include/TextTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <iostream>
#include <fstream>

using namespace bpt;
using namespace std;

const char *errorMessage = "> your input is invalid,print \".help\" for more infomation!\n";
const char *nextLineHeader ="> ";
const char *exitMessage = "> bye!\n";
const char *dbFileName = "./data/db.bin";
