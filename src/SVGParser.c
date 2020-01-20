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

    //printf("%s\n", fileName);

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

    newImg = (SVGimage*)malloc(sizeof(SVGimage));
    newImg->namespace[0] = '\0';
    newImg->title[0] = '\0';
    newImg->description[0] = '\0';
    newImg->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    newImg->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
    newImg->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    newImg->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    newImg->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);

    parse_image(root_element, newImg);

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

    tmpImg = img;

    circStr = toString(tmpImg->circles);
    recStr = toString(tmpImg->rectangles);
    pathStr = toString(tmpImg->paths);
    groupStr = toString(tmpImg->groups);
    attrStr = toString(tmpImg->otherAttributes);
    strcpy(nameStr, tmpImg->namespace);
    strcpy(titleStr, tmpImg->title);
    strcpy(descStr, tmpImg->description);

    statLen = 256 * 3;
    len = strlen(circStr) + strlen(recStr) + strlen(pathStr) + strlen(groupStr) + strlen(attrStr) + statLen;
	tmpStr = (char*)malloc(sizeof(char)*len + 97);
	
	sprintf(tmpStr, "Name: %s\nTitle: %s\nDescription: %s\nList of rects: %s\nList of circs: %s\nList of attributes: %s\nList of paths: %s\nList of groups: %s\n", nameStr, titleStr, descStr, recStr, circStr, attrStr, pathStr, groupStr);

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

    tmpImage = img;

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

    return img->rectangles;
}
// Function that returns a list of all circles in the image.
List* getCircles(SVGimage* img) {

    return img->circles;
}
// Function that returns a list of all groups in the image.
List* getGroups(SVGimage* img) {

    return img->groups;
}
// Function that returns a list of all paths in the image.
List* getPaths(SVGimage* img) {

    return img->paths;
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
    return 0;
}
// Function that returns the number of all circles with the specified area
int numCirclesWithArea(SVGimage* img, float area) {
    return 0;
}
// Function that returns the number of all paths with the specified data - i.e. Path.data field
int numPathsWithdata(SVGimage* img, char* data) {
    return 0;
}
// Function that returns the number of all groups with the specified length - see A1 Module 2 for details
int numGroupsWithLen(SVGimage* img, int len) {
    return 0;
}

/*  Function that returns the total number of Attribute structs in the SVGimage - i.e. the number of Attributes
    contained in all otherAttributes lists in the structs making up the SVGimage
    *@pre SVGimgage  exists, is not null, and has not been freed.
    *@post SVGimage has not been modified in any way
    *@return the total length of all attribute structs in the SVGimage
    *@param obj - a pointer to an SVG struct
*/
int numAttr(SVGimage* img) {
    
    return 0;
}


/* ******************************* List helper functions  - MUST be implemented *************************** */

/* Free's attribute appropriately by freeing the name, value, then the attribute */
void deleteAttribute( void* data) {
    
    Attribute* tmpAttribute;
	
	if (data == NULL){
		return;
	}
	
	tmpAttribute = (Attribute*)data;
	
	free(tmpAttribute->name);
	free(tmpAttribute->value);
	free(tmpAttribute);
}
/* Turns attribute into string form */
char* attributeToString( void* data) {

    char* tmpStr;
	Attribute* tmpAttribute;
	int len;
	
	if (data == NULL){
		return NULL;
	}
	
	tmpAttribute = (Attribute*)data;

    /* Length of the string is: length of name + value */
	
	len = strlen(tmpAttribute->name) + strlen(tmpAttribute->value);
	tmpStr = (char*)malloc(sizeof(char)*len + 15);
	
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
	
	return strcmp((char*)tmpAttribute1->value, (char*)tmpAttribute2->value);
}

/* Free's group appropriately by calling freeList for each list within group, then the group */
void deleteGroup(void* data) {

    Group* tmpGroup;

    if (data == NULL) {
        return;
    }

    tmpGroup = (Group*)data;

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

	tmpStr = (char*)malloc(sizeof(char)*(circLen + recLen + pathLen + attriLen + groupLen) + 74);
	
    sprintf(tmpStr, "Circle List: %s\nRectangle List: %s\nPath List: %s\nAttribute List: %s\nGroup List: %s\n", circStr, recStr, pathStr, attriStr, groupStr);	
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
    
    xLen = 4;
    yLen = 4;
    widthLen = 4;
    heightLen = 4;
    unitLen = 50;
    listStr = toString(tmpRectangle->otherAttributes);
    attriLen = strlen(listStr);

    tmpStr = (char*)malloc(sizeof(char)*(xLen + yLen + widthLen + heightLen + unitLen + attriLen) + 44);
	
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
    
    cxLen = 4;
    cyLen = 4;
    rLen = 4;
    unitLen = 50;
    listStr = toString(tmpCircle->otherAttributes);
    attriLen = strlen(listStr);

    tmpStr = (char*)malloc(sizeof(char)*(attriLen) + cxLen + cyLen + rLen + unitLen + 34);
	
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
	
	sprintf(tmpStr, "data: %s\nattribute list: %s\n", tmpPath->data, listStr);
	free(listStr);

	return tmpStr;
}
/*Stub - Only required for API use*/
int comparePaths(const void *first, const void *second) {
    return 0;
}

/*Tree traversal helpers*/
void parse_image(xmlNode * a_node, SVGimage* givenImg)
{
    SVGimage* tmpImg;
    xmlNode *cur_node = NULL;
    xmlNode *value;
    xmlAttr *attr;
    char* attrName;
    char*cont;

    tmpImg = givenImg;

    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            //printf("node type: Element, name: %s\n", cur_node->name);
            if (xmlStrcmp(cur_node->name, (const xmlChar*) "svg") == 0) {
                //found svg
                //printf ("Found svg\n");
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                if (cur_node->ns != NULL) {
                    //printf ("namespace exists\n");
                    strcpy(tmpImg->namespace, (char*) cur_node->ns->href);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    Attribute* newAttr = (Attribute*)malloc(sizeof(Attribute));
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                    newAttr->value = cont;
                    newAttr->name = attrName;
                    insertBack(tmpImg->otherAttributes, newAttr);
                }
            } else if (xmlStrcmp(cur_node->name, (const xmlChar*) "title") == 0) {
                //found title
                //printf ("Found title\n");
                if (cur_node->children->content != NULL ){
                    //printf("  content: %s\n", cur_node->children->content);
                    strcpy(tmpImg->title, (char *)(cur_node->children)->content);
                }
            } else if (xmlStrcmp(cur_node->name, (const xmlChar*) "desc") == 0) {
                //found description   
                //printf ("Found description\n");
                if (cur_node->children->content != NULL ){
                    //printf("  content: %s\n", cur_node->children->content);
                    strcpy(tmpImg->description, (char *)(cur_node->children)->content);
                }
            } else if (xmlStrcmp(cur_node->name, (const xmlChar*) "g") == 0) {
                //printf ("Found group\n");
                /*Parse groups*/
                Group* tmpGroup = (Group*)malloc(sizeof(Group));
                tmpGroup->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                tmpGroup->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
                tmpGroup->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
                tmpGroup->paths = initializeList(&pathToString, &deletePath, &comparePaths);
                tmpGroup->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                group_parse(cur_node->children, tmpGroup);
                insertBack(tmpImg->groups, tmpGroup);
            } else if (xmlStrcmp(cur_node->name, (const xmlChar*) "rect") == 0) {
                //printf ("Found rectangle\n");
                Rectangle* tmpRectangle = (Rectangle*)malloc(sizeof(Rectangle));
                tmpRectangle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "x") == 0) {
                        //printf ("found x\n");
                        tmpRectangle->x = atof(cont);
                    } else if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "y") == 0) {
                        //printf ("found y\n");
                        tmpRectangle->y = atof(cont);
                    } else if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "width") == 0) {
                        //printf ("found width\n");
                        tmpRectangle->width = atof(cont);
                    } else if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "height") == 0) {
                        //printf ("found height\n");
                        tmpRectangle->height = atof(cont);
                    } else {
                        //printf ("found other attribute");
                        Attribute* newAttr = (Attribute*)malloc(sizeof(Attribute));
                        newAttr->value = cont;
                        newAttr->name = attrName;
                        insertBack(tmpRectangle->otherAttributes, newAttr);
                    }
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
                insertBack(tmpImg->rectangles, tmpRectangle);
            } else if (xmlStrcmp(cur_node->name, (const xmlChar*) "path") == 0) {
                //printf ("Found path\n");
                Path* tmpPath = (Path*)malloc(sizeof(Path));
                tmpPath->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "d") == 0) {
                        //printf ("found data\n");
                        tmpPath->data = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                        strcpy(tmpPath->data, cont);
                    } else {
                        //printf ("found other attribute");
                        Attribute* newAttr = (Attribute*)malloc(sizeof(Attribute));
                        newAttr->value = cont;
                        newAttr->name = attrName;
                        insertBack(tmpPath->otherAttributes, newAttr);
                    }
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
                insertBack(tmpImg->paths, tmpPath);
            } else if (xmlStrcmp(cur_node->name, (const xmlChar*) "circle") == 0) {
                //printf ("found circle\n");
                Circle* tmpCircle = (Circle*)malloc(sizeof(Circle));
                tmpCircle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "cx") == 0) {
                        //printf ("found cx\n");
                        tmpCircle->cx = atof(cont);
                    } else if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "cy") == 0) {
                        //printf ("found cy\n");
                        tmpCircle->cy = atof(cont);
                    } else if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "r") == 0) {
                        //printf ("found r\n");
                        tmpCircle->r = atof(cont);
                    } else {
                        //printf ("found other attribute");
                        Attribute* newAttr = (Attribute*)malloc(sizeof(Attribute));
                        newAttr->value = cont;
                        newAttr->name = attrName;
                        insertBack(tmpCircle->otherAttributes, newAttr);
                    }
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
                insertBack(tmpImg->circles, tmpCircle);
            } else {
                printf ("found unknown element\n");
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
            }
        }

        parse_image(cur_node->children, tmpImg);
    }
}

void group_parse (xmlNode *a_node, Group* givenGroup) {

    Group* tmpGroup;
    xmlNode *cur_node = NULL;
    xmlNode *value;
    xmlAttr *attr;
    char* attrName;
    char*cont;

    tmpGroup = givenGroup;

    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(cur_node->name, (const xmlChar*) "g") == 0) {
                //printf ("Found group\n");
            }
            else if (xmlStrcmp(cur_node->name, (const xmlChar*) "rect") == 0) {
                //printf ("Found rectangle\n");
                Rectangle* tmpRectangle = (Rectangle*)malloc(sizeof(Rectangle));
                tmpRectangle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "x") == 0) {
                        //printf ("found x\n");
                        tmpRectangle->x = atof(cont);
                    } else if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "y") == 0) {
                        //printf ("found y\n");
                        tmpRectangle->y = atof(cont);
                    } else if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "width") == 0) {
                        //printf ("found width\n");
                        tmpRectangle->width = atof(cont);
                    } else if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "height") == 0) {
                        //printf ("found height\n");
                        tmpRectangle->height = atof(cont);
                    } else {
                        //printf ("found other attribute");
                        Attribute* newAttr = (Attribute*)malloc(sizeof(Attribute));
                        newAttr->value = cont;
                        newAttr->name = attrName;
                        insertBack(tmpRectangle->otherAttributes, newAttr);
                    }
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
                insertBack(tmpGroup->rectangles, tmpRectangle);
            } else if (xmlStrcmp(cur_node->name, (const xmlChar*) "path") == 0) {
                //printf ("Found path\n");
                Path* tmpPath = (Path*)malloc(sizeof(Path));
                tmpPath->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "d") == 0) {
                        //printf ("found data\n");
                        tmpPath->data = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                        strcpy(tmpPath->data, cont);
                    } else {
                        //printf ("found other attribute");
                        Attribute* newAttr = (Attribute*)malloc(sizeof(Attribute));
                        newAttr->value = cont;
                        newAttr->name = attrName;
                        insertBack(tmpPath->otherAttributes, newAttr);
                    }
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
                insertBack(tmpGroup->paths, tmpPath);
            } else if (xmlStrcmp(cur_node->name, (const xmlChar*) "circle") == 0) {
                //printf ("found circle\n");
                Circle* tmpCircle = (Circle*)malloc(sizeof(Circle));
                tmpCircle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "cx") == 0) {
                        //printf ("found cx\n");
                        tmpCircle->cx = atof(cont);
                    } else if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "cy") == 0) {
                        //printf ("found cy\n");
                        tmpCircle->cy = atof(cont);
                    } else if (xmlStrcmp((const xmlChar*) attrName, (const xmlChar*) "r") == 0) {
                        //printf ("found r\n");
                        tmpCircle->r = atof(cont);
                    } else {
                        //printf ("found other attribute");
                        Attribute* newAttr = (Attribute*)malloc(sizeof(Attribute));
                        newAttr->value = cont;
                        newAttr->name = attrName;
                        insertBack(tmpCircle->otherAttributes, newAttr);
                    }
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                }
                insertBack(tmpGroup->circles, tmpCircle);
            } else {
                //printf ("found unknown element\n");
                if (cur_node->content != NULL ){
                    //printf("  content: %s\n", cur_node->content);
                }
                for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
                    Attribute* newAttr = (Attribute*)malloc(sizeof(Attribute));
                    value = attr->children;
                    attrName = (char *)attr->name;
                    cont = (char *)(value->content);
                    //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
                    newAttr->value = cont;
                    newAttr->name = attrName;
                    insertBack(tmpGroup->otherAttributes, newAttr);
                }
            }
        }
        group_parse(cur_node->children, tmpGroup);
    }

}