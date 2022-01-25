#include <avr/pgmspace.h>

#include "fonts.h"

const FONT descriptors[] PROGMEM =
{
	{1,0},		// spc(32)
    {1,1},		// !  (33)
    {3,2},		// "  (34)
    {5,5},		// #  (35)
    {5,10},		// $  (36)
    {5,15},		// %  (37)
    {6,20},		// &  (38)
    {1,26},		// '  (39)
    {2,27},		// (  (40)
    {2,29},		// )  (41)
    {3,31},		// *  (42)
    {3,34},		// +  (43)
    {2,37},		// ,  (44)
    {3,39},		// -  (45)
    {1,42},		// .  (46)
    {5,43},		// /  (47)
    {5,48},		// 0  (48)
    {3,53},		// 1  (49)
    {5,56},		// 2  (50)
    {5,61},		// 3  (51)
    {5,66},		// 4  (52)
    {4,71},		// 5  (53)
    {4,75},		// 6  (54)
    {4,79},		// 7  (55)
    {4,83},		// 8  (56)
    {4,87},		// 9  (57)
    {1,91},		// :  (58)
    {2,92},		// ;  (59)
    {3,94},		// <  (60)
    {4,97},		// =  (61)
    {3,101},		// >  (62)
    {5,104},	// ?  (63)
    {7,109},	// @  (64)
    {5,116},	// A  (65)
    {5,121},	// B  (66)
    {5,126},	// C  (67)
    {5,131},	// D  (68)
    {5,136},	// E  (69)
    {5,141},	// F  (70)
    {5,146},	// G  (71)
    {4,151},	// H  (72)
    {3,155},	// I  (73)
    {4,158},	// J  (74)
    {4,162},	// K  (75)
    {4,166},	// L  (76)
    {5,170},		// M  (77)
    {5,175},		// N  (78)
    {5,180},		// O  (79)
    {5,185},		// P  (80)
    {5,190},		// Q  (81)
    {4,195},		// R  (82)
    {5,199},		// S  (83)
    {5,204},		// T  (84)
    {4,209},		// U  (85)
    {5,213},		// V  (86)
    {5,218},		// W  (87)
    {5,223},		// X  (88)
    {5,228},		// Y  (89)
    {4,233},		// Z  (90)
    {2,237},		// [  (91)
    {5,239},		// \  (92)
    {2,244},		// ]  (93)
    {5,246},		// ^  (94)
    {3,251},		// _  (95)
    {1,254},		// `  (96)
    {3,255},		// {  (123)		nowe 97
    {1,258},		// |  (124)		nowe 98
    {3,259},		// }  (125)		nowe 99
    {5,262},		// ~  (126)		nowe 100
    {5,0},		// Ł  (163)
    {6,0},		// Ą  (165)
    {5,0},		// Ę  (202)
};
