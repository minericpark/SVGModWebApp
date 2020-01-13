
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
    }

    root_element = xmlDocGetRootElement(doc);

    /*Run function*/

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    printf ("%s\n", argv[1]);
    return 0;
}