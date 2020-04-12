/**
 * Name: Eric Minseo Park
 * Student Number: 1001018
 **/

#include "SVGParser.h"
#include "SVGHelper.h"

/*Creates a SVGimage with provided SVG file (fileName)*/
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

    /*Invalid image*/
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

/*Creates a string of the SVG image*/
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

/*Deletes a provided SVGimage (by freeing)*/
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


/*Function that returns the list of rectangles from img*/
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
/*Function that returns a list of all circles in the image*/
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

/*Function that returns a list of all groups in the image*/
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

/* Function that returns a list of all paths in the image*/
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


/* Function that returns the number of all rectangles with the specified area*/
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

/* Function that returns the number of all circles with the specified area*/
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

/* Function that returns the number of all paths with the specified data - i.e. Path.data field*/
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

/* Function that returns the number of all groups with the specified length - see A1 Module 2 for details*/
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

/*  Function that returns the total number of Attribute structs in the SVGimage*/
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
/*Function that validates the SVGimage with provided schemafile and validation helper*/
bool validateSVGimage(SVGimage* image, char* schemaFile) {
    
    xmlDoc* doc;
    xmlSchema* givenSchema = NULL;
    xmlSchemaParserCtxt* ctxt;
    int *parseFail;

    if (image == NULL || schemaFile == NULL) {
        return false;
    }

    ctxt = xmlSchemaNewParserCtxt (schemaFile);
    /*Check if ctxt parser returns an error or not*/
    if (ctxt != NULL) {
        xmlSchemaSetParserErrors (ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
        givenSchema = xmlSchemaParse(ctxt);
        xmlSchemaFreeParserCtxt(ctxt);
    } else {
        /*Cleanup everything if ctxt holds an error*/
        xmlSchemaCleanupTypes();
        xmlCleanupParser();
        return false;
    }

    parseFail = (int*)malloc(sizeof(int));

    doc = convertImgToDoc(image);

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

/*Function that creates an SVG with provided SVG file, then validates it, and releases it if it successfully validates*/
SVGimage* createValidSVGimage(char* fileName, char* schemaFile) {

    SVGimage* newImg = NULL;
    xmlDoc* doc;
    xmlSchema* givenSchema = NULL;
    xmlSchemaParserCtxt* ctxt;
    int parseFail;
    
    if (fileName == NULL || schemaFile == NULL) {
        return NULL;
    }

    /*Doc check with schema*/
    ctxt = xmlSchemaNewParserCtxt (schemaFile);
    /*Check if ctxt parser returns an error or not*/
    if (ctxt != NULL) {
        xmlSchemaSetParserErrors (ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
        givenSchema = xmlSchemaParse(ctxt);
        xmlSchemaFreeParserCtxt(ctxt);
    } else {
        parseFail = 1;
    }

    doc = xmlReadFile(fileName, NULL, 0);

    if (doc == NULL) { //Doc fails to parse
        parseFail = 1;
    }
    else {
        xmlSchemaValidCtxt* cvtxt;
        int ret;

        cvtxt = xmlSchemaNewValidCtxt(givenSchema);
        xmlSchemaSetValidErrors(cvtxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
        ret = xmlSchemaValidateDoc(cvtxt, doc);
        if (ret == 0) { //Validates appropriately
            parseFail = 0;
        } else if (ret > 0) { //Fails to validate
            parseFail = 1;
        } else { //Creates an internal error
            parseFail = 1;
        }
        xmlSchemaFreeValidCtxt(cvtxt);
        xmlFreeDoc(doc);
    }
    
    if(givenSchema != NULL) {
        xmlSchemaFree(givenSchema);
    }
    
    xmlSchemaCleanupTypes();
    xmlCleanupParser();

    /*If no fail, create image*/
    if (parseFail != 1) {
        newImg = createSVGimage(fileName);
    }

    /*Validate with validation function*/
    if (validateSVGimage(newImg, schemaFile) != true) {
        deleteSVGimage(newImg);
    }

    /*Failed parse: Note that this is technicallyy useless, but just kept for safety*/
    if (newImg == NULL) {
        return NULL;
    }

    return newImg;
}

/*Function that writes an SVG image into a different file (provided by filename), returns true if success, else false*/
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

    /*Check if write is successful or not*/
    if (xmlSaveFormatFileEnc(tmpName, doc, NULL, 1) != -1) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return true;
    } else {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return false;
    }
}


//Function sets an attribute of the image, determining type by elemtype, index by elemindex
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

    //check if image or attribute is null
    if (image == NULL || newAttribute == NULL) {
        return;
    }

    //Check if attribute name or value are null
    if (newAttribute->name == NULL || newAttribute->value == NULL) {
        return;
    }

    //Modify the image itself
    if (elemType == SVG_IMAGE) {
        //Modify title
        if (strcmp(newAttribute->name, "title") == 0) {
            image->title[0] = '\0';
            strncpy(image->title, newAttribute->value, 256);
            foundAttr = 1;
        } else if (strcmp(newAttribute->name, "description") == 0) { //Modify description
            image->description[0] = '\0';
            strncpy(image->description, newAttribute->value, 256);
            foundAttr = 1;
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
                if ((newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) ||
                        (newAttribute->name != NULL && strcmp(newAttribute->name, "") != 0)) {
                    insertBack(image->otherAttributes, newAttribute);
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
                    foundAttr = 1;
                } else if (strcmp(newAttribute->name, "cy") == 0) { //Modify cy
                    ((Circle*)elem)->cy = strtof (newAttribute->value, &buffer);
                    foundAttr = 1;
                } else if (strcmp(newAttribute->name, "r") == 0) { //Modify r
                    if (strtof(newAttribute->value, &buffer) >= 0) {
                        ((Circle*)elem)->r = strtof (newAttribute->value, &buffer);
                        foundAttr = 1;
                    }
                } else if (strcmp(newAttribute->name, "units") == 0) { //Modify units
                    if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) {
                        strcpy(((Circle*)elem)->units, newAttribute->value);
                        foundAttr = 1;
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
                        if ((newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) ||
                                (newAttribute->name != NULL && strcmp(newAttribute->name, "") != 0)) {
                            insertBack(((Circle*)elem)->otherAttributes, newAttribute);
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
                    foundAttr = 1;
                } else if (strcmp(newAttribute->name, "y") == 0) { //Modify y
                    ((Rectangle*)elem)->y = strtof (newAttribute->value, &buffer);
                    foundAttr = 1;
                } else if (strcmp(newAttribute->name, "width") == 0) { //Modify width
                    if (strtof(newAttribute->value, &buffer) >= 0) {
                        ((Rectangle*)elem)->width = strtof (newAttribute->value, &buffer);
                        foundAttr = 1;
                    }
                } else if (strcmp(newAttribute->name, "height") == 0) { //Modify height
                    if (strtof(newAttribute->value, &buffer) >= 0) {
                        ((Rectangle*)elem)->height = strtof (newAttribute->value, &buffer);
                        foundAttr = 1;
                    }
                } else if (strcmp(newAttribute->name, "units") == 0) { //Modify units
                    if (newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) {
                        strcpy(((Rectangle*)elem)->units, newAttribute->value);
                        foundAttr = 1;
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
                        if ((newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) ||
                                (newAttribute->name != NULL && strcmp(newAttribute->name, "") != 0)) {
                            insertBack(((Rectangle*)elem)->otherAttributes, newAttribute);
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
                if (strcmp(newAttribute->name, "data") == 0 || strcmp(newAttribute->name, "d") == 0) { //Modify data
                    if (strcmp(newAttribute->value, "") != 0 && newAttribute->value != NULL) {
                        tmpString = (char*)realloc(((Path*)elem)->data, strlen(newAttribute->value) + 1);
                        ((Path*)elem)->data = tmpString;
                        strcpy(((Path*)elem)->data, newAttribute->value);
                        foundAttr = 1;
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
                        if ((newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) ||
                                (newAttribute->name != NULL && strcmp(newAttribute->name, "") != 0)) {
                            insertBack(((Path*)elem)->otherAttributes, newAttribute);
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
                    if ((newAttribute->value != NULL && strcmp(newAttribute->value, "") != 0) ||
                            (newAttribute->name != NULL && strcmp(newAttribute->name, "") != 0)) {
                        insertBack(((Group*)elem)->otherAttributes, newAttribute);
                    }
                }
            }
            count++;
        }
    }

    if (foundAttr != 1) { //If attribute was not found, do not free attribute (whether it has been inserted, or is invalid)
        freeAttr = 0;
    }

    /*Free attribute when necessary*/
    if (freeAttr == 1) {
        deleteAttribute(newAttribute);
    }

}

//Function adds a component with provided type if it is valid
void addComponent(SVGimage* image, elementType type, void* newElement) {

    //Check if image or newElement is null
    if (image == NULL || newElement == NULL) {
        return;
    }
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


//Turns an attribute into a JSON string
char* attrToJSON(const Attribute *a) {
    char* newStr;
    int nameLen;
    int valLen;
    
    if (a == NULL) {
        newStr = (char*)malloc(sizeof(char) * 2 + 1);
        strcpy (newStr, "{}");
        return newStr;
    }

    nameLen = strlen(a->name);
    valLen = strlen(a->value);

    newStr = (char*)malloc(sizeof(char) * (nameLen + valLen + 22) + 1);

    sprintf (newStr, "{\"name\":\"%s\",\"value\":\"%s\"}", a->name, a->value);    

    return newStr;
}

//Turns a circle into a JSON string
char* circleToJSON(const Circle *c) {
    char* newStr;
    int unitLen = 0;
    int numAttr = 0;
    int strLen = 0;
    
    if (c == NULL) {
        newStr = (char*)malloc(sizeof(char) * 2 + 1);
        strcpy (newStr, "{}");
        return newStr;
    }

    numAttr = getLength(c->otherAttributes);
    unitLen = strlen(c->units);
    strLen = unitLen + (25 * 3) + (sizeof(int) * 1) + 40;

    newStr = (char*)malloc(sizeof(char) * (strLen) + 1);

    sprintf (newStr, "{\"cx\":%.2f,\"cy\":%.2f,\"r\":%.2f,\"numAttr\":%d,\"units\":\"%s\"}", c->cx, c->cy, c->r, numAttr, c->units);    

    return newStr;
}

//Turns a rectangle into a JSON string
char* rectToJSON(const Rectangle *r) {
    char* newStr;
    int unitLen = 0;
    int numAttr = 0;
    int strLen = 0;
    
    if (r == NULL) {
        newStr = (char*)malloc(sizeof(char) * 2 + 1);
        strcpy (newStr, "{}");
        return newStr;
    }

    numAttr = getLength(r->otherAttributes);
    unitLen = strlen(r->units);
    //16 - Account for max size of float
    strLen = unitLen + (25 * 4) + (sizeof(int) * 1) + 43;

    newStr = (char*)malloc(sizeof(char) * (strLen) + 1);

    sprintf (newStr, "{\"x\":%.2f,\"y\":%.2f,\"w\":%.2f,\"h\":%.2f,\"numAttr\":%d,\"units\":\"%s\"}", r->x, r->y, r->width, r->height, numAttr, r->units);    

    return newStr;
}

//Turns a path into a JSON string
char* pathToJSON(const Path *p) {
    char* newStr;
    char tmpStr[64];
    int dataLen = 0;
    int numAttr = 0;
    int strLen = 0;
    
    if (p == NULL) {
        newStr = (char*)malloc(sizeof(char) * 2 + 1);
        strcpy (newStr, "{}");
        return newStr;
    }

    numAttr = getLength(p->otherAttributes);
    dataLen = strlen(p->data);
    strLen = dataLen + (sizeof(int) * 1) + 19;

    tmpStr[sizeof(tmpStr)] = '\0';
    newStr = (char*)malloc(sizeof(char) * (strLen) + 1);
    strncpy(tmpStr, p->data, 64);

    sprintf (newStr, "{\"d\":\"%s\",\"numAttr\":%d}", tmpStr, numAttr);    

    return newStr;
}

//Turns a group into a JSON string
char* groupToJSON(const Group *g) {
    char* newStr;
    int childLen = 0;
    int numAttr = 0;
    int strLen = 0;
    
    if (g == NULL) {
        newStr = (char*)malloc(sizeof(char) * 2 + 1);
        strcpy (newStr, "{}");
        return newStr;
    }

    childLen = getLength (g->circles) + getLength (g->rectangles) + getLength (g->paths) + getLength (g->groups);
    numAttr = getLength(g->otherAttributes);
    strLen = (sizeof(int) * 2) + 24;

    newStr = (char*)malloc(sizeof(char) * (strLen) + 1);

    sprintf (newStr, "{\"children\":%d,\"numAttr\":%d}", childLen, numAttr);    

    return newStr;
}

//Turns an SVG into a JSON string
char* SVGtoJSON(const SVGimage* imge) {
    char* newStr;
    int numGroup = 0;
    int numRect = 0;
    int numCirc = 0;
    int numPath = 0;
    int strLen = 0;
    List* tmpGroup;
    List* tmpRect;
    List* tmpCirc;
    List* tmpPath;
    SVGimage* tmpImg;
    
    if (imge == NULL) {
        newStr = (char*)malloc(sizeof(char) * 2 + 1);
        strcpy (newStr, "{}");
        return newStr;
    }

    tmpImg = (SVGimage*) imge;

    tmpGroup = getGroups(tmpImg);
    tmpRect = getRects(tmpImg);
    tmpCirc = getCircles(tmpImg);
    tmpPath = getPaths(tmpImg);

    numGroup = getLength(tmpGroup);
    numRect = getLength(tmpRect);
    numCirc = getLength(tmpCirc);
    numPath = getLength(tmpPath);

    strLen = (sizeof(int) * 4) + 48;

    newStr = (char*)malloc(sizeof(char) * (strLen) + 1);

    sprintf (newStr, "{\"numRect\":%d,\"numCirc\":%d,\"numPaths\":%d,\"numGroups\":%d}", numRect, numCirc, numPath, numGroup);    

    freeList(tmpGroup);
    freeList(tmpRect);
    freeList(tmpCirc);
    freeList(tmpPath);

    return newStr;
}

//Turns an attribute list to json (multiple attributes)
char* attrListToJSON(const List *list) {
    char* newStr;
    char* tmpStr;
    int numAttr = 0;
    int strLen = 0;
    int tmpCount = 0;
    ListIterator tmpIterator;
    List* tmpList;
    void* elem;
    
    if (list == NULL || getLength((List*) list) <= 0) {
        newStr = (char*)malloc(sizeof(char) * 2 + 1);
        strcpy (newStr, "[]");
        return newStr;
    }

    tmpList = (List*) list;
    tmpIterator = createIterator(tmpList);
    numAttr = getLength(tmpList);

    while((elem = nextElement(&tmpIterator)) != NULL) {
        tmpStr = attrToJSON((Attribute*)elem);
        strLen += (sizeof(char) * strlen(tmpStr));
        free(tmpStr);
    }

    newStr = (char*)malloc(sizeof(char) * (strLen + 2 + (numAttr - 1)) + 1);
    tmpIterator = createIterator(tmpList);
    elem = NULL;

    strcpy (newStr, "[");
    while((elem = nextElement(&tmpIterator)) != NULL) {
        //Last element
        tmpStr = attrToJSON((Attribute*)elem);
        strcat (newStr, tmpStr);
        free(tmpStr);
        if (tmpCount < numAttr - 1) {
            strcat (newStr, ",");
        }
        tmpCount++;
    }

    strcat (newStr, "]");

    return newStr;
}

//Turns a circle list to json (multiple circles)
char* circListToJSON(const List *list) {
    char* newStr;
    char* tmpStr;
    int numAttr = 0;
    int strLen = 0;
    int tmpCount = 0;
    ListIterator tmpIterator;
    List* tmpList;
    void* elem;
    
    if (list == NULL || getLength((List*) list) <= 0) {
        newStr = (char*)malloc(sizeof(char) * 2 + 1);
        strcpy (newStr, "[]");
        return newStr;
    }

    tmpList = (List*) list;
    tmpIterator = createIterator(tmpList);
    numAttr = getLength(tmpList);

    while((elem = nextElement(&tmpIterator)) != NULL) {
        tmpStr = circleToJSON((Circle*)elem);
        strLen += (sizeof(char) * strlen(tmpStr));
        free(tmpStr);
    }

    newStr = (char*)malloc(sizeof(char) * (strLen + 2 + (numAttr - 1)) + 1);
    tmpIterator = createIterator(tmpList);
    elem = NULL;

    strcpy (newStr, "[");
    while((elem = nextElement(&tmpIterator)) != NULL) {
        //Last element
        tmpStr = circleToJSON((Circle*)elem);
        strcat (newStr, tmpStr);
        free(tmpStr);
        if (tmpCount < numAttr - 1) {
            strcat (newStr, ",");
        }
        tmpCount++;
    }

    strcat (newStr, "]");

    return newStr;
}

//Turns a rectangle list to json (multiple rectangles)
char* rectListToJSON(const List *list) {
    char* newStr;
    char* tmpStr;
    int numAttr = 0;
    int strLen = 0;
    int tmpCount = 0;
    ListIterator tmpIterator;
    List* tmpList;
    void* elem;
    
    if (list == NULL || getLength((List*) list) <= 0) {
        newStr = (char*)malloc(sizeof(char) * 2 + 1);
        strcpy (newStr, "[]");
        return newStr;
    }

    tmpList = (List*) list;
    tmpIterator = createIterator(tmpList);
    numAttr = getLength(tmpList);

    while((elem = nextElement(&tmpIterator)) != NULL) {
        tmpStr = rectToJSON((Rectangle*)elem);
        strLen += (sizeof(char) * strlen(tmpStr));
        free(tmpStr);
    }

    newStr = (char*)malloc(sizeof(char) * (strLen + 2 + (numAttr - 1)) + 1);
    tmpIterator = createIterator(tmpList);
    elem = NULL;

    strcpy (newStr, "[");
    while((elem = nextElement(&tmpIterator)) != NULL) {
        //Last element
        tmpStr = rectToJSON((Rectangle*)elem);
        strcat (newStr, tmpStr);
        free(tmpStr);
        if (tmpCount < numAttr - 1) {
            strcat (newStr, ",");
        }
        tmpCount++;
    }

    strcat (newStr, "]");

    return newStr;
}

//Turns a path list to json (multiple paths)
char* pathListToJSON(const List *list) {
    char* newStr;
    char* tmpStr;
    int numAttr = 0;
    int strLen = 0;
    int tmpCount = 0;
    ListIterator tmpIterator;
    List* tmpList;
    void* elem;
    
    if (list == NULL || getLength((List*) list) <= 0) {
        newStr = (char*)malloc(sizeof(char) * 2 + 1);
        strcpy (newStr, "[]");
        return newStr;
    }

    tmpList = (List*) list;
    tmpIterator = createIterator(tmpList);
    numAttr = getLength(tmpList);

    while((elem = nextElement(&tmpIterator)) != NULL) {
        tmpStr = pathToJSON((Path*)elem);
        strLen += (sizeof(char) * strlen(tmpStr));
        free(tmpStr);
    }

    newStr = (char*)malloc(sizeof(char) * (strLen + 2 + (numAttr - 1)) + 1);
    tmpIterator = createIterator(tmpList);
    elem = NULL;

    strcpy (newStr, "[");
    while((elem = nextElement(&tmpIterator)) != NULL) {
        //Last element
        tmpStr = pathToJSON((Path*)elem);
        strcat (newStr, tmpStr);
        free(tmpStr);
        if (tmpCount < numAttr - 1) {
            strcat (newStr, ",");
        }
        tmpCount++;
    }

    strcat (newStr, "]");

    return newStr;
}

//Turns a group list to json (multiple groups)
char* groupListToJSON(const List *list) {
    char* newStr;
    char* tmpStr;
    int numAttr = 0;
    int strLen = 0;
    int tmpCount = 0;
    ListIterator tmpIterator;
    List* tmpList;
    void* elem;
    
    if (list == NULL || getLength((List*) list) <= 0) {
        newStr = (char*)malloc(sizeof(char) * 2 + 1);
        strcpy (newStr, "[]");
        return newStr;
    }

    tmpList = (List*) list;
    tmpIterator = createIterator(tmpList);
    numAttr = getLength(tmpList);

    while((elem = nextElement(&tmpIterator)) != NULL) {
        tmpStr = groupToJSON((Group*)elem);
        strLen += (sizeof(char) * strlen(tmpStr));
        free(tmpStr);
    }

    newStr = (char*)malloc(sizeof(char) * (strLen + 2 + (numAttr - 1)) + 1);
    tmpIterator = createIterator(tmpList);
    elem = NULL;

    strcpy (newStr, "[");
    while((elem = nextElement(&tmpIterator)) != NULL) {
        //Last element
        tmpStr = groupToJSON((Group*)elem);
        strcat (newStr, tmpStr);
        free(tmpStr);
        if (tmpCount < numAttr - 1) {
            strcat (newStr, ",");
        }
        tmpCount++;
    }

    strcat (newStr, "]");

    return newStr;
}

/* ******************************* Extra A2 functions - Optional ****************************************** */
//Turns a JSON string into a basic SVG if valid
SVGimage* JSONtoSVG(const char* svgString) {

    char tmpString[256];
    char* tmpString2;
    SVGimage* newImg;

    if (svgString == NULL || strcmp(svgString, "") == 0) {
        return NULL;
    }

    strcpy(tmpString, svgString);

    /*String must include both title and descr, if either null, invalid*/
    if (strstr(tmpString, "title") == NULL || strstr(tmpString, "descr") == NULL) {
        return NULL;
    }

    newImg = (SVGimage*)malloc(sizeof(SVGimage));
    newImg->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    newImg->circles = initializeList(circleToString, deleteCircle, compareCircles);
    newImg->groups = initializeList(groupToString, deleteGroup, compareGroups);
    newImg->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    newImg->paths = initializeList(pathToString, deletePath, comparePaths);
    strcpy(newImg->namespace, "https://www.w3.org/2000/svg");
    strcpy(newImg->title, "");
    strcpy(newImg->description, "");
    
    tmpString2 = strtok(tmpString, "{},:\"");
    while (tmpString2 != NULL) {
        if (strcmp(tmpString2, "title") == 0) {
            tmpString2 = strtok(NULL, "{},:\"");
            strcpy(newImg->title, tmpString2);
        } else if (strcmp(tmpString2, "descr") == 0) {
            tmpString2 = strtok(NULL, "{},:\"");
            strcpy(newImg->description, tmpString2);
        }
        tmpString2 = strtok(NULL, "{},:\"");
    }

    return newImg;
}

//Turns a basic JSON string to a rectangle if valid
Rectangle* JSONtoRect(const char* svgString) {

    char tmpString[256];
    char* tmpString2;
    char* tmpToken;
    Rectangle* newRect;

    if (svgString == NULL || strcmp(svgString, "") == 0) {
        return NULL;
    }

    strcpy(tmpString, svgString);

    /*String must include all elements, if any null, invalid*/
    if (strstr(tmpString, "x") == NULL || strstr(tmpString, "y") == NULL || strstr(tmpString, "w") == NULL ||
        strstr(tmpString, "h") == NULL || strstr(tmpString, "units") == NULL) {
        return NULL;
    }

    newRect = (Rectangle*)malloc(sizeof(Rectangle));
    newRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    strcpy(newRect->units, "");

    tmpString2 = strtok(tmpString, "{},:\"");
    while (tmpString2 != NULL) {
        if (strcmp(tmpString2, "x") == 0) {
            tmpString2 = strtok(NULL, "{},:\"");
            newRect->x = strtof(tmpString2, &tmpToken);
        } else if (strcmp(tmpString2, "y") == 0) {
            tmpString2 = strtok(NULL, "{},:\"");
            newRect->y = strtof(tmpString2, &tmpToken);
        } else if (strcmp(tmpString2, "w") == 0) {
            tmpString2 = strtok(NULL, "{},:\"");
            newRect->width = strtof(tmpString2, &tmpToken);
        } else if (strcmp(tmpString2, "h") == 0) {
            tmpString2 = strtok(NULL, "{},:\"");
            newRect->height = strtof(tmpString2, &tmpToken);
        } else if (strcmp(tmpString2, "units") == 0) {
            tmpString2 = strtok(NULL, "{},:\"");
            if (strcmp(tmpString2, " ") == 0) {
                continue;
            } else {
                strcpy(newRect->units, tmpString2);
            }
        }
        tmpString2 = strtok(NULL, "{},:\"");
    }

    return newRect;
}

//Turns a basic JSON string to a circle if valid
Circle* JSONtoCircle(const char* svgString) {

    char tmpString[256];
    char* tmpString2;
    char* tmpToken;
    Circle* newCirc;

    if (svgString == NULL || strcmp(svgString, "") == 0) {
        return NULL;
    }

    strcpy(tmpString, svgString);

    /*String must include all elements, if any null, invalid*/
    if (strstr(tmpString, "cx") == NULL || strstr(tmpString, "cy") == NULL || strstr(tmpString, "r") == NULL ||
        strstr(tmpString, "units") == NULL) {
        return NULL;
    }

    newCirc = (Circle*)malloc(sizeof(Circle));
    newCirc->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    strcpy(newCirc->units, "");

    tmpString2 = strtok(tmpString, "{},:\"");
    while (tmpString2 != NULL) {
        if (strcmp(tmpString2, "cx") == 0) {
            tmpString2 = strtok(NULL, "{},:\"");
            newCirc->cx = strtof(tmpString2, &tmpToken);
        } else if (strcmp(tmpString2, "cy") == 0) {
            tmpString2 = strtok(NULL, "{},:\"");
            newCirc->cy = strtof(tmpString2, &tmpToken);
        } else if (strcmp(tmpString2, "r") == 0) {
            tmpString2 = strtok(NULL, "{},:\"");
            newCirc->r = strtof(tmpString2, &tmpToken);
        } else if (strcmp(tmpString2, "units") == 0) {
            tmpString2 = strtok(NULL, "{},:\"");
            if (strcmp(tmpString2, " ") == 0) {
                continue;
            } else {
                strcpy(newCirc->units, tmpString2);
            }
        }
        tmpString2 = strtok(NULL, "{},:\"");
    }

    return newCirc;    
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
    xLen = 25;
    yLen = 25;
    widthLen = 25;
    heightLen = 25;
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
    cxLen = 25;
    cyLen = 25;
    rLen = 25;
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
    if (givenSVG->otherAttributes != NULL) {
        tmpIterator = createIterator(givenSVG->otherAttributes);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            xmlNewProp (root_node, (const xmlChar*)((Attribute*)elem)->name, (const xmlChar*)((Attribute*)elem)->value);
        }
    }

    /*If there exists list of rectangles, create node for them*/
    if (givenSVG->rectangles != NULL) {
        tmpIterator = createIterator(givenSVG->rectangles);
        while((elem = nextElement(&tmpIterator)) != NULL){
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
                strcat (widthStr, ((Rectangle*)elem)->units);
                strcat (heightStr, ((Rectangle*)elem)->units);
            }

            tmp_node = xmlNewChild (root_node, NULL, (const xmlChar*) "rect", NULL);
            xmlNewProp (tmp_node, (const xmlChar*) "x", (const xmlChar*) xStr);
            xmlNewProp (tmp_node, (const xmlChar*) "y", (const xmlChar*) yStr);
            xmlNewProp (tmp_node, (const xmlChar*) "width", (const xmlChar*) widthStr);
            xmlNewProp (tmp_node, (const xmlChar*) "height", (const xmlChar*) heightStr);

            if (((Rectangle*)elem)->otherAttributes != NULL) {
                tmpIterator2 = createIterator(((Rectangle*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                    /*Parse and add attributes*/
                    xmlNewProp (tmp_node, (const xmlChar*) ((Attribute*)elem2)->name, (const xmlChar*) ((Attribute*)elem2)->value);
                }
            }
	    }
    }

    /*If there exists list of circles, create node for them*/
    if (givenSVG->circles != NULL) {
        tmpIterator = createIterator(givenSVG->circles);
        while((elem = nextElement(&tmpIterator)) != NULL){
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

            if (((Circle*)elem)->otherAttributes != NULL) {
                tmpIterator2 = createIterator(((Circle*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                /*Parse and add attributes*/
                    xmlNewProp (tmp_node, (const xmlChar*) ((Attribute*)elem2)->name, (const xmlChar*) ((Attribute*)elem2)->value);
                }
            }
	    }
    }

    /*If there exists list of paths, create node for them*/
    if (givenSVG->paths != NULL) {
        tmpIterator = createIterator(givenSVG->paths);
        while((elem = nextElement(&tmpIterator)) != NULL){
            char dataStr[256];

            sprintf (dataStr, "%s", ((Path*)elem)->data);

            tmp_node = xmlNewChild (root_node, NULL, (const xmlChar*) "path", NULL);
            xmlNewProp (tmp_node, (const xmlChar*) "d", (const xmlChar*) dataStr);
            if (((Path*)elem)->otherAttributes != NULL) {
                tmpIterator2 = createIterator(((Path*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                    /*Parse and add attributes*/
                    xmlNewProp (tmp_node, (const xmlChar*) ((Attribute*)elem2)->name, (const xmlChar*) ((Attribute*)elem2)->value);
                }
            }
	    }
    }

    /*If there exists list of groups, create node for them (and use recursive function)*/
    if (givenSVG->groups != NULL) {
        tmpIterator = createIterator (givenSVG->groups);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            /*Parse group*/
            convertGroup (root_node, (Group*) elem);
        }
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
    if (givenGroup->otherAttributes != NULL) {
        tmpIterator = createIterator(givenGroup->otherAttributes);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            xmlNewProp (new_node, (const xmlChar*)((Attribute*)elem)->name, (const xmlChar*)((Attribute*)elem)->value);
        }   
    }

    /*If there exists list of rectangles, create node for them*/
    if (givenGroup->rectangles != NULL) {
        tmpIterator = createIterator(givenGroup->rectangles);
        while((elem = nextElement(&tmpIterator)) != NULL){
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
                strcat (widthStr, ((Rectangle*)elem)->units);
                strcat (heightStr, ((Rectangle*)elem)->units);
            }

            tmp_node = xmlNewChild (new_node, NULL, (const xmlChar*) "rect", NULL);
            xmlNewProp (tmp_node, (const xmlChar*) "x", (const xmlChar*) xStr);
            xmlNewProp (tmp_node, (const xmlChar*) "y", (const xmlChar*) yStr);
            xmlNewProp (tmp_node, (const xmlChar*) "width", (const xmlChar*) widthStr);
            xmlNewProp (tmp_node, (const xmlChar*) "height", (const xmlChar*) heightStr);

            if (((Rectangle*)elem)->otherAttributes != NULL) {
                tmpIterator2 = createIterator(((Rectangle*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                    /*Parse and add attributes*/
                    xmlNewProp (tmp_node, (const xmlChar*) ((Attribute*)elem2)->name, (const xmlChar*) ((Attribute*)elem2)->value);
                }
            }
	    }
    }

    /*If there exists list of circles, create node for them*/
    if (givenGroup->circles != NULL) {
        tmpIterator = createIterator(givenGroup->circles);
        while((elem = nextElement(&tmpIterator)) != NULL){
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

            if (((Circle*)elem)->otherAttributes != NULL) {
                tmpIterator2 = createIterator(((Circle*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                    /*Parse and add attributes*/
                    xmlNewProp (tmp_node, (const xmlChar*) ((Attribute*)elem2)->name, (const xmlChar*) ((Attribute*)elem2)->value);
                }
            }
    	}

    }

    /*If there exists list of paths, create node for them*/
    if (givenGroup->paths != NULL) {
        tmpIterator = createIterator(givenGroup->paths);
        while((elem = nextElement(&tmpIterator)) != NULL){
            char dataStr[strlen(((Path*)elem)->data) + 300];

            sprintf (dataStr, "%s", ((Path*)elem)->data);

            tmp_node = xmlNewChild (new_node, NULL, (const xmlChar*) "path", NULL);
            xmlNewProp (tmp_node, (const xmlChar*) "d", (const xmlChar*) dataStr);

            if (((Path*)elem)->otherAttributes != NULL) {
                tmpIterator2 = createIterator(((Path*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                    /*Parse and add attributes*/
                    xmlNewProp (tmp_node, (const xmlChar*) ((Attribute*)elem2)->name, (const xmlChar*) ((Attribute*)elem2)->value);
                }
            }
	    }
    }

    /*If there exists list of groups, create node for them (and use recursive function)*/
    if (givenGroup->groups != NULL) {
        tmpIterator = createIterator (givenGroup->groups);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            /*Parse group*/
            convertGroup (new_node, (Group*)elem);
        }
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
            if (((Rectangle*)elem)->width < 0) {
                *invalid = 1;
            }
            if (((Rectangle*)elem)->height < 0) {
                *invalid = 1;
            }
            if (((Rectangle*)elem)->otherAttributes != NULL) {
                tmpIterator2 = createIterator(((Rectangle*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                    if (((Attribute*)elem2)->name == NULL) {
                        *invalid = 1;
                    }
                    if (((Attribute*)elem2)->value == NULL) {
                        *invalid = 1;
                    }
                }
            } else {
                *invalid = 1;
            }
        }
    } else {
        *invalid = 1;
    }
    
    //Check all circles
    if (givenImg->circles != NULL) {
        tmpIterator = createIterator(givenImg->circles);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            if (((Circle*)elem)->r < 0) {
                *invalid = 1;
            }
            if (((Circle*)elem)->otherAttributes != NULL) {
                tmpIterator2 = createIterator(((Circle*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                    if (((Attribute*)elem2)->name == NULL) {
                        *invalid = 1;
                    }
                    if (((Attribute*)elem2)->value == NULL) {
                        *invalid = 1;
                    }
                }
            } else {
                *invalid = 1;
            }
        }
    } else {
        *invalid = 1;
    }

    //Check all paths
    if (givenImg->paths != NULL) {
        tmpIterator = createIterator(givenImg->paths);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            if (((Path*)elem)->data == NULL) {
                *invalid = 1;
            }
            if (((Path*)elem)->otherAttributes != NULL) {
                tmpIterator2 = createIterator(((Path*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                    if (((Attribute*)elem2)->name == NULL) {
                        *invalid = 1;
                    }
                    if (((Attribute*)elem2)->value == NULL) {
                        *invalid = 1;
                    }
                }
            } else {
                *invalid = 1;
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
            if (((Rectangle*)elem)->width < 0) {
                *invalid = 1;
            }
            if (((Rectangle*)elem)->height < 0) {
                *invalid = 1;
            }
            if (((Rectangle*)elem)->otherAttributes != NULL) {
                tmpIterator2 = createIterator(((Rectangle*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                    if (((Attribute*)elem2)->name == NULL) {
                        *invalid = 1;
                    }
                    if (((Attribute*)elem2)->value == NULL) {
                        *invalid = 1;
                    }
                }
            } else {
                *invalid = 1;
            }
        }
    } else {
        *invalid = 1;
    }
    
    //Check all circles
    if (givenGroup->circles != NULL) {
        tmpIterator = createIterator(givenGroup->circles);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            if (((Circle*)elem)->r < 0) {
                *invalid = 1;
            }
            if (((Circle*)elem)->otherAttributes != NULL) {
                tmpIterator2 = createIterator(((Circle*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                    if (((Attribute*)elem2)->name == NULL) {
                        *invalid = 1;
                    }
                    if (((Attribute*)elem2)->value == NULL) {
                        *invalid = 1;
                    }
                }
            } else {
                *invalid = 1;
            }
        }
    } else {
        *invalid = 1;
    }

    //Check all paths
    if (givenGroup->paths != NULL) {
        tmpIterator = createIterator(givenGroup->paths);
        while ((elem = nextElement(&tmpIterator)) != NULL) {
            if (((Path*)elem)->data == NULL) {
                *invalid = 1;
            }
            if (((Path*)elem)->otherAttributes != NULL) {
                tmpIterator2 = createIterator(((Path*)elem)->otherAttributes);
                while ((elem2 = nextElement(&tmpIterator2)) != NULL) {
                    if (((Attribute*)elem2)->name == NULL) {
                        *invalid = 1;
                    }
                    if (((Attribute*)elem2)->value == NULL) {
                        *invalid = 1;
                    }
                }
            } else {
                *invalid = 1;
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

//Wrapper functions for ff-napi from this point on

//Function that converts a given file to a valid img, and returns the appropriate JSON string
char* fileToJSON (char* filename, char* schema) {
    
    SVGimage* tmpImg;
    tmpImg = createValidSVGimage(filename, schema);

    if (tmpImg == NULL) {
        return "{}";
    } else {
        return SVGtoJSON(tmpImg);
    }

}

//Function that determines if given file is a valid img, and returns true or false dependent on the answer
bool verifyFile (char* filename, char* schema) {
    
    SVGimage* tmpImg;
    tmpImg = createValidSVGimage(filename, schema);

    if (tmpImg == NULL) {
        return false;
    } else {
        return true;
    }
}

//Function that returns a title and description from a provided SVG (filename) in JSON format
char* titleDescToJSON (char* filename, char* schema) {
    SVGimage* tmpImg;
    char* tmpStr;
    int descLen;
    int titleLen;

    tmpImg = createValidSVGimage(filename, schema);
    
    if (tmpImg == NULL) {
        return "{}";
    } 

    descLen = strlen(tmpImg->description);
    titleLen = strlen(tmpImg->title);

    tmpStr = (char*)malloc(sizeof(char) * (descLen + titleLen + 22) + 1);

    sprintf (tmpStr, "{\"title\":\"%s\",\"desc\":\"%s\"}", tmpImg->title, removeNL(tmpImg->description));   

    return tmpStr;
}

//Function that returns the rectangles from provided SVG in JSON format
char* SVGrectToJSON (char* filename, char* schema) {
    SVGimage* tmpImg;
    
    tmpImg = createValidSVGimage(filename, schema);
    
    if (tmpImg == NULL) {
        printf ("null\n");
        return "[]";
    } else {
        return rectListToJSON(tmpImg->rectangles);
    }
}

//Function that returns the circles from provided SVG in JSON format
char* SVGcircToJSON (char* filename, char* schema) {
    SVGimage* tmpImg;
    
    tmpImg = createValidSVGimage(filename, schema);
    
    if (tmpImg == NULL) {
        return "[]";
    } else {
        return circListToJSON(tmpImg->circles);
    }
}

//Function that returns the paths from provided SVG in JSON format
char* SVGpathToJSON (char* filename, char* schema) {
    SVGimage* tmpImg;
    
    tmpImg = createValidSVGimage(filename, schema);
    
    if (tmpImg == NULL) {
        return "[]";
    } else {
        return pathListToJSON(tmpImg->paths);
    }
}

//Function that returns the groups from provided SVG in JSON format
char* SVGgroupToJSON (char* filename, char* schema) {
    SVGimage* tmpImg;

    tmpImg = createValidSVGimage(filename, schema);
    
    if (tmpImg == NULL) {
        return "[]";
    } else {
        return groupListToJSON(tmpImg->groups);
    }
}

//Wrapper function that Creates and writes a new SVG dependent on server calls
char* createSVGFile (char* filename, char* schema) {
    SVGimage* selecImage;
    bool valCheck;
    char* tmpJSON;

    selecImage = (SVGimage*)calloc(1, sizeof(SVGimage));
    selecImage->namespace[0] = '\0';
    selecImage->title[0] = '\0';
    selecImage->description[0] = '\0';
    selecImage->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    selecImage->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
    selecImage->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    selecImage->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    selecImage->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);

    strcpy(selecImage->namespace, "http://www.w3.org/2000/svg");
    strcpy(selecImage->title, "");
    strcpy(selecImage->description, "");

    //Create some basic shapes
    Rectangle* baseRect = (Rectangle*)malloc(sizeof(Rectangle));
    Circle* baseCirc = (Circle*)malloc(sizeof(Circle));
    Attribute* baseAttr = (Attribute*)malloc(sizeof(Attribute));

    baseRect->x = 0;
    baseRect->y = 0;
    baseRect->width = 4;
    baseRect->height = 4;
    strcpy(baseRect->units, "cm");
    baseRect->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    
    baseCirc->cx = 0;
    baseCirc->cy = 0;
    baseCirc->r = 3;
    strcpy(baseCirc->units, "cm");
    baseCirc->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    //Create a viewbox attribute
    baseAttr->name = (char*)malloc(sizeof(char) * 8);
    baseAttr->value = (char*)malloc(sizeof(char) * 13);
    strcpy(baseAttr->name, "viewBox");
    strcpy(baseAttr->value, "0 0 1200 600");

    //Insert basic shapes
    insertBack(selecImage->circles, baseCirc);
    insertBack(selecImage->rectangles, baseRect);
    insertBack(selecImage->otherAttributes, baseAttr);

    //Temporarily check if img is valid
    valCheck = validateSVGimage(selecImage, schema);

    if (valCheck) { //Succeeds to validate
        //Rewrite file
        printf ("valid");
        writeSVGimage(selecImage, filename);

        //Return true
        tmpJSON = (char*)malloc(sizeof(char) * 18);
        strcpy(tmpJSON, "{\"value\":\"true\"}");
    } else {
        //Return false
        tmpJSON = (char*)malloc(sizeof(char) * 19);
        strcpy(tmpJSON, "{\"value\":\"false\"}");
    }

    return tmpJSON;
}

//Wrapper function that modifies titles dependent on server calls
char* modifyTitle (char* filename, char* schema, char* title) {
    SVGimage* selecImage;
    bool valCheck;
    char* tmpJSON;
    Attribute* tmpAttr;

    tmpAttr = JSONtoAttribute("title", title);
    selecImage = createValidSVGimage(filename, schema);
    setAttribute(selecImage, SVG_IMAGE, 0, tmpAttr);

    //Temporarily check if img is valid
    valCheck = validateSVGimage(selecImage, schema);

    //Assure the string length is not too large
    if (strlen(title) > 255) {
        valCheck = false;
    }

    if (valCheck) { //Succeeds to validate
        //Rewrite file
        writeSVGimage(selecImage, filename);

        //Return true
        tmpJSON = (char*)malloc(sizeof(char) * 18);
        strcpy(tmpJSON, "{\"value\":\"true\"}");
    } else {
        //Return false
        tmpJSON = (char*)malloc(sizeof(char) * 19);
        strcpy(tmpJSON, "{\"value\":\"false\"}");
    }

    return tmpJSON;
}

//Wrapper function that modifies descriptions dependent on server calls
char* modifyDescr (char* filename, char* schema, char* desc) {
    SVGimage* selecImage;
    bool valCheck;
    char* tmpJSON;
    Attribute* tmpAttr;

    tmpAttr = JSONtoAttribute("description", desc);
    selecImage = createValidSVGimage(filename, schema);
    setAttribute(selecImage, SVG_IMAGE, 0, tmpAttr);

    //Temporarily check if img is valid
    valCheck = validateSVGimage(selecImage, schema);

    //Assure the string length is not too large
    if (strlen(desc) > 255) {
        valCheck = false;
    }

    if (valCheck) { //Succeeds to validate
        //Rewrite file
        writeSVGimage(selecImage, filename);

        //Return true
        tmpJSON = (char*)malloc(sizeof(char) * 18);
        strcpy(tmpJSON, "{\"value\":\"true\"}");
    } else {
        //Return false
        tmpJSON = (char*)malloc(sizeof(char) * 19);
        strcpy(tmpJSON, "{\"value\":\"false\"}");
    }

    return tmpJSON;
}

//Wrapper function that modifies or adds a new attribute to the given component
char* modifyAttr (char* filename, char* schema, int indexNum, char* name, char* value) {

    Attribute* tmpAttr;
    SVGimage* selecImage;
    bool valCheck;
    char* attrType;
    int tmpIndex;
    char* tmpJSON;

    tmpIndex = indexNum;
    tmpAttr = JSONtoAttribute(name, value);
    selecImage = createValidSVGimage(filename, schema);
    attrType = getSVGAttrType(selecImage, indexNum);

    if (strcmp(attrType, "RECT") == 0) { //if Rectangle
        setAttribute(selecImage, RECT, tmpIndex, tmpAttr);
    } else if (strcmp(attrType, "CIRC") == 0) {
        printf ("circle detected");
        tmpIndex = tmpIndex - getLength(selecImage->rectangles);
        setAttribute(selecImage, CIRC, tmpIndex, tmpAttr);
    } else if (strcmp(attrType, "PATH") == 0) {
        printf ("path detected");
        tmpIndex = tmpIndex - (getLength(selecImage->rectangles) + getLength(selecImage->circles));
        setAttribute(selecImage, PATH, tmpIndex, tmpAttr);
    } else {
        printf ("group detected");
        tmpIndex = tmpIndex - (getLength(selecImage->rectangles) + getLength(selecImage->circles) + getLength(selecImage->paths));
        setAttribute(selecImage, GROUP, tmpIndex, tmpAttr);
    }

    //Temporarily check if img is valid
    valCheck = validateSVGimage(selecImage, schema);

    if (valCheck) { //Succeeds to validate
        //Rewrite file
        writeSVGimage(selecImage, filename);

        //Return true
        tmpJSON = (char*)malloc(sizeof(char) * 18);
        strcpy(tmpJSON, "{\"value\":\"true\"}");
    } else {
        //Return false
        tmpJSON = (char*)malloc(sizeof(char) * 19);
        strcpy(tmpJSON, "{\"value\":\"false\"}");
    }

    return tmpJSON;
}

//Wrapper function that modifies or adds a new attribute to the given component
char* modifySVGAttr (char* filename, char* schema, char* name, char* value) {

    Attribute* tmpAttr;
    SVGimage* selecImage;
    bool valCheck;
    char* tmpJSON;

    tmpAttr = JSONtoAttribute(name, value);
    selecImage = createValidSVGimage(filename, schema);
    setAttribute(selecImage, SVG_IMAGE, 0, tmpAttr);

    //Temporarily check if img is valid
    valCheck = validateSVGimage(selecImage, schema);

    if (valCheck) { //Succeeds to validate
        //Rewrite file
        writeSVGimage(selecImage, filename);

        //Return true
        tmpJSON = (char*)malloc(sizeof(char) * 18);
        strcpy(tmpJSON, "{\"value\":\"true\"}");
    } else {
        //Return false
        tmpJSON = (char*)malloc(sizeof(char) * 19);
        strcpy(tmpJSON, "{\"value\":\"false\"}");
    }

    return tmpJSON;
}

//Wrapper function that adds in a new shape
char* addSVGRect (char* filename, char* schema, char* JSONstring) {

    Rectangle* tmpRect;
    SVGimage* selecImage;
    bool valCheck;
    char* tmpJSON;

    tmpRect = JSONtoRect(JSONstring);
    selecImage = createValidSVGimage(filename, schema);
    addComponent(selecImage, RECT, tmpRect);

    //Temporarily check if img is valid
    valCheck = validateSVGimage(selecImage, schema);

    if (valCheck) { //Succeeds to validate
        //Rewrite file
        writeSVGimage(selecImage, filename);

        //Return true
        tmpJSON = (char*)malloc(sizeof(char) * 18);
        strcpy(tmpJSON, "{\"value\":\"true\"}");
    } else {
        //Return false
        tmpJSON = (char*)malloc(sizeof(char) * 19);
        strcpy(tmpJSON, "{\"value\":\"false\"}");
    }

    return tmpJSON;
}

//Wrapper function that adds in a new shape
char* addSVGCirc (char* filename, char* schema, char* JSONstring) {

    Circle* tmpCirc;
    SVGimage* selecImage;
    bool valCheck;
    char* tmpJSON;

    tmpCirc = JSONtoCircle(JSONstring);
    selecImage = createValidSVGimage(filename, schema);
    addComponent(selecImage, CIRC, tmpCirc);

    //Temporarily check if img is valid
    valCheck = validateSVGimage(selecImage, schema);

    if (valCheck) { //Succeeds to validate
        //Rewrite file
        writeSVGimage(selecImage, filename);

        //Return true
        tmpJSON = (char*)malloc(sizeof(char) * 18);
        strcpy(tmpJSON, "{\"value\":\"true\"}");
    } else {
        //Return false
        tmpJSON = (char*)malloc(sizeof(char) * 19);
        strcpy(tmpJSON, "{\"value\":\"false\"}");
    }

    return tmpJSON;
}

//Wrapper function that scales all circles
char* scaleSVGCircs (char* filename, char* schema, double scalingFactor) {

    SVGimage* selecImage;
    bool valCheck;
    char* tmpJSON;
    List* allCircs;
    ListIterator tmpIterator;
    void* elem;

    selecImage = createValidSVGimage(filename, schema);
    allCircs = getCircles(selecImage);

    //Iterate through all elements
    tmpIterator = createIterator(allCircs);
    while ((elem = nextElement(&tmpIterator)) != NULL) {
        //Scale all circles
        ((Circle*)elem)->r = ((Circle*)elem)->r * scalingFactor;
    }

    //Temporarily check if img is valid
    valCheck = validateSVGimage(selecImage, schema);

    if (valCheck) { //Succeeds to validate
        //Rewrite file
        writeSVGimage(selecImage, filename);

        //Return true
        tmpJSON = (char*)malloc(sizeof(char) * 18);
        strcpy(tmpJSON, "{\"value\":\"true\"}");
    } else {
        //Return false
        tmpJSON = (char*)malloc(sizeof(char) * 19);
        strcpy(tmpJSON, "{\"value\":\"false\"}");
    }

    return tmpJSON;
}

//Wrapper function that scales all rectangles
char* scaleSVGRects (char* filename, char* schema, double scalingFactor) {

    SVGimage* selecImage;
    bool valCheck;
    char* tmpJSON;
    List* allRects;
    ListIterator tmpIterator;
    void* elem;

    selecImage = createValidSVGimage(filename, schema);
    allRects = getRects(selecImage);

    //Iterate through all elements
    tmpIterator = createIterator(allRects);
    while ((elem = nextElement(&tmpIterator)) != NULL) {
        ((Rectangle*)elem)->width = ((Rectangle*)elem)->width * scalingFactor;
        ((Rectangle*)elem)->height = ((Rectangle*)elem)->height * scalingFactor;
    }

    //Temporarily check if img is valid
    valCheck = validateSVGimage(selecImage, schema);

    if (valCheck) { //Succeeds to validate
        //Rewrite file
        writeSVGimage(selecImage, filename);

        //Return true
        tmpJSON = (char*)malloc(sizeof(char) * 18);
        strcpy(tmpJSON, "{\"value\":\"true\"}");
    } else {
        //Return false
        tmpJSON = (char*)malloc(sizeof(char) * 19);
        strcpy(tmpJSON, "{\"value\":\"false\"}");
    }

    return tmpJSON;
}

//Wrapper function that creates an attribute through a JSON file
Attribute* JSONtoAttribute (char* name, char* value) {

    Attribute* tmpAttr;

    if (name == NULL || strcmp(name, "") == 0 || value == NULL || strcmp(value, "") == 0) {
        return NULL;
    } else {
        tmpAttr = (Attribute*)malloc(sizeof(Attribute));
        tmpAttr->name = (char*)malloc(sizeof(char) * strlen(name) + 1);
        tmpAttr->value = (char*)malloc(sizeof(char) * strlen(value) + 1);
        strcpy (tmpAttr->name, name);
        strcpy (tmpAttr->value, value);
    }

    return tmpAttr;
}

//Wrapper function that receives the SVG component, an index num (according to the table), and returns the appropriate JSON attribute string
char* getSVGComponentAttr (char* filename, char* schema, int indexNum) {
    
    List* tmpList;
    SVGimage* tmpImage;

    tmpImage = createValidSVGimage(filename, schema);
    tmpList = getSVGAttrList (tmpImage, indexNum);
    if (tmpList == NULL) {
        return "[]";
    } else {
        return attrListToJSON(tmpList);
    }
}

//Wrapper function that receives attributes of svg
char* getSVGAttr (char* filename, char* schema) {
    List* tmpList;
    SVGimage* tmpImage;

    tmpImage = createValidSVGimage(filename, schema);
    if (tmpImage == NULL) {
        return "[]";
    } else {
        tmpList = tmpImage->otherAttributes;
        return attrListToJSON(tmpList);
    }
}

//Wrapper function that receives a request for an attribute list
char* getSVGAttrType (SVGimage* givenImg, int indexNum) {

    SVGimage* tmpImg;

    tmpImg = givenImg;

    if (tmpImg == NULL) {
        return NULL;
    } else {
        if (indexNum < getLength(tmpImg->rectangles)) { //Check for rect 
            return "RECT";
        } else if (indexNum < (getLength(tmpImg->circles) + getLength(tmpImg->rectangles))) { //Check for circ num
            return "CIRC";
        } else if (indexNum < (getLength(tmpImg->paths) + getLength(tmpImg->circles) + getLength(tmpImg->rectangles))) {//Check for path num
            return "PATH";
        } else { //Index must be within groups
            return "GROUP";
        }
    }

}

//Wrapper function that receives a request for an attribute list
List* getSVGAttrList (SVGimage* givenImg, int indexNum) {

    SVGimage* tmpImg;
    ListIterator tmpIterator;
    int i = 0;
    int tmpIndex;
    void* elem;

    tmpImg = givenImg;

    if (tmpImg == NULL) {
        return NULL;
    } else {
        if (indexNum < getLength(tmpImg->rectangles)) { //Check for rect 
            Rectangle* tmpRect;
            tmpIndex = indexNum;
            tmpIterator = createIterator(tmpImg->rectangles);
            while ((elem = nextElement(&tmpIterator)) != NULL) {
                if (i == tmpIndex) { //Find specified index
                    tmpRect = (Rectangle*)elem;
                }
                i++;
            }   
            return tmpRect->otherAttributes;
        } else if (indexNum < (getLength(tmpImg->circles) + getLength(tmpImg->rectangles))) { //Check for circ num
            Circle* tmpCirc;
            tmpIndex = indexNum - getLength(tmpImg->rectangles);
            tmpIterator = createIterator(tmpImg->circles);
            while ((elem = nextElement(&tmpIterator)) != NULL) {
                if (i == tmpIndex) { //Find specified index
                    tmpCirc = (Circle*)elem;
                }
                i++;
            }   
            return tmpCirc->otherAttributes;
        } else if (indexNum < (getLength(tmpImg->paths) + getLength(tmpImg->circles) + getLength(tmpImg->rectangles))) {//Check for path num
            Path* tmpPath;
            tmpIndex = indexNum - (getLength(tmpImg->rectangles) + getLength(tmpImg->circles));
            tmpIterator = createIterator(tmpImg->paths);
            while ((elem = nextElement(&tmpIterator)) != NULL) {
                if (i == tmpIndex) { //Find specified index
                    tmpPath = (Path*)elem;
                }
                i++;
            }   
            return tmpPath->otherAttributes;
        } else { //Index must be within groups
            Group* tmpGroup;
            tmpIndex = indexNum - (getLength(tmpImg->rectangles) + getLength(tmpImg->circles) + getLength(tmpImg->paths));
            tmpIterator = createIterator(tmpImg->groups);
            while ((elem = nextElement(&tmpIterator)) != NULL) {
                if (i == tmpIndex) { //Find specified index
                    printf ("found group\n");
                    tmpGroup = (Group*)elem;
                }
                i++;
            }
            return tmpGroup->otherAttributes;
        }
    }

}



//Function that checks string and assures no JSON fails
char* passJSON (char* givenString) {

    char* tmpStr;
    int i;

    tmpStr = givenString;

    for (i = 0; i < strlen(tmpStr); i++) {
        //Remove all quotations and replace with interesting character
        if (tmpStr[i] == '\"') {
            tmpStr[i] = '*';
        }
    }

    return tmpStr;
}

//Helper function that removes all new lines
char* removeNL (char* givenString) {

    char* tmpStr;
    int i;

    tmpStr =  givenString;

    for (i = 0; i < strlen(tmpStr); i++) {
        if (tmpStr[i] == '\n') {
            tmpStr[i] = ' ';
        }
    }

    return tmpStr;

}