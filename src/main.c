#include "LinkedListAPI.h"
#include "SVGParser.h"

SVGimage* SVGtest(char* fileName) {

    SVGimage* testImg;
    testImg = createSVGimage(fileName);
    return testImg;
}

void attributeTest () {

    printf ("----------Attribute Test----------\n");
    Attribute* tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
    Attribute* tmpAttribute2 = (Attribute*)malloc(sizeof(Attribute));
    tmpAttribute->name = (char*)malloc(sizeof(char)*strlen("Min") + 1);
    tmpAttribute->value = (char*)malloc(sizeof(char)*strlen("20") + 1);
    tmpAttribute2->name = (char*)malloc(sizeof(char)*strlen("Dame") + 1);
    tmpAttribute2->value = (char*)malloc(sizeof(char)*strlen("19") + 1);
    strcpy(tmpAttribute->name, "Min");
    strcpy(tmpAttribute->value, "20");
    strcpy(tmpAttribute2->name, "Dame");
    strcpy(tmpAttribute2->value, "19");

    char* testString;
    char* testString2;
    int compLen;

    testString = attributeToString (tmpAttribute);
    printf ("%s\n", testString);
    testString2 = attributeToString (tmpAttribute2);
    printf ("%s\n", testString2);
    compLen = compareAttributes(tmpAttribute, tmpAttribute2);
    printf ("%d\n", compLen);

    deleteAttribute(tmpAttribute);
    deleteAttribute(tmpAttribute2);
    free(testString);
    free(testString2);
}

void rectTest() {
    
    printf ("----------Rectangle Test----------");
    Rectangle* testRect = (Rectangle*)malloc(sizeof(Rectangle));
    Rectangle* testRect2 = (Rectangle*)malloc(sizeof(Rectangle));
    char tmpStr[100];
	int memLen;

    testRect->x = 5;
    testRect->y = 5;
    testRect->width = 10;
    testRect->height = 10;
    strcpy(testRect->units, "testRect");

    testRect2->x = 3;
    testRect2->y = 3;
    testRect2->width = 9;
    testRect2->height = 9;
    strcpy(testRect2->units, "testRect2");

    List* attriList = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    List* attriList2 = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    Attribute* tmpAttribute;

    for (int i = 0; i < 4; i++){

        tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		sprintf(tmpStr, "Name%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->name, tmpStr);
		
		sprintf(tmpStr, "Value%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->value, tmpStr);
	
		insertBack(attriList, (void*)tmpAttribute);
	}
    for (int i = 3; i >= 0; i--){

        tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		sprintf(tmpStr, "Name%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->name, tmpStr);
		
		sprintf(tmpStr, "Value%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->value, tmpStr);
	
		insertBack(attriList2, (void*)tmpAttribute);
	}

    testRect->otherAttributes = attriList;
    testRect2->otherAttributes = attriList2;

    char* testString = rectangleToString(testRect);
    char* testString2 = rectangleToString(testRect2);
    int compLen;

    printf ("\n%s", testString);
    printf ("\n%s\n", testString2);
    compLen = compareRectangles(testRect, testRect2);
    printf ("Compare value: %d\n", compLen);

    free(testString);
    free(testString2);

    deleteRectangle(testRect);
    deleteRectangle(testRect2);
}

void circTest() {
    
    printf ("----------Circle Test----------");
    Circle* testCirc = (Circle*)malloc(sizeof(Circle));
    Circle* testCirc2 = (Circle*)malloc(sizeof(Circle));
    char tmpStr[100];
	int memLen;

    testCirc->cx = 5;
    testCirc->cy = 5;
    testCirc->r = 10;
    strcpy(testCirc->units, "testCirc");

    testCirc2->cx = 3;
    testCirc2->cy = 3;
    testCirc2->r = 9;
    strcpy(testCirc2->units, "testCirc2");

    List* attriList = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    List* attriList2 = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    Attribute* tmpAttribute;

    for (int i = 0; i < 4; i++){

        tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		sprintf(tmpStr, "Name%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->name, tmpStr);
		
		sprintf(tmpStr, "Value%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->value, tmpStr);
	
		insertBack(attriList, (void*)tmpAttribute);
	}
    for (int i = 3; i >= 0; i--){

        tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		sprintf(tmpStr, "Name%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->name, tmpStr);
		
		sprintf(tmpStr, "Value%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->value, tmpStr);
	
		insertBack(attriList2, (void*)tmpAttribute);
	}

    testCirc->otherAttributes = attriList;
    testCirc2->otherAttributes = attriList2;

    char* testString = circleToString(testCirc);
    char* testString2 = circleToString(testCirc2);
    int compLen;

    printf ("\n%s", testString);
    printf ("\n%s\n", testString2);
    compLen = compareCircles(testCirc, testCirc2);
    printf ("Compare value: %d\n", compLen);

    free(testString);
    free(testString2);

    deleteCircle(testCirc);
    deleteCircle(testCirc2);
}

void pathTest() {

    printf ("----------Path Test----------");
    Path* testPath = (Path*)malloc(sizeof(Path));
    Path* testPath2 = (Path*)malloc(sizeof(Path));
    char tmpStr[100];
	int memLen;

    testPath->data = (char*)malloc(sizeof(char)*strlen("Pathdata") + 1);
    testPath2->data = (char*)malloc(sizeof(char)*strlen("Pathdata2") + 1);
    strcpy(testPath->data, "Pathdata");
    strcpy(testPath2->data, "Pathdata2");

    List* attriList = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    List* attriList2 = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    Attribute* tmpAttribute;

    for (int i = 0; i < 4; i++){

        tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		sprintf(tmpStr, "Name%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->name, tmpStr);
		
		sprintf(tmpStr, "Value%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->value, tmpStr);
	
		insertBack(attriList, (void*)tmpAttribute);
	}
    for (int i = 3; i >= 0; i--){

        tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		sprintf(tmpStr, "Name%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->name, tmpStr);
		
		sprintf(tmpStr, "Value%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->value, tmpStr);
	
		insertBack(attriList2, (void*)tmpAttribute);
	}

    testPath->otherAttributes = attriList;
    testPath2->otherAttributes = attriList2;

    char* testString = pathToString(testPath);
    char* testString2 = pathToString(testPath2);
    int compLen;

    printf ("\n%s", testString);
    printf ("\n%s\n", testString2);
    compLen = comparePaths(testPath, testPath2);
    printf ("Compare value: %d\n", compLen);

    free(testString);
    free(testString2);

    deletePath(testPath);
    deletePath(testPath2);
}
/*
void groupTest() {
    Group* testGroup = (Group*)malloc(sizeof(Group));
    Group* testGroup2 = (Group*)malloc(sizeof(Group));

    List* rectList = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    List* rectList2 = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);

    List* circList = initializeList(&circleToString, &deleteCircle, &compareCircles);
    List* circList2 = initializeList(&circleToString, &deleteCircle, &compareCircles);

    List* pathList = initializeList(&pathToString, &deletePath, &comparePaths);
    List* pathList2 = initializeList(&pathToString, &deletePath, &comparePaths);

    List* groupList = initializeList(&groupToString, &deleteGroup, &compareGroups);
    List* groupList2 = initializeList(&groupToString, &deleteGroup, &compareGroups);

    List* attriList = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    List* attriList2 = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    List* tmpList = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    Attribute* tmpAttribute;
    Rectangle* tmpRectangle;
    Circle* tmpCircle;
    Path* tmpPath;
    Group* tmpGroup;
    char tmpStr[100];
	int memLen;

    for (int i = 0; i < 4; i++){

        tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		sprintf(tmpStr, "Name%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->name, tmpStr);
		
		sprintf(tmpStr, "Value%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->value, tmpStr);
	
		insertBack(attriList, (void*)tmpAttribute);
	}
    for (int i = 3; i >= 0; i--){

        tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		sprintf(tmpStr, "Name%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->name, tmpStr);
		
		sprintf(tmpStr, "Value%d", i);
		memLen = strlen(tmpStr)+2;
		tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpAttribute->value, tmpStr);
	
		insertBack(attriList2, (void*)tmpAttribute);
	}
    testGroup->otherAttributes = attriList;
    testGroup2->otherAttributes = attriList2;

    //Rectangle
    for (int i = 0; i < 4; i++){

        tmpRectangle = (Rectangle*)malloc(sizeof(Rectangle));
		
        tmpRectangle->x = i;
        tmpRectangle->y = i;
        tmpRectangle->width = i;
        tmpRectangle->height = i;
		sprintf(tmpStr, "unit%d", i);
		strcpy(tmpRectangle->units, tmpStr);
        for (int i = 0; i < 4; i++){

            tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		    sprintf(tmpStr, "Name%d", i);
		    memLen = strlen(tmpStr)+2;
		    tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		    strcpy(tmpAttribute->name, tmpStr);
		
		    sprintf(tmpStr, "Value%d", i);
		    memLen = strlen(tmpStr)+2;
		    tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		    strcpy(tmpAttribute->value, tmpStr);
	
		    insertBack(tmpList, (void*)tmpAttribute);
	    }
        tmpRectangle->otherAttributes = tmpList;
		
		insertBack(rectList, (void*)tmpRectangle);
	}
    for (int i = 3; i >= 0; i--){
        
        tmpRectangle = (Rectangle*)malloc(sizeof(Rectangle));
		
        tmpRectangle->x = i;
        tmpRectangle->y = i;
        tmpRectangle->width = i;
        tmpRectangle->height = i;
		sprintf(tmpStr, "unit%d", i);
		strcpy(tmpRectangle->units, tmpStr);
        for (int i = 3; i <= 1; i--){

            tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		    sprintf(tmpStr, "Name%d", i);
		    memLen = strlen(tmpStr)+2;
		    tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		    strcpy(tmpAttribute->name, tmpStr);
		
		    sprintf(tmpStr, "Value%d", i);
		    memLen = strlen(tmpStr)+2;
		    tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		    strcpy(tmpAttribute->value, tmpStr);
	
		    insertBack(tmpList, (void*)tmpAttribute);
	    }
        tmpRectangle->otherAttributes = tmpList;
		
		insertBack(rectList2, (void*)tmpRectangle);
	}
    testGroup->rectangles = rectList;
    testGroup2->rectangles = rectList2;

    //Circle
    for (int i = 0; i < 4; i++){

        tmpCircle = (Circle*)malloc(sizeof(Circle));
		
        tmpCircle->cx = i;
        tmpCircle->cy = i;
        tmpCircle->r = i;
		sprintf(tmpStr, "unit%d", i);
		strcpy(tmpCircle->units, tmpStr);
        for (int i = 0; i < 4; i++){

            tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		    sprintf(tmpStr, "Name%d", i);
		    memLen = strlen(tmpStr)+2;
		    tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		    strcpy(tmpAttribute->name, tmpStr);
		
		    sprintf(tmpStr, "Value%d", i);
		    memLen = strlen(tmpStr)+2;
		    tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		    strcpy(tmpAttribute->value, tmpStr);
	
		    insertBack(tmpList, (void*)tmpAttribute);
	    }
        tmpCircle->otherAttributes = tmpList;
		
		insertBack(circList, (void*)tmpCircle);
	}
    for (int i = 3; i >= 0; i--){
        
        tmpCircle = (Circle*)malloc(sizeof(Circle));
		
        tmpCircle->cx = i;
        tmpCircle->cy = i;
        tmpCircle->r = i;
		sprintf(tmpStr, "unit%d", i);
		strcpy(tmpCircle->units, tmpStr);
        for (int i = 3; i <= 1; i--){

            tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		    sprintf(tmpStr, "Name%d", i);
		    memLen = strlen(tmpStr)+2;
		    tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		    strcpy(tmpAttribute->name, tmpStr);
		
		    sprintf(tmpStr, "Value%d", i);
		    memLen = strlen(tmpStr)+2;
		    tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		    strcpy(tmpAttribute->value, tmpStr);
	
		    insertBack(tmpList, (void*)tmpAttribute);
	    }
        tmpCircle->otherAttributes = tmpList;
		
		insertBack(circList2, (void*)tmpCircle);
	}
    testGroup->circles = circList;
    testGroup2->circles = circList2;

    //Path
    for (int i = 0; i < 4; i++){

        tmpPath = (Path*)malloc(sizeof(Path));
		
		sprintf(tmpStr, "Data%d", i);
		memLen = strlen(tmpStr)+2;
		tmpPath->data = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpPath->data, tmpStr);
        
        for (int i = 0; i < 4; i++){

            tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		    sprintf(tmpStr, "Name%d", i);
		    memLen = strlen(tmpStr)+2;
		    tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		    strcpy(tmpAttribute->name, tmpStr);
		
		    sprintf(tmpStr, "Value%d", i);
		    memLen = strlen(tmpStr)+2;
		    tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		    strcpy(tmpAttribute->value, tmpStr);
	
		    insertBack(tmpList, (void*)tmpAttribute);
	    }
        tmpPath->otherAttributes = tmpList;
	
		insertBack(pathList, (void*)tmpPath);
	}
    for (int i = 3; i >= 0; i--){

        tmpPath = (Path*)malloc(sizeof(Path));
		
		sprintf(tmpStr, "Data%d", i);
		memLen = strlen(tmpStr)+2;
		tmpPath->data = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpPath->data, tmpStr);

        for (int i = 3; i <= 1; i--){

            tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
		
		    sprintf(tmpStr, "Name%d", i);
		    memLen = strlen(tmpStr)+2;
		    tmpAttribute->name = (char*)malloc(sizeof(char)*memLen);
		    strcpy(tmpAttribute->name, tmpStr);
		
		    sprintf(tmpStr, "Value%d", i);
		    memLen = strlen(tmpStr)+2;
		    tmpAttribute->value = (char*)malloc(sizeof(char)*memLen);
		    strcpy(tmpAttribute->value, tmpStr);
	
		    insertBack(tmpList, (void*)tmpAttribute);
	    }
        tmpPath->otherAttributes = tmpList;
	
		insertBack(pathList2, (void*)tmpPath);
	}
    testGroup->paths = pathList;
    testGroup2->paths = pathList2;

    List* groupList = initializeList(&groupToString, &deleteGroup, &compareGroups);
    List* groupList2 = initializeList(&groupToString, &deleteGroup, &compareGroups);

    deleteGroup(testGroup);
    deleteGroup(testGroup2);
}
*/

/**
 * Simple main for testing of library
 */
int main(int argc, char **argv) {

    SVGimage* testImg;
    char* testString;
    char* testString2;
    char* testString3;
    char* testString4;
    char* testString5;
    List* rects;
    List* circs;
    List* paths;
    List* groups;
    int numAttributes;
    int numRects;

    if (argc != 2)
        return(1);

    /*Function*/
    //attributeTest();
    //rectTest();
    //circTest();
    //pathTest();
    //groupTest();
    testImg = SVGtest(argv[1]);
    testString = SVGimageToString(testImg);
    printf ("%s\n", testString);
    printf ("--------------------------\n");
    rects = getRects(testImg);
    circs = getCircles(testImg);
    paths = getPaths(testImg);
    groups = getGroups(testImg);
    numAttributes = numAttr(testImg);
    printf ("==============================\n");
    printf ("%d attributes\n", numAttributes);
    printf ("%d groups\n", getLength(groups));
    printf ("%d circs\n", getLength(circs));
    printf ("%d rects\n", getLength(rects));
    printf ("%d paths\n", getLength(paths));
    free(testString);
    freeList(rects);
    freeList(circs);
    freeList(paths);
    freeList(groups);
    deleteSVGimage(testImg);

    return 0;
}