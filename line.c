#include "line.h"

void initLine(Line *line, char *data, int lineSize){
    line->line = data;
    line->lineSize = lineSize;
}
