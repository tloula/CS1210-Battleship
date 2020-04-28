#include <string>
#include "memory.h"

using namespace std;

void   initMemoryloula(ComputerMemory &memory);
void   updateMemoryloula(int row, int col, int result, ComputerMemory &memory);
string smartMoveloula(const ComputerMemory &memory);