#include <math.h>

#define PI 3.1415926535

void parse_image(xmlNode * a_node, SVGimage* givenImg, int count);
void group_parse(xmlNode * a_node, Group* givenGroup, int count);
void add_additional_rects(Group * givenGroup, List* givenList);
void add_additional_circs(Group * givenGroup, List* givenList);
void add_additional_groups(Group * givenGroup, List* givenList);
void add_additional_paths(Group * givenGroup, List* givenList);
int count_additional_attributes(Group * givenGroup);
void emptyStub();