#include "SVGParser.h"

/** Function to create an SVG object based on the contents of an SVG file.
 *@pre File name cannot be an empty string or NULL.
       File represented by this name must exist and must be readable.
 *@post Either:
        A valid SVGimage has been created and its address was returned
		or
		An error occurred, and NULL was returned
 *@return the pinter to the new struct or NULL
 *@param fileName - a string containing the name of the SVG file
**/
SVGimage* createSVGimage(char* fileName) {
    return;
}

/** Function to create a string representation of an SVG object.
 *@pre SVGimgage exists, is not null, and is valid
 *@post SVGimgage has not been modified in any way, and a string representing the SVG contents has been created
 *@return a string contaning a humanly readable representation of an SVG object
 *@param obj - a pointer to an SVG struct
**/
char* SVGimageToString(SVGimage* img) {
    return;
}

/** Function to delete image content and free all the memory.
 *@pre SVGimgage  exists, is not null, and has not been freed
 *@post SVSVGimgageG  had been freed
 *@return none
 *@param obj - a pointer to an SVG struct
**/
void deleteSVGimage(SVGimage* img) {

}

/* For the four "get..." functions below, make sure you return a list of opinters to the existing structs
 - do not allocate new structs.  They all share the same format, and only differ in the contents of the lists
 they return.

 *@pre SVGimgage exists, is not null, and has not been freed
 *@post SVGimgage has not been modified in any way
 *@return a newly allocated List of components.  While the List struct itself is new, the components in it are just pointers
  to the ones in the image.

 The list must me empty if the element is not found - do not return NULL

 *@param obj - a pointer to an SVG struct
 */

// Function that returns a list of all rectangles in the image.
List* getRects(SVGimage* img) {
    return;
}
// Function that returns a list of all circles in the image.
List* getCircles(SVGimage* img) {
    return;
}
// Function that returns a list of all groups in the image.
List* getGroups(SVGimage* img) {
    return;
}
// Function that returns a list of all paths in the image.
List* getPaths(SVGimage* img) {
    return;
}


/* For the four "num..." functions below, you need to search the SVG image for components  that match the search
  criterion.  You may wish to write some sort of a generic searcher fucntion that accepts an image, a predicate function,
  and a dummy search record as arguments.  We will discuss such search functions in class

 NOTE: For consistency, use the ceil() function to round the floats up to the nearest integer once you have computed
 the number you need.  See A1 Module 2 for details.

 *@pre SVGimgage exists, is not null, and has not been freed.  The search criterion is valid
 *@post SVGimgage has not been modified in any way
 *@return an int indicating how many objects matching the criterion are contained in the image
 *@param obj - a pointer to an SVG struct
 *@param 2nd - the second param depends on the function.  See details below
 */

// Function that returns the number of all rectangles with the specified area
int numRectsWithArea(SVGimage* img, float area) {
    return;
}
// Function that returns the number of all circles with the specified area
int numCirclesWithArea(SVGimage* img, float area) {
    return;
}
// Function that returns the number of all paths with the specified data - i.e. Path.data field
int numPathsWithdata(SVGimage* img, char* data) {
    return;
}
// Function that returns the number of all groups with the specified length - see A1 Module 2 for details
int numGroupsWithLen(SVGimage* img, int len) {
    return;
}

/*  Function that returns the total number of Attribute structs in the SVGimage - i.e. the number of Attributes
    contained in all otherAttributes lists in the structs making up the SVGimage
    *@pre SVGimgage  exists, is not null, and has not been freed.
    *@post SVGimage has not been modified in any way
    *@return the total length of all attribute structs in the SVGimage
    *@param obj - a pointer to an SVG struct
*/
int numAttr(SVGimage* img) {
    return;
}


/* ******************************* List helper functions  - MUST be implemented *************************** */

void deleteAttribute( void* data) {

}
char* attributeToString( void* data) {
    return;
}
int compareAttributes(const void *first, const void *second) {
    return;
}

void deleteGroup(void* data) {

}
char* groupToString( void* data) {
    return;
}
int compareGroups(const void *first, const void *second) {
    return;
}

void deleteRectangle(void* data) {

}
char* rectangleToString(void* data) {
    return;
}
int compareRectangles(const void *first, const void *second) {
    return;
}

void deleteCircle(void* data) {

}
char* circleToString(void* data) {
    return;
}
int compareCircles(const void *first, const void *second) {
    return;
}

void deletePath(void* data) {

}
char* pathToString(void* data) {
    return;
}
int comparePaths(const void *first, const void *second) {
    return;
}

/**
 * Simple main for testing of library
 */
int main(int argc, char **argv) {
    return 0;
}