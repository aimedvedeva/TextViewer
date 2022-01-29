#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

typedef struct{
    char *line;
    int lineSize;
}Line;

void initLine(Line *line, char *data, int lineSize);

#endif // LINE_H_INCLUDED
