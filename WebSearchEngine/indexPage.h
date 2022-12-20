#ifndef INDEXPAGE_H
#define INDEXPAGE_H

    #define LETTER_COUNT 26
    struct Node
    {
    int counter;
    struct Node *children[LETTER_COUNT]; 
    };
    

    struct Node *indexPage(const char* url, int *pTotalNumTerms);

    int addWordOccurrence(const char* word, const int wordLength, struct Node *node);

    void printTrieContents(struct Node *node, char *str);

    int freeTrieMemory(struct Node *node);

    int getText(const char* srcAddr, char* buffer, const int bufSize);

    int getWord(char **word, int *wordlen);

    char getChar(int index);

    int getIndex(char nextChar);

    struct Node* createNode();

#endif

