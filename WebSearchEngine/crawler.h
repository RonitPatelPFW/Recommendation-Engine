#ifndef CRAWLER_H
#define CRAWLER_H


  #define MAX_ADDR_LENGTH 1000

    struct listNode{
        char addr[MAX_ADDR_LENGTH]; 
        struct listNode *next;
    };


    int contains(const struct listNode *pNode, const char *addr);

    void insertBack(struct listNode *pNode, const char *addr);

    void printAddresses(const struct listNode *pNode);

    void destroyList(struct listNode *pNode);

    int getLink(const char* srcAddr, char* link, const int maxLinkLength);



#endif