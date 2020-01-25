/**
 * Name: Eric Minseo Park
 * Student Number: 1001018
 **/

#include <math.h>

//Defined PI for circle area calculation
#define PI 3.1415926535

//Recursion function that parses the a_node and stores product into givenImg
//Utilizes count to assure recursion does not pass farther than required
void parse_image(xmlNode * a_node, SVGimage* givenImg, int count);
//Recursion function used for additional parsing (similar function to parse_image) for groups
//Called by parse_image when encountering groups, then recursively called when groups encountered within groups
void group_parse(xmlNode * a_node, Group* givenGroup, int count);
//Recursion function that parses the givenGroup and stores the rectangles found into givenList
void add_additional_rects(Group * givenGroup, List* givenList);
//Recursion function that parses the givenGroup and stores the circles found into givenList
void add_additional_circs(Group * givenGroup, List* givenList);
//Recursion function that parses the givenGroup and stores the groups found into givenList
void add_additional_groups(Group * givenGroup, List* givenList);
//Recursion function that parses the givenGroup and stores the paths found into givenList
void add_additional_paths(Group * givenGroup, List* givenList);
//Recursion function that counts the additional attributes from the provided givenGroup
int count_additional_attributes(Group * givenGroup);
//Empty stub (to avoid logical errors)
void emptyStub();