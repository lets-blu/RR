#ifndef __STRINGS_H__
#define __STRINGS_H__

#define ALLRED                      AllRed
#define EASTWEST                    EastWest
#define SOUTHNORTH                  SouthNorth
#define STRAIGHT                    Straight
#define LEFTTURN                    LeftTurn
#define QUIT                        Quit

#define __STRING(str)               __STRING__(str)
#define __STRING__(str)             #str
#define __STRCAT(str1, str2)        __STRCAT__(str1, str2)
#define __STRCAT__(str1, str2)      str1##str2

#define ALLRED_STRING               __STRING(ALLRED)
#define EASTWEST_LEFTTURN_STRING    __STRING(__STRCAT(EASTWEST, LEFTTURN))
#define EASTWEST_STRAIGHT_STRING    __STRING(__STRCAT(EASTWEST, STRAIGHT))
#define SOUTHNORTH_LEFTTURN_STRING  __STRING(__STRCAT(SOUTHNORTH, LEFTTURN))
#define SOUTHNORTH_STRAIGHT_STRING  __STRING(__STRCAT(SOUTHNORTH, STRAIGHT))
#define QUIT_STRING                 __STRING(QUIT)

#endif // __STRINGS_H__
