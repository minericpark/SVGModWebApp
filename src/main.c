#include "LinkedListAPI.h"
#include "SVGParser.h"
/**
 * Simple main for testing of library
 */
int main(int argc, char **argv) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    if (argc != 2)
        return(1);

    /*parse the file and get the DOM */
    doc = xmlReadFile(argv[1], NULL, 0);

    if (doc == NULL) {
        printf("error: could not parse file %s\n", argv[1]);
        return(1);
    }

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    /*Function*/
    attributeTest();

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    return 0;
}

void attributeTest () {

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
    
    List* attriList = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

}

void circTest() {

}

void pathTest() {

}

void groupTest() {
    Group* testGroup = (Group*)malloc(sizeof(Group));
    Group* testGroup2 = (Group*)malloc(sizeof(Group));

    List* rectList = (List*)malloc(sizeof(List));
    List* rectList2 = (List*)malloc(sizeof(List));

    List* circList = (List*)malloc(sizeof(List));
    List* circList2 = (List*)malloc(sizeof(List));

    List* pathList = (List*)malloc(sizeof(List));
    List* pathList2 = (List*)malloc(sizeof(List));

    List* groupList = (List*)malloc(sizeof(List));
    List* groupList2 = (List*)malloc(sizeof(List));

    List* attriList = (List*)malloc(sizeof(List));
    List* attriList2 = (List*)malloc(sizeof(List));

    for (int i = 0; i < 4; i++){/*
		tmpName = (Name*)malloc(sizeof(Name));
		tmpName->age = (i+1)*10;
		
		sprintf(tmpStr, "Name%d", i);
		memLen = strlen(tmpStr)+2;
		tmpName->firstName = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpName->firstName, tmpStr);
		
		sprintf(tmpStr, "Lastname%d", i);
		memLen = strlen(tmpStr)+2;
		tmpName->lastName = (char*)malloc(sizeof(char)*memLen);
		strcpy(tmpName->lastName, tmpStr);
	
		insertBack(list, (void*)tmpName);*/
	}

}

void SVGtest() {

}