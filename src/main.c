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
    

    Attribute* tmpAttribute = (Attribute*)malloc(sizeof(Attribute));
    tmpAttribute->name = (char*)malloc(sizeof(char)*strlen("Min") + 1);
    tmpAttribute->value = (char*)malloc(sizeof(char)*strlen("No") + 1);
    strcpy(tmpAttribute->name, "Min");
    strcpy(tmpAttribute->value, "No");

    char* testString;

    testString = attributeToString (tmpAttribute);
    printf ("%s\n", testString);


    deleteAttribute(tmpAttribute);
    free(testString);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    return 0;
}