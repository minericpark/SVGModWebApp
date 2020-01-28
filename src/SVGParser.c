/**
 * Name: Eric Minseo Park
 * Student Number: 1001018
 **/

#include "SVGParser.h"
#include "SVGHelper.h"

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

    SVGimage* newImg = NULL;

    if (fileName == NULL) {
        return NULL;
    }

    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    /*parse the file and get the DOM */
    doc = xmlReadFile(fileName, NULL, 0);

    if (doc == NULL) {
        printf("error: could not parse file %s\n", fileName);
        return NULL;
    }

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    if (root_element == NULL) {
        printf ("empty document\n");
        return NULL;
    }

    /*Check for namespace (if null, return NULL)*/
    if ((root_element->ns) == NULL) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return NULL;
    }

    /*Allocate SVGimage appropriately*/
    newImg = (SVGimage*)calloc(1, sizeof(SVGimage));
    newImg->namespace[0] = '\0';
    newImg->title[0] = '\0';
    newImg->description[0] = '\0';
    newImg->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    newImg->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
    newImg->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    newImg->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    newImg->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);

    /*Call helper function that parses and stores svg struct into newImg*/
    parse_image(root_element, newImg, 0);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    return newImg;
}

/** Function to create a string representation of an SVG object.
 *@pre SVGimgage exists, is not null, and is valid
 *@post SVGimgage has not been modified in any way, and a string representing the SVG contents has been created
 *@return a string contaning a humanly readable representation of an SVG object
 *@param obj - a pointer to an SVG struct
**/
char* SVGimageToString(SVGimage* img) {
    
    char* tmpStr;
    char* circStr;
    char* recStr;
    char* pathStr;
    char* groupStr;
    char* attrStr;
    char* nameStr = (char*)malloc(sizeof(char) * 256);
    char* titleStr = (char*)malloc(sizeof(char) * 256);
    char* descStr = (char*)malloc(sizeof(char) * 256);
    SVGimage* tmpImg;
    int len;
    int statLen;

    if (img == NULL) {
        return 0;
    }

    tmpImg = img;

    /*Initialize all strings of provided img*/
    circStr = toString(tmpImg->circles);
    recStr = toString(tmpImg->rectangles);
    pathStr = toString(tmpImg->paths);
    groupStr = toString(tmpImg->groups);
    attrStr = toString(tmpImg->otherAttributes);
    strncpy(nameStr, tmpImg->namespace, 256);
    strncpy(titleStr, tmpImg->title, 256);
    strncpy(descStr, tmpImg->description, 256);

    statLen = 256 * 3;
    len = strlen(circStr) + strlen(recStr) + strlen(pathStr) + strlen(groupStr) + strlen(attrStr) + statLen;
	tmpStr = (char*)malloc(sizeof(char)*len + 217);
	
    /*Compile all strings into tmpStr (to return)*/
	sprintf(tmpStr, "--------------------\nName: %s\nTitle: %s\nDescription: %s\n--------------------\nList of rects: %s\n--------------------\nList of circs: %s\n--------------------\nList of attributes: %s\n--------------------\nList of paths: %s\n--------------------\nList of groups: %s\n", nameStr, titleStr, descStr, recStr, circStr, attrStr, pathStr, groupStr);

    free(attrStr);
    free(circStr);
    free(recStr);
    free(pathStr);
    free(groupStr);
    free(nameStr);
    free(titleStr);
    free(descStr);
    return tmpStr;
}

/** Function to delete image content and free all the memory.
 *@pre SVGimgage  exists, is not null, and has not been freed
 *@post SVSVGimgageG  had been freed
 *@return none
 *@param obj - a pointer to an SVG struct
**/
void deleteSVGimage(SVGimage* img) {

    SVGimage* tmpImage;

    if (img == NULL) {
        return;
    }

    tmpImage = img;

    /*Call LinkedListAPI freeList function to free all lists and tmpImage*/
    freeList(tmpImage->otherAttributes);
    freeList(tmpImage->groups);
    freeList(tmpImage->circles);
    freeList(tmpImage->paths);
    freeList(tmpImage->rectangles);
    free(tmpImage);
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

    List* rects = NULL;
    List* additionalGroups = NULL;
    ListIterator rectIterator;
    ListIterator extraIterator;
    void* elem;
    void* elem2;

    if (img == NULL) {
        return NULL;
    }

    /*Initialize list appropiately and iterator*/
    rects = initializeList(&rectangleToString, &emptyStub, &compareRectangles);
    rectIterator = createIterator(img->rectangles);

    /*Insert rectangles found into temporary list*/
    while((elem = nextElement(&rectIterator)) != NULL){
		insertBack(rects, (Rectangle*) elem);
	}
    /*Check all other groups within img for other rectangles, and add them into temporary list*/
    additionalGroups = img->groups;
    extraIterator = createIterator(additionalGroups);
    while((elem2 = nextElement(&extraIterator)) != NULL){
        add_additional_rects((Group*)elem2, rects);
	}
    return rects;
}
// Function that returns a list of all circles in the image.
List* getCircles(SVGimage* img) {

    List* circs = NULL;
    List* additionalGroups = NULL;
    ListIterator circIterator;
    ListIterator extraIterator;
    void* elem;
    void* elem2;

    if (img == NULL) {
        return NULL;
    }

    /*Initialize list appropiately and iterator*/
    circs = initializeList(&circleToString, &emptyStub, &compareCircles);
    circIterator = createIterator(img->circles);

    /*Insert circles found into temporary list*/
    while((elem = nextElement(&circIterator)) != NULL){
		insertBack(circs, (Circle*) elem);
	}
    /*Check all other groups within img for other circles, and add them into temporary list*/
    additionalGroups = img->groups;
    extraIterator = createIterator(additionalGroups);
    while((elem2 = nextElement(&extraIterator)) != NULL){
        add_additional_circs((Group*)elem2, circs);
	}
    return circs;
}
// Function that returns a list of all groups in the image.
List* getGroups(SVGimage* img) {

    List* groups = NULL;
    List* additionalGroups = NULL;
    ListIterator groupIterator;
    ListIterator extraIterator;
    void* elem;
    void* elem2;

    if (img == NULL) {
        return NULL;
    }

    /*Initialize list appropiately and iterator*/
    groups = initializeList(&groupToString, &emptyStub, &compareGroups);
    groupIterator = createIterator(img->groups);

    /*Insert groups found into the temporary list*/
    while((elem = nextElement(&groupIterator)) != NULL){
        //printf ("got group\n");
		insertBack(groups, (Group*) elem);
	}
    /*Check all other groups within img for other groups, and add them into temporary list*/
    additionalGroups = img->groups;
    extraIterator = createIterator(additionalGroups);
    while((elem2 = nextElement(&extraIterator)) != NULL){
        add_additional_groups((Group*)elem2, groups);
	}
    return groups;
}
// Function that returns a list of all paths in the image.
List* getPaths(SVGimage* img) {

    List* paths = NULL;
    List* additionalGroups = NULL;
    ListIterator pathIterator;
    ListIterator extraIterator;
    void* elem;
    void* elem2;

    if (img == NULL) {
        return NULL;
    }

    /*Initialize list appropiately and iterator*/
    paths = initializeList(&pathToString, &emptyStub, &comparePaths);
    pathIterator = createIterator(img->paths);

    /*Insert paths found into the temporary list*/
    while((elem = nextElement(&pathIterator)) != NULL){
		insertBack(paths, (Path*) elem);
	}
    /*Check all other groups within img for other paths, and add them into temporary list*/
    additionalGroups = img->groups;
    extraIterator = createIterator(additionalGroups);
    while((elem2 = nextElement(&extraIterator)) != NULL){
        add_additional_paths((Group*)elem2, paths);
	}
    return paths;
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

    List* rects;
    ListIterator rectIterator;
    void* elem;
    int count = 0;
    float width;
    float height;

    if (img == NULL || area < 0) {
        return 0;
    }
    /*Initialize list and listiterator*/
    rects = getRects(img);
    rectIterator = createIterator(rects);
    /*Check all rectangles, count only those that match the area provided*/
    while((elem = nextElement(&rectIterator)) != NULL){
		//compare and count
        width = ((Rectangle*)elem)->width;
        height = ((Rectangle*)elem)->height;
        if (ceil(area) == ceil(width * height)) {
            count++;
        }
	}
    freeList(rects);

    return count;
}
// Function that returns the number of all circles with the specified area
int numCirclesWithArea(SVGimage* img, float area) {

    List* circs;
    ListIterator circIterator;
    void* elem;
    int count = 0;
    float radius;

    if (img == NULL || area < 0) {
        return 0;
    }
    /*Initialize list and listiterator*/
    circs = getCircles(img);
    circIterator = createIterator(circs);
    /*Check all circles, count only those that match the area provided*/
    while((elem = nextElement(&circIterator)) != NULL){
		//compare and count
        radius = ((Circle*)elem)->r;
        if (ceil(area) == ceil(pow(radius, 2) * PI)) {
            count++;
        }
	}
    freeList(circs);

    return count;
}
// Function that returns the number of all paths with the specified data - i.e. Path.data field
int numPathsWithdata(SVGimage* img, char* data) {

    List* paths;
    ListIterator pathIterator;
    void* elem;
    int count = 0;

    if (img == NULL || data == NULL) {
        return 0;
    }
    /*Initialize list and listiterator*/
    paths = getPaths(img);
    pathIterator = createIterator(paths);
    /*Check all paths, count only those that match the data provided*/
    while((elem = nextElement(&pathIterator)) != NULL){
		//compare and count
        if (strcmp((((Path*)elem)->data), data) == 0) {
            count++;
        }
	}
    freeList(paths);

    return count;
}
// Function that returns the number of all groups with the specified length - see A1 Module 2 for details
int numGroupsWithLen(SVGimage* img, int len) {

    List* groups;
    ListIterator groupIterator;
    void* elem;
    int count = 0;
    int size = 0;

    if (img == NULL || len < 0) {
        return 0;
    }
    /*Initialize list and listiterator*/
    groups = getGroups(img);
    groupIterator = createIterator(groups);
    /*Check all groups, count only those that match the count provided*/
    while((elem = nextElement(&groupIterator)) != NULL){
        //check size of group vs length
        size = getLength(((Group*)elem)->circles) + getLength(((Group*)elem)->groups) + getLength(((Group*)elem)->rectangles) + getLength(((Group*)elem)->paths);
        if (size == len) {
            count++;
        }
    }
    freeList(groups);

    return count;
}

/*  Function that returns the total number of Attribute structs in the SVGimage - i.e. the number of Attributes
    contained in all otherAttributes lists in the structs making up the SVGimage
    *@pre SVGimgage  exists, is not null, and has not been freed.
    *@post SVGimage has not been modified in any way
    *@return the total length of all attribute structs in the SVGimage
    *@param obj - a pointer to an SVG struct
*/
int numAttr(SVGimage* img) {
    
    ListIterator attriIterator;
    ListIterator attriIteratorC;
    ListIterator attriIteratorR;
    ListIterator attriIteratorP;
    ListIterator extraIterator;
    List* additionalGroups;
    void* elem;
    void* elem2;
    int count = 0;

    if (img == NULL) {
        return 0;
    }

    /*Initialize all iterators*/
    attriIterator = createIterator(img->otherAttributes);
    attriIteratorC = createIterator(img->circles);
    attriIteratorR = createIterator(img->rectangles);
    attriIteratorP = createIterator(img->paths);

    /*Check for elements, increase count when found through img, circles, rectangles, paths, and other groups*/
    while((elem = nextElement(&attriIterator)) != NULL){
		count++;
	}
    while((elem = nextElement(&attriIteratorC)) != NULL) {
        extraIterator = createIterator(((Circle*)elem)->otherAttributes);
        while ((elem2 = nextElement(&extraIterator)) != NULL) {
            count++;
        }
    }
    while((elem = nextElement(&attriIteratorR)) != NULL) {
        extraIterator = createIterator(((Rectangle*)elem)->otherAttributes);
        while ((elem2 = nextElement(&extraIterator)) != NULL) {
            count++;
        }
    }
    while((elem = nextElement(&attriIteratorP)) != NULL) {
        extraIterator = createIterator(((Path*)elem)->otherAttributes);
        while ((elem2 = nextElement(&extraIterator)) != NULL) {
            count++;
        }
    }
    additionalGroups = img->groups;
    extraIterator = createIterator(additionalGroups);
    while((elem2 = nextElement(&extraIterator)) != NULL){
        /*Call recursive function that checks attributes of group*/
        count += count_additional_attributes((Group*)elem2);
	}

    return count;
}


/* ******************************* List helper functions  - MUST be implemented *************************** */

/* Free's attribute appropriately by freeing the name, value, then the attribute */
void deleteAttribute( void* data) {
    
    Attribute* tmpAttribute;
	
	if (data == NULL){
		return;
	}
	
	tmpAttribute = (Attribute*)data;
	
    /*Free the attribute*/
	free(tmpAttribute->name);
	free(tmpAttribute->value);
	free(tmpAttribute);
}
/* Turns attribute into string form */
char* attributeToString( void* data) {

    char* tmpStr;
	Attribute* tmpAttribute;
	int len = 0;
	
	if (data == NULL){
		return NULL;
	}
	
	tmpAttribute = (Attribute*)data;

    /* Length of the string is: length of name + value */
    len = strlen(tmpAttribute->name) + strlen(tmpAttribute->value);
	tmpStr = (char*)malloc(sizeof(char)*len + 15);
	
    /*Store string into tmpString*/
	sprintf(tmpStr, "Name: %s Value: %s", tmpAttribute->name, tmpAttribute->value);
	
	return tmpStr;
}
/* (Stub for API) Compares attributes between first and second (using their value) */
int compareAttributes(const void *first, const void *second) {

    Attribute* tmpAttribute1;
	Attribute* tmpAttribute2;
	
	if (first == NULL || second == NULL){
		return 0;
	}
	
	tmpAttribute1 = (Attribute*)first;
	tmpAttribute2 = (Attribute*)second;
	
    /*Not necessary, but compares values between attributes*/
	return strcmp((char*)tmpAttribute1->value, (char*)tmpAttribute2->value);
}

/* Free's group appropriately by calling freeList for each list within group, then the group */
void deleteGroup(void* data) {

    Group* tmpGroup;

    if (data == NULL) {
        return;
    }

    tmpGroup = (Group*)data;

    /*Free the group*/
    freeList(tmpGroup->rectangles);
    freeList(tmpGroup->circles);
    freeList(tmpGroup->paths);
    freeList(tmpGroup->groups);
    freeList(tmpGroup->otherAttributes);
	free(tmpGroup);
}
/* Turns group into string form */
char* groupToString( void* data) {
       
    char* tmpStr;
    char* circStr;
    char* recStr;
    char* pathStr;
    char* attriStr;
    char* groupStr;
	Group* tmpGroup;
	int circLen;
    int recLen;
    int pathLen;
    int attriLen;
    int groupLen;
	
	if (data == NULL){
		return NULL;
	}
	
	tmpGroup = (Group*)data;

    /* Length of the string is: length of name + value */
	/*Initialize all strings required*/
    circStr = toString(tmpGroup->circles);
    recStr = toString(tmpGroup->rectangles);
    pathStr = toString(tmpGroup->paths);
    attriStr = toString(tmpGroup->otherAttributes);
    groupStr = toString(tmpGroup->groups);
    circLen = strlen(circStr);
    recLen = strlen(recStr);
    pathLen = strlen(pathStr);
    attriLen = strlen(attriStr);
    groupLen = strlen(groupStr);

	tmpStr = (char*)malloc(sizeof(char)*(circLen + recLen + pathLen + attriLen + groupLen) + 95);
	
    /*Store compiled string into tmpStr*/
    sprintf(tmpStr, "--------------------\nCircle List: %s\nRectangle List: %s\nPath List: %s\nAttribute List: %s\nGroup List: %s\n", circStr, recStr, pathStr, attriStr, groupStr);	
	free(circStr);
    free(recStr);
    free(pathStr);
    free(attriStr);
    free(groupStr);
    return tmpStr;
}
/*Stub - Only required for API use*/
int compareGroups(const void *first, const void *second) {
    return 0;
}


void deleteRectangle(void* data) {

    Rectangle* tmpRectangle;

    if (data == NULL) {
        return;
    }

    tmpRectangle = (Rectangle*)data;

    /*Free the rectangle*/
    freeList(tmpRectangle->otherAttributes);
	free(tmpRectangle);
}

char* rectangleToString(void* data) {
           
    char* tmpStr;
    char* listStr;
	Rectangle* tmpRectangle;
	int xLen;
    int yLen;
    int widthLen;
    int heightLen;
    int unitLen;
    int attriLen;
	
	if (data == NULL){
		return NULL;
	}
	
	tmpRectangle = (Rectangle*)data;
    /*Initialize all required values*/
    xLen = 4;
    yLen = 4;
    widthLen = 4;
    heightLen = 4;
    unitLen = 50;
    listStr = toString(tmpRectangle->otherAttributes);
    attriLen = strlen(listStr);

    tmpStr = (char*)malloc(sizeof(char)*(xLen + yLen + widthLen + heightLen + unitLen + attriLen) + 44);
	
    /*Store compiled string into tmpStr*/
    sprintf(tmpStr, "x: %f\ny: %f\nwidth: %f\nheight: %f\nunit: %s\nattribute list: %s\n", tmpRectangle->x, tmpRectangle->y, tmpRectangle->width, tmpRectangle->height, tmpRectangle->units, listStr);	
	free(listStr);
    return tmpStr;
}
/*Stub - Only required for API use*/
int compareRectangles(const void *first, const void *second) {
    return 0;
}


void deleteCircle(void* data) {

    Circle* tmpCircle;

    if (data == NULL) {
        return;
    }

    tmpCircle = (Circle*)data;
    /*Free the circle*/
    freeList(tmpCircle->otherAttributes);
    free(tmpCircle);
}

char* circleToString(void* data) {
               
    char* tmpStr;
    char* listStr;
	Circle* tmpCircle;
	int cxLen;
    int cyLen;
    int rLen;
    int unitLen;
    int attriLen;
	
	if (data == NULL){
		return NULL;
	}
	
	tmpCircle = (Circle*)data;
    /*Initialize all required variables*/
    cxLen = 4;
    cyLen = 4;
    rLen = 4;
    unitLen = 50;
    listStr = toString(tmpCircle->otherAttributes);
    attriLen = strlen(listStr);

    tmpStr = (char*)malloc(sizeof(char)*(attriLen) + cxLen + cyLen + rLen + unitLen + 34);
	/*Store compiled string into tmpStr*/
    sprintf(tmpStr, "cx: %f\ncy: %f\nr: %f\nunit: %s\nattribute list: %s\n", tmpCircle->cx, tmpCircle->cy, tmpCircle->r, tmpCircle->units, listStr);	
	free(listStr);
    return tmpStr;
}
/*Stub - Only required for API use*/
int compareCircles(const void *first, const void *second) {
    return 0;
}


void deletePath(void* data) {

    Path* tmpPath;

    if (data == NULL) {
        return;
    }

    tmpPath = (Path*)data;
    /*Free the path*/
    freeList(tmpPath->otherAttributes);
    free(tmpPath->data);
    free(tmpPath);
}

char* pathToString(void* data) {
        
    char* tmpStr;
	Path* tmpPath;
	int dataLen;
    char* listStr;
    int attriLen;
	
	if (data == NULL){
		return NULL;
	}
	
	tmpPath = (Path*)data;

    /* Length of the string is: length of name + value */
	listStr = toString(tmpPath->otherAttributes);
    attriLen = strlen(listStr);
	dataLen = strlen(tmpPath->data);
	tmpStr = (char*)malloc(sizeof(char)*(dataLen + attriLen) + 25);
	/*Store compiled string into tmpStr*/
	sprintf(tmpStr, "data: %s\nattribute list: %s\n", tmpPath->data, listStr);
	free(listStr);

	return tmpStr;
}
/*Stub - Only required for API use*/
int comparePaths(const void *first, const void *second) {
    return 0;
}

/*----------------------------------------------------------------*/
/*Tree traversal helpers*/
void parse_image(xmlNode * a_node, SVGimage* givenImg, int count)
{
    SVGimage* tmpImg;
    xmlNode* cur_node = NULL;
    xmlNode* value;
    xmlAttr* attr;
    char* attrName;
    char* cont;
    char* unit;

    tmpImg = givenImg;

    /*Traverse through tree appropriately*/
    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            //printf("node type: Element, name: %s\n", cur_node->name);
            if (xmlStrcasecmp(cur_node->name, (const xmlChar*) "svg") == 0) {
                //found svg
                //printf ("Found svg\n");
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                /*Parse namespace*/
                if (cur_node->ns != NULL) {
                    //printf ("namespace exists\n");
                    strncpy(tmpImg->namespace, (char*) cur_node->ns->href, 256);
                    printf ("%s\n", tmpImg->namespace);
                    if (strlen((char*)(cur_node->ns)->href) >= 256) {
                        (tmpImg->namespace)[255] = '\0';
                    }
                }
                /*Parse attributes*/
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (cont != NULL && attrName != NULL) {
                        Attribute* newAttr = (Attribute*)calloc(1, sizeof(Attribute));
                        //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                        newAttr->value = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                        newAttr->name = (char*)malloc(sizeof(char) * strlen(attrName) + 1);
                        strcpy(newAttr->value, cont);
                        strcpy(newAttr->name, attrName);
                        insertBack(tmpImg->otherAttributes, newAttr);
                    }
                }
            } else if (xmlStrcasecmp(cur_node->name, (const xmlChar*) "title") == 0) {
                //found title
                //printf ("Found title\n");
                if (cur_node->children->content != NULL ){
                    //printf("  content: %s\n", cur_node->children->content);
                    strncpy(tmpImg->title, (char *)(cur_node->children)->content, 256);
                    if (strlen((char*)(cur_node->children)->content) >= 256) {
                        (tmpImg->title)[255] = '\0';
                    }
                }
            } else if (xmlStrcasecmp(cur_node->name, (const xmlChar*) "desc") == 0) {
                //found description   
                //printf ("Found description\n");
                if (cur_node->children->content != NULL ){
                    //printf("  content: %s\n", cur_node->children->content);
                    strncpy(tmpImg->description, (char *)(cur_node->children)->content, 256);
                    if (strlen((char*)(cur_node->children)->content) >= 256) {
                        (tmpImg->description)[255] = '\0';
                    }
                }
            } else if (xmlStrcasecmp(cur_node->name, (const xmlChar*) "g") == 0) {
                //printf ("Found group\n");
                /*Parse groups*/
                Group* tmpGroup = (Group*)calloc(1, sizeof(Group));
                tmpGroup->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                tmpGroup->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
                tmpGroup->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
                tmpGroup->paths = initializeList(&pathToString, &deletePath, &comparePaths);
                tmpGroup->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                /*Parse attributes*/
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (cont != NULL && attrName != NULL) {
                        Attribute* newAttr = (Attribute*)calloc(1, sizeof(Attribute));
                        //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                        newAttr->value = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                        newAttr->name = (char*)malloc(sizeof(char) * strlen(attrName) + 1);
                        strcpy(newAttr->value, cont);
                        strcpy(newAttr->name, attrName);
                        insertBack(tmpGroup->otherAttributes, newAttr);
                    }
                }
                /*Call recursion function that will parse through group*/
                group_parse(cur_node->children, tmpGroup, 0);
                insertBack(tmpImg->groups, tmpGroup);
                
            } else if (xmlStrcasecmp(cur_node->name, (const xmlChar*) "rect") == 0) {
                //printf ("Found rectangle\n");
                Rectangle* tmpRectangle = (Rectangle*)calloc(1, sizeof(Rectangle));
                tmpRectangle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                /*Parse rectangle*/
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "x") == 0) {
                        //printf ("found x\n");
                        tmpRectangle->x = strtof(cont, &unit);
                        if (strlen(unit) > 0) { 
                            strncpy(tmpRectangle->units, unit, 50);
                            if (strlen(unit) >= 50) {
                                (tmpRectangle->units)[49] = '\0';
                            }
                            unit = '\0';
                        }
                    } else if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "y") == 0) {
                        //printf ("found y\n");
                        tmpRectangle->y = strtof(cont, &unit);
                        if (strlen(unit) > 0) { 
                            strncpy(tmpRectangle->units, unit, 50);
                            if (strlen(unit) >= 50) {
                                (tmpRectangle->units)[49] = '\0';
                            }
                            unit = '\0';
                        }
                    } else if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "width") == 0) {
                        //printf ("found width\n");
                        if ((float*)cont >= 0) {
                            tmpRectangle->width = strtof(cont, &unit);
                            if (strlen(unit) > 0) { 
                                strncpy(tmpRectangle->units, unit, 50);
                                if (strlen(unit) >= 50) {
                                    (tmpRectangle->units)[49] = '\0';
                                }
                                unit = '\0';
                            }
                        }
                    } else if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "height") == 0) {
                        //printf ("found height\n");
                        if ((float*)cont >= 0) {
                            tmpRectangle->height = strtof(cont, &unit);
                            if (strlen(unit) > 0) { 
                                strncpy(tmpRectangle->units, unit, 50);
                                if (strlen(unit) >= 50) {
                                    (tmpRectangle->units)[49] = '\0';
                                }
                                unit = '\0';
                            }
                        }
                    } else {
                        //printf ("found other attribute");
                        if (cont != NULL && attrName != NULL) {
                            Attribute* newAttr = (Attribute*)calloc(1, sizeof(Attribute));
                            newAttr->value = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                            newAttr->name = (char*)malloc(sizeof(char) * strlen(attrName) + 1);
                            strcpy(newAttr->value, cont);
                            strcpy(newAttr->name, attrName);
                            insertBack(tmpRectangle->otherAttributes, newAttr);
                        }
                    }
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
                insertBack(tmpImg->rectangles, tmpRectangle);
            } else if (xmlStrcasecmp(cur_node->name, (const xmlChar*) "path") == 0) {
                //printf ("Found path\n");
                Path* tmpPath = (Path*)calloc(1, sizeof(Path));
                tmpPath->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                /*Parse path*/
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "d") == 0) {
                        //printf ("found data\n");
                        if (cont != NULL) {
                            tmpPath->data = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                            strcpy(tmpPath->data, cont);
                        } else {
                            deletePath(tmpPath);
                            break;
                        }
                    } else {
                        //printf ("found other attribute");
                        if (cont != NULL && attrName != NULL) {
                            Attribute* newAttr = (Attribute*)calloc(1, sizeof(Attribute));
                            newAttr->value = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                            newAttr->name = (char*)malloc(sizeof(char) * strlen(attrName) + 1);
                            strcpy(newAttr->value, cont);
                            strcpy(newAttr->name, attrName);
                            insertBack(tmpPath->otherAttributes, newAttr);
                        }
                    }
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
                if (tmpPath != NULL) {
                    insertBack(tmpImg->paths, tmpPath);
                }
            } else if (xmlStrcasecmp(cur_node->name, (const xmlChar*) "circle") == 0) {
                //printf ("found circle\n");
                Circle* tmpCircle = (Circle*)calloc(1, sizeof(Circle));
                tmpCircle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                /*Parse circle*/
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "cx") == 0) {
                        //printf ("found cx\n");
                        tmpCircle->cx = strtof(cont, &unit);
                        if (strlen(unit) > 0) {
                            strncpy(tmpCircle->units, unit, 50);
                            if (strlen(unit) >= 50) {
                                (tmpCircle->units)[49] = '\0';
                            }   
                            unit = '\0';
                        }
                    } else if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "cy") == 0) {
                        //printf ("found cy\n");
                        tmpCircle->cy = strtof(cont, &unit);
                        if (strlen(unit) > 0) {
                            strncpy(tmpCircle->units, unit, 50);
                            if (strlen(unit) >= 50) {
                                (tmpCircle->units)[49] = '\0';
                            }   
                            unit = '\0';
                        }
                    } else if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "r") == 0) {
                        //printf ("found r\n");
                        if (atof(cont) >= 0) {
                            tmpCircle->r = strtof(cont, &unit);
                            if (strlen(unit) > 0) {
                                strncpy(tmpCircle->units, unit, 50);
                                if (strlen(unit) >= 50) {
                                    (tmpCircle->units)[49] = '\0';
                                }   
                                unit = '\0';
                            }
                        }
                    } else {
                        //printf ("found other attribute");
                        if (cont != NULL && attrName != NULL) {
                            Attribute* newAttr = (Attribute*)calloc(1, sizeof(Attribute));
                            newAttr->value = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                            newAttr->name = (char*)malloc(sizeof(char) * strlen(attrName) + 1);
                            strcpy(newAttr->value, cont);
                            strcpy(newAttr->name, attrName);
                            insertBack(tmpCircle->otherAttributes, newAttr);
                        }
                    }
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
                insertBack(tmpImg->circles, tmpCircle);
            }
        }
        if (count < 1) {
            parse_image(cur_node->children, tmpImg, 1);
        }
    }
}

/*Similar to parse_image, similar structure and similar functions*/
void group_parse (xmlNode *a_node, Group* givenGroup, int count) {

    Group* tmpGroup;
    xmlNode* cur_node = NULL;
    xmlNode* value;
    xmlAttr* attr;
    char* attrName;
    char* cont;
    char* unit;

    tmpGroup = givenGroup;

    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (xmlStrcasecmp(cur_node->name, (const xmlChar*) "g") == 0) {
                //printf ("Found group\n");
                Group* tmpGroup2 = (Group*)calloc(1, sizeof(Group));
                tmpGroup2->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                tmpGroup2->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
                tmpGroup2->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
                tmpGroup2->paths = initializeList(&pathToString, &deletePath, &comparePaths);
                tmpGroup2->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (cont != NULL && attrName != NULL) {
                        Attribute* newAttr = (Attribute*)calloc(1, sizeof(Attribute));
                         //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                        newAttr->value = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                        newAttr->name = (char*)malloc(sizeof(char) * strlen(attrName) + 1);
                        strcpy(newAttr->value, cont);
                        strcpy(newAttr->name, attrName);
                        insertBack(tmpGroup2->otherAttributes, newAttr);
                    }
                }
                //printf("recursion called\n");
                group_parse(cur_node->children, tmpGroup2, 0);
                insertBack(tmpGroup->groups, tmpGroup2);
                //printf ("group inserted\n");
                count = 1;
            }
            else if (xmlStrcasecmp(cur_node->name, (const xmlChar*) "rect") == 0) {
                //printf ("Found rectangle\n");
                Rectangle* tmpRectangle = (Rectangle*)calloc(1, sizeof(Rectangle));
                tmpRectangle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "x") == 0) {
                        //printf ("found x\n");
                        tmpRectangle->x = strtof(cont, &unit);
                        if (strlen(unit) > 0) {
                            strncpy(tmpRectangle->units, unit, 50);
                            if (strlen(unit) >= 50) {
                                (tmpRectangle->units)[49] = '\0';
                            }  
                            unit = '\0';           
                        }
                    } else if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "y") == 0) {
                        //printf ("found y\n");
                        tmpRectangle->y = strtof(cont, &unit);
                        if (strlen(unit) > 0) {
                            strncpy(tmpRectangle->units, unit, 50);
                            if (strlen(unit) >= 50) {
                                (tmpRectangle->units)[49] = '\0';
                            }    
                            unit = '\0';         
                        }
                    } else if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "width") == 0) {
                        //printf ("found width\n");
                        if (atof(cont) >= 0) {
                            tmpRectangle->width = strtof(cont, &unit);
                            if (strlen(unit) > 0) {
                                strncpy(tmpRectangle->units, unit, 50);
                                if (strlen(unit) >= 50) {
                                    (tmpRectangle->units)[49] = '\0';
                                }  
                                unit = '\0';           
                            }               
                        }
                    } else if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "height") == 0) {
                        //printf ("found height\n");
                        if (atof(cont) >= 0) {
                            tmpRectangle->height = strtof(cont, &unit);
                            if (strlen(unit) > 0) {
                                strncpy(tmpRectangle->units, unit, 50);
                                if (strlen(unit) >= 50) {
                                    (tmpRectangle->units)[49] = '\0';
                                }  
                                unit = '\0';           
                            }    
                        }
                    } else {
                        //printf ("found other attribute");
                        if (cont != NULL && attrName != NULL) {
                            Attribute* newAttr = (Attribute*)calloc(1, sizeof(Attribute));
                            newAttr->value = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                            newAttr->name = (char*)malloc(sizeof(char) * strlen(attrName) + 1);
                            strcpy(newAttr->value, cont);
                            strcpy(newAttr->name, attrName);
                            insertBack(tmpRectangle->otherAttributes, newAttr);
                        }
                    }
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
                insertBack(tmpGroup->rectangles, tmpRectangle);
            } else if (xmlStrcasecmp(cur_node->name, (const xmlChar*) "path") == 0) {
                //printf ("Found path\n");
                Path* tmpPath = (Path*)calloc(1, sizeof(Path));
                tmpPath->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "d") == 0) {
                        //printf ("found data\n");
                        if (cont != NULL) {
                            tmpPath->data = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                            strcpy(tmpPath->data, cont);
                        } else {
                            deletePath(tmpPath);
                            break;
                        }
                    } else {
                        //printf ("found other attribute");
                        if (cont != NULL && attrName != NULL) {
                            Attribute* newAttr = (Attribute*)calloc(1, sizeof(Attribute));
                            newAttr->value = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                            newAttr->name = (char*)malloc(sizeof(char) * strlen(attrName) + 1);
                            strcpy(newAttr->value, cont);
                            strcpy(newAttr->name, attrName);
                            insertBack(tmpPath->otherAttributes, newAttr);
                        }
                    }
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
                insertBack(tmpGroup->paths, tmpPath);
            } else if (xmlStrcasecmp(cur_node->name, (const xmlChar*) "circle") == 0) {
                //printf ("found circle\n");
                Circle* tmpCircle = (Circle*)calloc(1, sizeof(Circle));
                tmpCircle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "cx") == 0) {
                        //printf ("found cx\n");
                        tmpCircle->cx = strtof(cont, &unit);
                        if (strlen(unit) > 0) {
                            strncpy(tmpCircle->units, unit, 50);
                            if (strlen(unit) >= 50) {
                                (tmpCircle->units)[49] = '\0';
                            }   
                            unit = '\0';
                        }
                    } else if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "cy") == 0) {
                        //printf ("found cy\n");
                        tmpCircle->cy = strtof(cont, &unit);
                        if (strlen(unit) > 0) {
                            strncpy(tmpCircle->units, unit, 50);
                            if (strlen(unit) >= 50) {
                                (tmpCircle->units)[49] = '\0';
                            }   
                            unit = '\0';
                        }
                    } else if (xmlStrcasecmp((const xmlChar*) attrName, (const xmlChar*) "r") == 0) {
                        //printf ("found r\n");
                        if (atof(cont) >= 0) {
                            tmpCircle->r = strtof(cont, &unit);
                            if (strlen(unit) > 0) {
                                strncpy(tmpCircle->units, unit, 50);
                                if (strlen(unit) >= 50) {
                                    (tmpCircle->units)[49] = '\0';
                                }   
                                unit = '\0';
                            }
                        }
                    } else {
                        //printf ("found other attribute");
                        if (cont != NULL && attrName != NULL) {
                            Attribute* newAttr = (Attribute*)calloc(1, sizeof(Attribute));
                            newAttr->value = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                            newAttr->name = (char*)malloc(sizeof(char) * strlen(attrName) + 1);
                            strcpy(newAttr->value, cont);
                            strcpy(newAttr->name, attrName);
                            insertBack(tmpCircle->otherAttributes, newAttr);
                        }
                    }
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
                insertBack(tmpGroup->circles, tmpCircle);
            }
        }
        if (count < 1) {
            //printf ("recurses one iteration\n");
            group_parse(cur_node->children, tmpGroup, 1);
        }
    }

}

void add_additional_rects(Group * givenGroup, List * givenList) {
    ListIterator extraIterator;
    ListIterator rectIterator = createIterator(givenGroup->rectangles);
    void* elem;
    void* elem2;

    /*Check additional rectangles within group, and add them, then check for groups within the group*/
    while((elem = nextElement(&rectIterator)) != NULL){
        insertBack(givenList, (Rectangle*) elem);
	}
    extraIterator = createIterator(givenGroup->groups);
    while ((elem2 = nextElement(&extraIterator)) != NULL) {
        add_additional_rects((Group*) elem2, givenList);
    }
}

void add_additional_circs(Group * givenGroup, List * givenList) {
    ListIterator extraIterator;
    ListIterator circIterator = createIterator(givenGroup->circles);
    void* elem;
    void* elem2;


    /*Check additional circles within group, and add them, then check for groups within the group*/
    while((elem = nextElement(&circIterator)) != NULL){
        insertBack(givenList, (Circle*) elem);
	}
    extraIterator = createIterator(givenGroup->groups);
    while ((elem2 = nextElement(&extraIterator)) != NULL) {
        add_additional_circs((Group*) elem2, givenList);
    }
}

void add_additional_groups(Group * givenGroup, List * givenList) {
    ListIterator extraIterator;
    ListIterator groupIterator = createIterator(givenGroup->groups);
    void* elem;
    void* elem2;

    /*Check additional groups within group, and add them, then check for groups within the group*/
    while((elem = nextElement(&groupIterator)) != NULL){
        //printf ("got group\n");
        insertBack(givenList, (Group*) elem);
	}
    extraIterator = createIterator(givenGroup->groups);
    while ((elem2 = nextElement(&extraIterator)) != NULL) {
        //printf ("recursion called\n");
        add_additional_groups((Group*) elem2, givenList);
    }
}

void add_additional_paths(Group * givenGroup, List * givenList) {
    ListIterator extraIterator;
    ListIterator pathIterator = createIterator(givenGroup->paths);
    void* elem;
    void* elem2;

    /*Check additional paths within group, and add them, then check for groups within the group*/
    while((elem = nextElement(&pathIterator)) != NULL){
        insertBack(givenList, (Path*) elem);
	}
    extraIterator = createIterator(givenGroup->groups);
    while ((elem2 = nextElement(&extraIterator)) != NULL) {
        add_additional_paths((Group*) elem2, givenList);
    }
}

int count_additional_attributes(Group * givenGroup) {
    ListIterator extraIterator;
    ListIterator attriIterator = createIterator(givenGroup->otherAttributes);
    ListIterator attriIteratorC = createIterator(givenGroup->circles);
    ListIterator attriIteratorR = createIterator(givenGroup->rectangles);
    ListIterator attriIteratorP = createIterator(givenGroup->paths);
    void* elem;
    void* elem2;
    int count = 0;

    /*Check additional attributes within group, and add them, then check for groups within the group*/
    while((elem = nextElement(&attriIterator)) != NULL){
        count++;
	}
    while((elem = nextElement(&attriIterator)) != NULL){
		count++;
	}
    while((elem = nextElement(&attriIteratorC)) != NULL) {
        extraIterator = createIterator(((Circle*)elem)->otherAttributes);
        while ((elem2 = nextElement(&extraIterator)) != NULL) {
            count++;
        }
    }
    while((elem = nextElement(&attriIteratorR)) != NULL) {
        extraIterator = createIterator(((Rectangle*)elem)->otherAttributes);
        while ((elem2 = nextElement(&extraIterator)) != NULL) {
            count++;
        }
    }
    while((elem = nextElement(&attriIteratorP)) != NULL) {
        extraIterator = createIterator(((Path*)elem)->otherAttributes);
        while ((elem2 = nextElement(&extraIterator)) != NULL) {
            count++;
        }
    }
    extraIterator = createIterator(givenGroup->groups);
    while ((elem2 = nextElement(&extraIterator)) != NULL) {
        /*Call recursion function that counts additional attributes again*/
        count += count_additional_attributes((Group*) elem2);
    }
    return count;
}

void emptyStub() {
    /*Necessary for list API and to prevent logical issues*/
}

