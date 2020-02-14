#include "LinkedListAPI.h"
#include "SVGParser.h"

SVGimage* SVGtest(char* fileName) {

    SVGimage* testImg;
    testImg = createSVGimage(fileName);
    return testImg;
}

SVGimage* SVGtestA2 (char* fileName, char* schemaName) {
    SVGimage* testImg;
    testImg = createValidSVGimage(fileName, schemaName);
    
    return testImg;
}

void groupTest(SVGimage* givenImg) {
    char* tmpString;
    List* tmpList;

    printf ("----------Group Test----------\n");
    tmpString = groupToJSON(getFromBack(givenImg->groups));
    printf ("%s\n", tmpString);
    free(tmpString);

    tmpString = SVGtoJSON(givenImg);
    printf ("%s\n", tmpString);
    free(tmpString);

    tmpString = attrListToJSON(givenImg->otherAttributes);
    printf ("%s\n", tmpString);
    free(tmpString);

    tmpList = getCircles(givenImg);
    tmpString = circListToJSON(tmpList);
    printf ("%s\n", tmpString);
    freeList(tmpList);
    free(tmpString);

    tmpList = getRects(givenImg);
    tmpString = rectListToJSON(tmpList);
    printf ("%s\n", tmpString);
    freeList(tmpList);
    free(tmpString);

    tmpList = getPaths(givenImg);
    tmpString = pathListToJSON(tmpList);
    printf ("%s\n", tmpString);
    freeList(tmpList);
    free(tmpString);

    tmpList = getGroups(givenImg);
    tmpString = groupListToJSON(tmpList);
    printf ("%s\n", tmpString);
    freeList(tmpList);
    free(tmpString);

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

    free(testString);
    free(testString2);

    testString = attrToJSON(tmpAttribute);
    printf ("%s\n", testString);
    free(testString);

    testString = attrToJSON(tmpAttribute2);
    printf ("%s\n", testString);
    free(testString);

    deleteAttribute(tmpAttribute);
    deleteAttribute(tmpAttribute2);
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

    testString = rectToJSON(testRect);
    printf ("%s\n", testString);
    free(testString);
    
    testString = rectToJSON(testRect2);
    printf ("%s\n", testString);
    free(testString);

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

    testString = circleToJSON(testCirc);
    printf ("%s\n", testString);
    free(testString);

    testString = circleToJSON(testCirc2);
    printf ("%s\n", testString);
    free(testString);

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
    testString = pathToJSON(testPath);
    printf ("%s\n", testString);

    free(testString);
    testString = pathToJSON(testPath2);
    printf ("%s\n", testString);

    free(testString);
    free(testString2);

    deletePath(testPath);
    deletePath(testPath2);
}

void mainTest(char* fileName1, char* fileName2) {
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
    int memLen;
    Attribute* newAttribute;
    Attribute* newAttribute2;
    Attribute* newAttribute3;
    Attribute* newAttribute4;
    Attribute* newAttribute5;
    Attribute* fakeAttribute = NULL;
    Rectangle* newRect;
    Circle* newCircle;
    Path* newPath;
    Group* newGroup;
    char tmpStr[100];

    /*Function*/
    //attributeTest();
    //rectTest();
    //circTest();
    //pathTest();
    //groupTest();
    printf ("----------Main Test----------\n");

    testImg = SVGtestA2(fileName1, fileName2);
    if (testImg != NULL) {
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

        newAttribute = (Attribute*)calloc(1, sizeof(Attribute));
        newAttribute2 = (Attribute*)calloc(1, sizeof(Attribute));
        newAttribute3 = (Attribute*)calloc(1, sizeof(Attribute));
        newAttribute4 = (Attribute*)calloc(1, sizeof(Attribute));
        newAttribute5 = (Attribute*)calloc(1, sizeof(Attribute));

        newRect = (Rectangle*)calloc(1, sizeof(Rectangle));
        newCircle = (Circle*)calloc(1, sizeof(Circle));
        newPath = (Path*)calloc(1, sizeof(Path));

        strcpy(tmpStr, "width");
		memLen = strlen(tmpStr)+2;
		newAttribute->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(newAttribute->name, tmpStr);
		
		strcpy(tmpStr, "90000");
		memLen = strlen(tmpStr)+2;
		newAttribute->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(newAttribute->value, tmpStr);

        testString = attrToJSON(newAttribute);
        printf ("%s\n", testString);
        free(testString);

        strcpy(tmpStr, "r");
		memLen = strlen(tmpStr)+2;
		newAttribute2->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(newAttribute2->name, tmpStr);
		
		strcpy(tmpStr, "60");
		memLen = strlen(tmpStr)+2;
		newAttribute2->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(newAttribute2->value, tmpStr);

        testString = attrToJSON(newAttribute2);
        printf ("%s\n", testString);
        free(testString);

        strcpy(tmpStr, "data");
		memLen = strlen(tmpStr)+2;
		newAttribute3->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(newAttribute3->name, tmpStr);
		
		strcpy(tmpStr, "100 4378 32190231");
		memLen = strlen(tmpStr)+2;
		newAttribute3->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(newAttribute3->value, tmpStr);

        strcpy(tmpStr, "fill");
		memLen = strlen(tmpStr)+2;
		newAttribute4->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(newAttribute4->name, tmpStr);
		
		strcpy(tmpStr, "all");
		memLen = strlen(tmpStr)+2;
		newAttribute4->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(newAttribute4->value, tmpStr);

        strcpy(tmpStr, "title");
		memLen = strlen(tmpStr)+2;
		newAttribute5->name = (char*)malloc(sizeof(char)*memLen);
		strcpy(newAttribute5->name, tmpStr);
		
		strcpy(tmpStr, "10px");
		memLen = strlen(tmpStr)+2;
		newAttribute5->value = (char*)malloc(sizeof(char)*memLen);
		strcpy(newAttribute5->value, tmpStr);

        newRect->x = 30;
        newRect->y = 30;
        newRect->width = 100;
        newRect->height = 100;
        newRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

        newCircle->cx = 30;
        newCircle->cy = 30;
        newCircle->r = 30;
        newCircle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

        newPath->data = (char*)malloc(sizeof(char)*17 + 1);
        strcpy(newPath->data, "30 30 30 30 30 30");
        newPath->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

        setAttribute(testImg, RECT, 0, fakeAttribute);
        setAttribute(testImg, RECT, 0, newAttribute);
        setAttribute(testImg, CIRC, 0, newAttribute2);
        setAttribute(testImg, PATH, 0, newAttribute3);
        setAttribute(testImg, GROUP, 0, newAttribute4);
        setAttribute(testImg, SVG_IMAGE, 0, newAttribute5);

        addComponent(testImg, GROUP, newGroup);
        addComponent(testImg, RECT, newRect);
        addComponent(testImg, CIRC, newCircle);
        addComponent(testImg, PATH, newPath);

        if (writeSVGimage(testImg, "test") == false) {
            printf ("failed write\n");
        }
        if (validateSVGimage(testImg, fileName2) == false){
            printf ("validation failed\n");
        } else {
            printf ("validation success\n");
        }

        deleteSVGimage(testImg);
    }
}

/**
 * Simple main for testing of library
 */
int main(int argc, char **argv) {


    SVGimage* test = SVGtestA2(argv[1], argv[2]);

    mainTest(argv[1], argv[2]);

    attributeTest();
    rectTest();
    pathTest();
    circTest();
    groupTest(test);

    deleteSVGimage(test);

    return 0;
}