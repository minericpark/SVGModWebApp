/**
 * Name: Eric Minseo Park
 * Student Number: 1001018
 **/

#include "SVGParser_A2temp2.h"
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
    int* valid;

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

    valid = (int*)malloc(sizeof(int));
    *valid = 1;

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    /*Empty document detected*/
    if (root_element == NULL) {
        free(valid);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return NULL;
    }

    /*Check for namespace (if null, return NULL)*/
    if ((root_element->ns) == NULL) {
        free(valid);
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
    parse_image(root_element, newImg, 0, valid);

    //Invalid image
    if (*valid == 0) {
        free(valid);
        deleteSVGimage(newImg);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return NULL;
    }

    free(valid);

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
	tmpStr = (char*)malloc(sizeof(char)*len + 300);
	
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

/* ************************************ A2 Functions ****************************************************** */

bool validateSVGimage(SVGimage* image, char* schemaFile) {
    
    xmlDoc* doc;
    xmlSchema* givenSchema = NULL;
    xmlSchemaParserCtxt* ctxt;
    int *parseFail;

    if (image == NULL || schemaFile == NULL) {
        return false;
    }

    parseFail = (int*)malloc(sizeof(int));

    doc = convertImgToDoc(image);

    ctxt = xmlSchemaNewParserCtxt (schemaFile);
    xmlSchemaSetParserErrors (ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    givenSchema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);

    if (doc == NULL) { //Doc fails to parse
        *parseFail = 1;
    }
    else {
        xmlSchemaValidCtxt* cvtxt;
        int ret;

        cvtxt = xmlSchemaNewValidCtxt(givenSchema);
        xmlSchemaSetValidErrors(cvtxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
        ret = xmlSchemaValidateDoc(cvtxt, doc);
        if (ret == 0) { //Validates appropriately
            *parseFail = 0;
        } else if (ret > 0) { //Fails to validate
            *parseFail = 1;
        } else { //Creates an internal error
            *parseFail = 1;
        }
        xmlSchemaFreeValidCtxt(cvtxt);
        xmlFreeDoc(doc);
    }
    
    if(givenSchema != NULL) {
        xmlSchemaFree(givenSchema);
    }
    
    xmlSchemaCleanupTypes();
    xmlCleanupParser();

    //Do manual img travel parsing
    validateImage (image, parseFail);

    if (*parseFail == 1) {
        free(parseFail);
        return false;
    } else {
        free(parseFail);
        return true;
    }
}

SVGimage* createValidSVGimage(char* fileName, char* schemaFile) {

    xmlDoc* imgDoc;
    SVGimage* newImg = NULL;
    xmlSchema* givenSchema = NULL;
    xmlSchemaParserCtxt* ctxt;
    int parseFail = 0;
    
    if (fileName == NULL || schemaFile == NULL) {
        return NULL;
    }

    ctxt = xmlSchemaNewParserCtxt (schemaFile);

    xmlSchemaSetParserErrors (ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);

    givenSchema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);

    imgDoc = xmlReadFile (fileName, NULL, 0);

    //img doc unable to parse
    if (imgDoc == NULL) {
        parseFail = 1;
    }
    else {
        xmlSchemaValidCtxt* cvtxt;
        int ret;

        cvtxt = xmlSchemaNewValidCtxt(givenSchema);
        xmlSchemaSetValidErrors(cvtxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
        ret = xmlSchemaValidateDoc(cvtxt, imgDoc);
        //Validates
        if (ret == 0) {
            parseFail = 0;
        } else if (ret > 0) { //Fails to validate
            parseFail = 1;
        } else { //Generates an internal issue
            parseFail = 1;
        }
        xmlSchemaFreeValidCtxt(cvtxt);
        xmlFreeDoc(imgDoc);
    }

    if (parseFail != 1) {
        /*Call createSVGimage function*/
        newImg = createSVGimage(fileName);
    }
    
    /*Free schema and cleanup parser*/
    if (givenSchema != NULL) {
        xmlSchemaFree(givenSchema);
    }
    
    xmlSchemaCleanupTypes();
    xmlCleanupParser();

    /*Failed parse*/
    if (newImg == NULL) {
        return NULL;
    } else if (parseFail == 1) {
        return NULL;
    }

    return newImg;
}

bool writeSVGimage(SVGimage* image, char* fileName) {

    xmlDoc* doc;
    char tmpName[256];

    if (image == NULL || fileName == NULL) {
        return false;
    }

    doc = convertImgToDoc(image);

    /*Check if error occurred with the parsing*/
    if (doc == NULL) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return false;
    }

    strcpy (tmpName, fileName);
    strcat (tmpName, ".svg");

    /*Check if write is successful or not*/
    if (xmlSaveFormatFileEnc(tmpName, doc, "UTF-8", 1) != -1) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return true;
    } else {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return false;
    }
}

void setAttribute(SVGimage* image, elementType elemType, int elemIndex, Attribute* newAttribute) {

    //New attribute either -> changes existing attribute, changes existing attribute within list, adds new attribute
    ListIterator tmpIterator;
    ListIterator tmpIterator2;
    void* elem;
    void* elem2;
    int count = 0;
    int foundAttr = 0;
    int freeAttr = 1;
    char* buffer;
    char* tmpString;

    //If no elemtype, exit function
    if (elemType != RECT && elemType != CIRC && elemType != PATH && elemType != GROUP && elemType != SVG_IMAGE) {
        return;
    }

    //Modify the image itself
    if (elemType == SVG_IMAGE) {
        //Modify title
        if (strcmp(newAttribute->name, "title") == 0) {
            image->title[0] = '\0';
            strncpy(image->title, newAttribute->value, 256);
        } else if (strcmp(newAttribute->name, "description") == 0) { //Modify description
            image->description[0] = '\0';
            strncpy(image->description, newAttribute->value, 256);
        } else { //Add new attribute
            tmpIterator = createIterator(image->otherAttributes);
            while ((elem = nextElement(&tmpIterator)) != NULL) {
                if (strcmp(((Attribute*)elem)->name, newAttribute->name) == 0) { //Matched attribute
                    if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) {
                        tmpString = (char*)realloc(((Attribute*)elem)->value, strlen(newAttribute->value) + 1);
                        ((Attribute*)elem)->value = tmpString;
                        strcpy(((Attribute*)elem)->value, newAttribute->value);
                        foundAttr = 1;
                    }
                }
            }
            //Add new attribute if not found
            if (foundAttr != 1) {
                if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0 &&
                        newAttribute->name != NULL && strcmp(newAttribute->name, "") != 0) {
                    insertBack(image->otherAttributes, newAttribute);
                    freeAttr = 0;
                }
            }
        }
    } else if (elemType == CIRC) { //Modify a circle object
        tmpIterator = createIterator(image->circles);
        while((elem = nextElement(&tmpIterator)) != NULL){
            //Determine if index is found
            if (elemIndex == count) {
                if (strcmp(newAttribute->name, "cx") == 0) { //Modify cx
                    ((Circle*)elem)->cx = strtof (newAttribute->value, &buffer);
                } else if (strcmp(newAttribute->name, "cy") == 0) { //Modify cy
                    ((Circle*)elem)->cy = strtof (newAttribute->value, &buffer);
                } else if (strcmp(newAttribute->name, "r") == 0) { //Modify r
                    if (strtof(newAttribute->value, &buffer) >= 0) {
                        ((Circle*)elem)->r = strtof (newAttribute->value, &buffer);
                    }
                } else if (strcmp(newAttribute->name, "units") == 0) { //Modify units
                    if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) {
                        strcpy(((Circle*)elem)->units, newAttribute->value);
                    }
                } else { //Search through attributes list
                    tmpIterator2 = createIterator(((Circle*)elem)->otherAttributes);
                    while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                        if (strcmp(((Attribute*)elem2)->name, newAttribute->name) == 0) { //Matched attribute
                            if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) {
                                tmpString = (char*)realloc(((Attribute*)elem2)->value, strlen(newAttribute->value) + 1);
                                ((Attribute*)elem2)->value = tmpString;
                                strcpy(((Attribute*)elem2)->value, newAttribute->value);
                                foundAttr = 1;
                            }
                        }
                    }
                    //Add new attribute if not found
                    if (foundAttr != 1) {
                        if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0 &&
                                newAttribute->name != NULL && strcmp(newAttribute->name, "") != 0) {
                            insertBack(((Circle*)elem)->otherAttributes, newAttribute);
                            freeAttr = 0;
                        }
                    }
                }
            }
            count++;
        }
    } else if (elemType == RECT) { //Modify a rectangle object
        tmpIterator = createIterator(image->rectangles);
        while((elem = nextElement(&tmpIterator)) != NULL){
            //Determine if index is found
            if (elemIndex == count) {
                if (strcmp(newAttribute->name, "x") == 0) { //Modify x
                    ((Rectangle*)elem)->x = strtof (newAttribute->value, &buffer);
                } else if (strcmp(newAttribute->name, "y") == 0) { //Modify y
                    ((Rectangle*)elem)->y = strtof (newAttribute->value, &buffer);
                } else if (strcmp(newAttribute->name, "width") == 0) { //Modify width
                    if (strtof(newAttribute->value, &buffer) >= 0) {
                        ((Rectangle*)elem)->width = strtof (newAttribute->value, &buffer);
                    }
                } else if (strcmp(newAttribute->name, "height") == 0) { //Modify height
                    if (strtof(newAttribute->value, &buffer) >= 0) {
                        ((Rectangle*)elem)->height = strtof (newAttribute->value, &buffer);
                    }
                } else if (strcmp(newAttribute->name, "units") == 0) { //Modify units
                    if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) {
                        strcpy(((Rectangle*)elem)->units, newAttribute->value);
                    }
                } else { //Search through attributes list
                    tmpIterator2 = createIterator(((Rectangle*)elem)->otherAttributes);
                    while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                        if (strcmp(((Attribute*)elem2)->name, newAttribute->name) == 0) { //Matched attribute
                            if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) {
                                tmpString = (char*)realloc(((Attribute*)elem2)->value, strlen(newAttribute->value) + 1);
                                ((Attribute*)elem2)->value = tmpString;
                                strcpy(((Attribute*)elem2)->value, newAttribute->value);
                                foundAttr = 1;
                            }
                        }
                    }
                    //Add new attribute if not found
                    if (foundAttr != 1) {
                        if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0 &&
                                newAttribute->name != NULL && strcmp(newAttribute->name, "") != 0) {
                            insertBack(((Rectangle*)elem)->otherAttributes, newAttribute);
                            freeAttr = 0;
                        }
                    }
                }
            }
            count++;
        }
    } else if (elemType == PATH) { //Modify a path object
        tmpIterator = createIterator(image->paths);
        while((elem = nextElement(&tmpIterator)) != NULL){
            //Determine if index is found
            if (elemIndex == count) {
                if (strcmp(newAttribute->name, "data") == 0) { //Modify data
                    if (strcmp(newAttribute->value, "") != 0 && newAttribute->value != NULL) {
                        tmpString = (char*)realloc(((Path*)elem)->data, strlen(newAttribute->value) + 1);
                        ((Path*)elem)->data = tmpString;
                        strcpy(((Path*)elem)->data, newAttribute->value);
                    }
                } else { //Search through attributes list
                    tmpIterator2 = createIterator(((Path*)elem)->otherAttributes);
                    while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                        if (strcmp(((Attribute*)elem2)->name, newAttribute->name) == 0) { //Matched attribute
                            if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) {
                                tmpString = (char*)realloc(((Attribute*)elem2)->value, strlen(newAttribute->value) + 1);
                                ((Attribute*)elem2)->value = tmpString;
                                strcpy(((Attribute*)elem2)->value, newAttribute->value);
                                foundAttr = 1;
                            }
                        }
                    }
                    //Add new attribute if not found
                    if (foundAttr != 1) {
                        if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0 &&
                                newAttribute->name != NULL && strcmp(newAttribute->name, "") != 0) {
                            insertBack(((Path*)elem)->otherAttributes, newAttribute);
                            freeAttr = 0;
                        }
                    }
                }
            }
            count++;
        }
    } else if (elemType == GROUP) { //Modify a group object
        tmpIterator = createIterator(image->groups);
        while((elem = nextElement(&tmpIterator)) != NULL){
            //Determine if index is found
            if (elemIndex == count) {
                tmpIterator2 = createIterator(((Group*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                    if (strcmp(((Attribute*)elem2)->name, newAttribute->name) == 0) { //Matched attribute
                        if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) {
                            tmpString = (char*)realloc(((Attribute*)elem2)->value, strlen(newAttribute->value) + 1);
                            ((Attribute*)elem2)->value = tmpString;
                            strcpy(((Attribute*)elem2)->value, newAttribute->value);
                            foundAttr = 1;
                        }
                    }
                }
                //Add new attribute if not found
                if (foundAttr != 1) {
                    if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0 &&
                            newAttribute->name != NULL && strcmp(newAttribute->name, "") != 0) {
                        insertBack(((Group*)elem)->otherAttributes, newAttribute);
                        freeAttr = 0;
                    }
                }
            }
            count++;
        }
    }

    /*Free attribute when necessary*/
    if (freeAttr == 1) {
        deleteAttribute(newAttribute);
    }

}

void addComponent(SVGimage* image, elementType type, void* newElement) {

    //If no elemtype, exit function
    if (type != RECT && type != CIRC && type != PATH && type != GROUP && type != SVG_IMAGE) {
        return;
    }

    if (type == CIRC) { //If type is circle, insert into circle list of image
        insertBack ((image)->circles, (Circle*)newElement);
    } else if (type == RECT) { //If type is rectangle, insert into rectangle list of image
        insertBack ((image)->rectangles, (Rectangle*)newElement);
    } else if (type == PATH) { //If type is path, insert into path list of image
        insertBack ((image)->paths, (Path*)newElement);
    } else { //If none, then exit
        return;
    }

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
    unitLen = 51;
    listStr = toString(tmpRectangle->otherAttributes);
    attriLen = strlen(listStr) + 1;

    tmpStr = (char*)malloc(sizeof(char)*(xLen + yLen + widthLen + heightLen + unitLen + attriLen) + 100);
	
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
void parse_image(xmlNode * a_node, SVGimage* givenImg, int count, int * valid)
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
                    if (strlen((char*)(cur_node->ns)->href) >= 256) {
                        (tmpImg->namespace)[255] = '\0';
                    }
                } else {
                    *valid = 0;
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
                group_parse(cur_node->children, tmpGroup, 0, valid);
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
                        if (strtof(cont, &unit) < 0) {
                            *valid = 0;
                        }
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
                        if (strtof(cont, &unit) < 0) {
                            *valid = 0;
                        }
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
                        if (strcmp(cont, "") == 0) {
                            *valid = 0;
                        }
                        if (cont != NULL) {
                            tmpPath->data = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                            strcpy(tmpPath->data, cont);
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
                        if (strtof(cont, &unit) < 0) {
                            *valid = 0;
                        }
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
            parse_image(cur_node->children, tmpImg, 1, valid);
        }
    }
}

/*Similar to parse_image, similar structure and similar functions*/
void group_parse (xmlNode *a_node, Group* givenGroup, int count, int * valid) {

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
                group_parse(cur_node->children, tmpGroup2, 0, valid);
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
                        if (strtof(cont, &unit) < 0) {
                            *valid = 0;
                        }
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
                        if (strtof(cont, &unit) < 0) {
                            *valid = 0;
                        }
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
                        if (strcmp(cont, "") == 0) {
                            *valid = 0;
                        }
                        if (cont != NULL) {
                            tmpPath->data = (char*)malloc(sizeof(char) * strlen(cont) + 1);
                            strcpy(tmpPath->data, cont);
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
                        if (strtof(cont, &unit) < 0) {
                            *valid = 0;
                        }
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
            group_parse(cur_node->children, tmpGroup, 1, valid);
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

/* **************************** A2 Helper Functions **************************************************** */

/*Convert provided givenSVG to a doc file*/
xmlDoc* convertImgToDoc (SVGimage * givenSVG) {

    ListIterator tmpIterator;
    ListIterator tmpIterator2;
    xmlDoc* doc = NULL;
    xmlNode* root_node = NULL;
    xmlNode* tmp_node = NULL;
    void* elem;
    void* elem2;
    
    doc = xmlNewDoc ((const xmlChar*) "1.0");
    root_node = xmlNewNode (NULL, (const xmlChar*) "svg"); //Create node (root)
    xmlDocSetRootElement (doc, root_node);

    /*If there exists a namespace, add as attribute*/
    if (givenSVG->namespace == NULL || strcmp(givenSVG->namespace, "") == 0) {
        return NULL;
    } else {
        xmlSetNs(root_node, xmlNewNs(root_node, (const xmlChar*) givenSVG->namespace, NULL));
    }

    /*If there exists a title in the SVGimage, create node for it*/
    if (givenSVG->title != NULL && strcmp(givenSVG->title, "") != 0) {
        xmlNewChild (root_node, NULL, (const xmlChar*) "title", (const xmlChar*) givenSVG->title);
    }
    /*If there exists a description in the SVGimage, create node for it*/
    if (givenSVG->description != NULL && strcmp(givenSVG->description, "") != 0) {
        xmlNewChild (root_node, NULL, (const xmlChar*) "desc", (const xmlChar*) givenSVG->description);
    }

    /*If there exists attributes for svg, create attributes*/
    tmpIterator = createIterator(givenSVG->otherAttributes);
    while ((elem = nextElement(&tmpIterator)) != NULL) {
        xmlNewProp (root_node, (const xmlChar*)((Attribute*)elem)->name, (const xmlChar*)((Attribute*)elem)->value);
    }

    /*If there exists list of rectangles, create node for them*/
    tmpIterator = createIterator(givenSVG->rectangles);
    while((elem = nextElement(&tmpIterator)) != NULL){
        tmpIterator2 = createIterator(((Rectangle*)elem)->otherAttributes);
        char xStr[256];
        char yStr[256];
        char widthStr[256];
        char heightStr[256];

        sprintf (xStr, "%f", ((Rectangle*)elem)->x);
        sprintf (yStr, "%f", ((Rectangle*)elem)->y);
        sprintf (heightStr, "%f", ((Rectangle*)elem)->height);
        sprintf (widthStr, "%f", ((Rectangle*)elem)->width);

        if (((Rectangle*) elem)->units != NULL) {
            strcat (xStr, ((Rectangle*)elem)->units);
            strcat (yStr, ((Rectangle*)elem)->units);
            strcat (heightStr, ((Rectangle*)elem)->units);
            strcat (widthStr, ((Rectangle*)elem)->units);
        }

        tmp_node = xmlNewChild (root_node, NULL, (const xmlChar*) "rect", NULL);
        xmlNewProp (tmp_node, (const xmlChar*) "x", (const xmlChar*) xStr);
        xmlNewProp (tmp_node, (const xmlChar*) "y", (const xmlChar*) yStr);
        xmlNewProp (tmp_node, (const xmlChar*) "height", (const xmlChar*) heightStr);
        xmlNewProp (tmp_node, (const xmlChar*) "width", (const xmlChar*) widthStr);

        while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
            /*Parse and add attributes*/
            xmlNewProp (tmp_node, (const xmlChar*) ((Attribute*)elem2)->name, (const xmlChar*) ((Attribute*)elem2)->value);
        }
	}

    /*If there exists list of circles, create node for them*/
    tmpIterator = createIterator(givenSVG->circles);
    while((elem = nextElement(&tmpIterator)) != NULL){
        tmpIterator2 = createIterator(((Circle*)elem)->otherAttributes);
        char cxStr[256];
        char cyStr[256];
        char rStr[256];

        sprintf (cxStr, "%f", ((Circle*)elem)->cx);
        sprintf (cyStr, "%f", ((Circle*)elem)->cy);
        sprintf (rStr, "%f", ((Circle*)elem)->r);

        if (((Circle*) elem)->units != NULL) {
            strcat (cxStr, ((Circle*)elem)->units);
            strcat (cyStr, ((Circle*)elem)->units);
            strcat (rStr, ((Circle*)elem)->units);
        }

        tmp_node = xmlNewChild (root_node, NULL, (const xmlChar*) "circle", NULL);
        xmlNewProp (tmp_node, (const xmlChar*) "cx", (const xmlChar*) cxStr);
        xmlNewProp (tmp_node, (const xmlChar*) "cy", (const xmlChar*) cyStr);
        xmlNewProp (tmp_node, (const xmlChar*) "r", (const xmlChar*) rStr);

        while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
            /*Parse and add attributes*/
            xmlNewProp (tmp_node, (const xmlChar*) ((Attribute*)elem2)->name, (const xmlChar*) ((Attribute*)elem2)->value);
        }
	}

    /*If there exists list of paths, create node for them*/
    tmpIterator = createIterator(givenSVG->paths);
    while((elem = nextElement(&tmpIterator)) != NULL){
        tmpIterator2 = createIterator(((Path*)elem)->otherAttributes);
        char dataStr[256];

        sprintf (dataStr, "%s", ((Path*)elem)->data);

        tmp_node = xmlNewChild (root_node, NULL, (const xmlChar*) "path", NULL);
        xmlNewProp (tmp_node, (const xmlChar*) "d", (const xmlChar*) dataStr);

        while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
            /*Parse and add attributes*/
            xmlNewProp (tmp_node, (const xmlChar*) ((Attribute*)elem2)->name, (const xmlChar*) ((Attribute*)elem2)->value);
        }
	}

    /*If there exists list of groups, create node for them (and use recursive function)*/
    tmpIterator = createIterator (givenSVG->groups);
    while ((elem = nextElement(&tmpIterator)) != NULL) {
        /*Parse group*/
        convertGroup (root_node, (Group*) elem);
    }

    return doc;
}

/*Recursion function that checks each groups, and creates new nodes (then attachs them into the provided parent node*/
void convertGroup (xmlNode * parent_node, Group * givenGroup) {

    ListIterator tmpIterator;
    ListIterator tmpIterator2;
    xmlNode* new_node = NULL;
    xmlNode* tmp_node = NULL;
    void* elem;
    void* elem2;

    new_node = xmlNewChild (parent_node, NULL, (const xmlChar*) "g", NULL); 

    /*If there exists attributes for svg, create attributes*/
    tmpIterator = createIterator(givenGroup->otherAttributes);
    while ((elem = nextElement(&tmpIterator)) != NULL) {
        xmlNewProp (new_node, (const xmlChar*)((Attribute*)elem)->name, (const xmlChar*)((Attribute*)elem)->value);
    }

    /*If there exists list of rectangles, create node for them*/
    tmpIterator = createIterator(givenGroup->rectangles);
    while((elem = nextElement(&tmpIterator)) != NULL){
        tmpIterator2 = createIterator(((Rectangle*)elem)->otherAttributes);
        char xStr[256];
        char yStr[256];
        char widthStr[256];
        char heightStr[256];

        sprintf (xStr, "%f", ((Rectangle*)elem)->x);
        sprintf (yStr, "%f", ((Rectangle*)elem)->y);
        sprintf (heightStr, "%f", ((Rectangle*)elem)->height);
        sprintf (widthStr, "%f", ((Rectangle*)elem)->width);

        if (((Rectangle*) elem)->units != NULL) {
            strcat (xStr, ((Rectangle*)elem)->units);
            strcat (yStr, ((Rectangle*)elem)->units);
            strcat (heightStr, ((Rectangle*)elem)->units);
            strcat (widthStr, ((Rectangle*)elem)->units);
        }

        tmp_node = xmlNewChild (new_node, NULL, (const xmlChar*) "rect", NULL);
        xmlNewProp (tmp_node, (const xmlChar*) "x", (const xmlChar*) xStr);
        xmlNewProp (tmp_node, (const xmlChar*) "y", (const xmlChar*) yStr);
        xmlNewProp (tmp_node, (const xmlChar*) "height", (const xmlChar*) heightStr);
        xmlNewProp (tmp_node, (const xmlChar*) "width", (const xmlChar*) widthStr);

        while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
            /*Parse and add attributes*/
            xmlNewProp (tmp_node, (const xmlChar*) ((Attribute*)elem2)->name, (const xmlChar*) ((Attribute*)elem2)->value);
        }
	}

    /*If there exists list of circles, create node for them*/
    tmpIterator = createIterator(givenGroup->circles);
    while((elem = nextElement(&tmpIterator)) != NULL){
        tmpIterator2 = createIterator(((Circle*)elem)->otherAttributes);
        char cxStr[256];
        char cyStr[256];
        char rStr[256];

        sprintf (cxStr, "%f", ((Circle*)elem)->cx);
        sprintf (cyStr, "%f", ((Circle*)elem)->cy);
        sprintf (rStr, "%f", ((Circle*)elem)->r);

        if (((Circle*) elem)->units != NULL) {
            strcat (cxStr, ((Circle*)elem)->units);
            strcat (cyStr, ((Circle*)elem)->units);
            strcat (rStr, ((Circle*)elem)->units);
        }

        tmp_node = xmlNewChild (new_node, NULL, (const xmlChar*) "circle", NULL);
        xmlNewProp (tmp_node, (const xmlChar*) "cx", (const xmlChar*) cxStr);
        xmlNewProp (tmp_node, (const xmlChar*) "cy", (const xmlChar*) cyStr);
        xmlNewProp (tmp_node, (const xmlChar*) "r", (const xmlChar*) rStr);

        while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
            /*Parse and add attributes*/
            xmlNewProp (tmp_node, (const xmlChar*) ((Attribute*)elem2)->name, (const xmlChar*) ((Attribute*)elem2)->value);
        }
	}

    /*If there exists list of paths, create node for them*/
    tmpIterator = createIterator(givenGroup->paths);
    while((elem = nextElement(&tmpIterator)) != NULL){
        tmpIterator2 = createIterator(((Path*)elem)->otherAttributes);
        char dataStr[256];

        sprintf (dataStr, "%s", ((Path*)elem)->data);

        tmp_node = xmlNewChild (new_node, NULL, (const xmlChar*) "path", NULL);
        xmlNewProp (tmp_node, (const xmlChar*) "d", (const xmlChar*) dataStr);

        while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
            /*Parse and add attributes*/
            xmlNewProp (tmp_node, (const xmlChar*) ((Attribute*)elem2)->name, (const xmlChar*) ((Attribute*)elem2)->value);
        }
	}

    /*If there exists list of groups, create node for them (and use recursive function)*/
    tmpIterator = createIterator (givenGroup->groups);
    while ((elem = nextElement(&tmpIterator)) != NULL) {
        /*Parse group*/
        /**/
        convertGroup (new_node, (Group*)elem);
    }
}

/*Validate provided document and return either 0 or 1*/
void validateImage (SVGimage * givenImg, int * invalid) {

    //Validates a provided tree (manually?)
    //Follow specified requirements
    ListIterator tmpIterator;
    ListIterator tmpIterator2;
    void* elem;
    void* elem2;

    //Namespace is empty
    if (givenImg->namespace == NULL || strcmp(givenImg->namespace, "") == 0) {
        *invalid = 1;
    }

    //Check all attributes
    if (givenImg->otherAttributes != NULL) {
        tmpIterator = createIterator(givenImg->otherAttributes);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            if (((Attribute*)elem)->name == NULL) {
                *invalid = 1;
            }
            if (((Attribute*)elem)->value == NULL) {
            *invalid = 1;
            }
        }
    } else {
        *invalid = 1;
    }

    //check all rectangles
    if (givenImg->rectangles != NULL) {
        tmpIterator = createIterator(givenImg->rectangles);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            tmpIterator2 = createIterator(((Rectangle*)elem)->otherAttributes);
            if (((Rectangle*)elem)->width < 0) {
                *invalid = 1;
            }
            if (((Rectangle*)elem)->height < 0) {
                *invalid = 1;
            }
            if (((Rectangle*)elem)->otherAttributes == NULL) {
                *invalid = 1;
            }
            while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                if (((Attribute*)elem2)->name == NULL) {
                    *invalid = 1;
                }
                if (((Attribute*)elem2)->value == NULL) {
                    *invalid = 1;
                }
            }
        }
    } else {
        *invalid = 1;
    }
    
    //Check all circles
    if (givenImg->circles != NULL) {
        tmpIterator = createIterator(givenImg->circles);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            tmpIterator2 = createIterator(((Circle*)elem)->otherAttributes);
            if (((Circle*)elem)->r < 0) {
                *invalid = 1;
            }
            if (((Circle*)elem)->otherAttributes == NULL) {
                *invalid = 1;
            }
            while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                if (((Attribute*)elem2)->name == NULL) {
                    *invalid = 1;
                }
                if (((Attribute*)elem2)->value == NULL) {
                    *invalid = 1;
                }
            }
        }
    } else {
        *invalid = 1;
    }

    //Check all paths
    if (givenImg->paths != NULL) {
        tmpIterator = createIterator(givenImg->paths);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            tmpIterator2 = createIterator(((Path*)elem)->otherAttributes);
            if (((Path*)elem)->data == NULL) {
                *invalid = 1;
            }
            if (((Path*)elem)->otherAttributes == NULL) {
                *invalid = 1;
            }
            while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                if (((Attribute*)elem2)->name == NULL) {
                    *invalid = 1;
                }
                if (((Attribute*)elem2)->value == NULL) {
                    *invalid = 1;
                }
            }
        }
    } else {
        *invalid = 1;
    }

    //Check all groups
    if (givenImg->groups != NULL) {
        tmpIterator = createIterator(givenImg->groups);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            validateGroup ((Group*)elem, invalid);
        }
    } else {
        *invalid = 1;
    }

}

void validateGroup (Group * givenGroup, int * invalid) {

    ListIterator tmpIterator;
    ListIterator tmpIterator2;
    void* elem;
    void* elem2;

    //Check all attributes
    if (givenGroup->otherAttributes != NULL) {
        tmpIterator = createIterator(givenGroup->otherAttributes);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            if (((Attribute*)elem)->name == NULL) {
                *invalid = 1;
            }
            if (((Attribute*)elem)->value == NULL) {
            *invalid = 1;
            }
        }
    } else {
        *invalid = 1;
    }

    //check all rectangles
    if (givenGroup->rectangles != NULL) {
        tmpIterator = createIterator(givenGroup->rectangles);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            tmpIterator2 = createIterator(((Rectangle*)elem)->otherAttributes);
            if (((Rectangle*)elem)->width < 0) {
                *invalid = 1;
            }
            if (((Rectangle*)elem)->height < 0) {
                *invalid = 1;
            }
            if (((Rectangle*)elem)->otherAttributes == NULL) {
                *invalid = 1;
            }
            while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                if (((Attribute*)elem2)->name == NULL) {
                    *invalid = 1;
                }
                if (((Attribute*)elem2)->value == NULL) {
                    *invalid = 1;
                }
            }
        }
    } else {
        *invalid = 1;
    }
    
    //Check all circles
    if (givenGroup->circles != NULL) {
        tmpIterator = createIterator(givenGroup->circles);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            tmpIterator2 = createIterator(((Circle*)elem)->otherAttributes);
            if (((Circle*)elem)->r < 0) {
                *invalid = 1;
            }
            if (((Circle*)elem)->otherAttributes == NULL) {
                *invalid = 1;
            }
            while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                if (((Attribute*)elem2)->name == NULL) {
                    *invalid = 1;
                }
                if (((Attribute*)elem2)->value == NULL) {
                    *invalid = 1;
                }
            }
        }
    } else {
        *invalid = 1;
    }

    //Check all paths
    if (givenGroup->paths != NULL) {
        tmpIterator = createIterator(givenGroup->paths);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            tmpIterator2 = createIterator(((Path*)elem)->otherAttributes);
            if (((Path*)elem)->data == NULL) {
                *invalid = 1;
            }
            if (((Path*)elem)->otherAttributes == NULL) {
                *invalid = 1;
            }
            while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                if (((Attribute*)elem2)->name == NULL) {
                    *invalid = 1;
                }
                if (((Attribute*)elem2)->value == NULL) {
                    *invalid = 1;
                }
            }
        }
    } else {
        *invalid = 1;
    }

    //Check all groups
    if (givenGroup->groups != NULL) {
        tmpIterator = createIterator(givenGroup->groups);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            validateGroup ((Group*)elem, invalid);
        }
    } else {
        *invalid = 1;
    }
}

