#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "indexPage.h"
#include "crawler.h"
    
double termFreq(char *url, int total, char* term);
double inverseDocumentFreq(char *term);
double scoreDocument(char *term);
int findTotalOccurence(const char* word, const int wordLength, struct Node *node);

int MAX_N; // command arg
int maxNumOfHops; // read from file
int hopsLimit, hopNum;
int totalNumTerms = 0;
static int n = 0; // number of pages that the program has indexed

int fileOpen;

char query;
char url[MAX_ADDR_LENGTH]; // read from file
char readURL[MAX_ADDR_LENGTH];
char startAddr[MAX_ADDR_LENGTH];
char destAddr[MAX_ADDR_LENGTH];

struct listNode *pListStart;
struct Node *words;
char* str;

int totalTerms[] = {};

int main(int argc, char** argv)
{


  if(argc > 1) 
  {
    MAX_N = atoi(argv[2]);
  }

 //Indexing Phase
  FILE *fp = fopen(argv[1], "r");

	if(fp == NULL)
  {
		fprintf(stderr, "Could not open file (%s)\n", argv[1]);
		return -1;
	}
pListStart = malloc(sizeof(struct listNode));
printf("Indexing...\n");

while (fscanf(fp, "%s %d\n", url, &maxNumOfHops) != EOF && n < MAX_N) 
{
    int i = 0;
   printf("%s\n", url);
   insertBack(pListStart,  url);

    strcpy(readURL, url);

    hopsLimit = maxNumOfHops;
    hopNum = 0;

    while (1)
    {
 
    if(pListStart->next->addr != readURL) 
      {       
         words = indexPage(readURL, &totalNumTerms);
         str = malloc(sizeof(char) * 300000);
                for (int i = 0; i < 300000; i++)
                  str[i] = '\0';
            
        printTrieContents(words, str);
        totalTerms[i] = totalNumTerms;

       free(str);
      freeTrieMemory(words);

          n++; 
      }
     
      hopNum++;

      if(hopNum <= hopsLimit && n < MAX_N) 
      {

           int res = getLink(readURL, destAddr, MAX_ADDR_LENGTH);
          if(!res){
              break;
          }

          insertBack(pListStart,  destAddr);

          strcpy(readURL, destAddr);   
       
         printf("%s\n", readURL);

      }
      else
        break;
     
    } 
    
  } 
  fclose(fp);
  
    destroyList(pListStart);

  char word[20];

    printf("Enter a web query: ");
     fgets(word, sizeof(word), stdin);

    while(isupper(word[0])) {
     printf("Please enter a query containing only lower-case letters.\n");
      printf("Enter a web query: ");
      fgets(word, sizeof(word), stdin);
   }  

    printf("Query is %s\n", word);

    printf("IDF Scores are \n");

    double IDF = inverseDocumentFreq(word);
    printf("IDF(%s): %lf\n", word, IDF);
  
    printf("Web Pages: \n");
   double score = scoreDocument(word);
    printf("1. (%s): (score:%f)\n", url, score);


}

int findTotalOccurence(const char* word, const int wordLength, struct Node *node) {
  
  if(wordLength == 0) 
  {
     node->counter++;
     return node -> counter;
  }
  else
  {
   int index = getIndex(word[0]);

    if(node->children[index] == NULL) 
    {
      return node -> counter;

    }
 
    return findTotalOccurence(word + 1, wordLength - 1, node->children[index]); 
  } 
}

double scoreDocument(char *term) 
{
      double tf = termFreq(url, totalTerms[n], term);
      double idf = inverseDocumentFreq(term);

      return tf * idf;
   
} 

double termFreq(char *url, int n, char* term) 
{
    int freq;
    int total;

        total = totalTerms[2];
        freq = findTotalOccurence(term, strlen(term), words);
    
    return freq / total;
} 

double inverseDocumentFreq(char *term) 
{    
int freq = findTotalOccurence(term, strlen(term), words);
 
  return  log((1.0 + n) / (1.0 + freq));


}
