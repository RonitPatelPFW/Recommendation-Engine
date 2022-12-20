#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* TODO: structure definitions */
#define LETTER_COUNT 26
struct Node
{
  int counter;
  struct Node *children[LETTER_COUNT]; 
};

struct Node* createNode()
{
  // Create the new node
  struct Node* newNode = malloc(sizeof(struct Node));

  // Initialize counter
  newNode->counter = 0;
  
  // Initialize pointers
  for(int i = 0; i < LETTER_COUNT; ++i)
    newNode->children[i] = NULL;

  return newNode;
}

int getIndex(char nextChar)
{
  return nextChar - 'a';
}

char getChar(int index)
{
  return index + 'a';
}

int getWord(char **word, int *wordlen)
{
    // Find the beginning of the next word
    for(int i = 0; (*word)[i] != '\0'; i++)
    {
      // Once the first letter is found
      if(isalpha((*word)[i])) {
        
        // Set the beginning of the word
        *word += i;
        *wordlen = 0;

        // Find the length of the word
        while(isalpha((*word)[*wordlen]))
        {
          (*wordlen)++;
        }

        return 1;
      }
    }
    
  return 0;
} 

int addWordOccurrence(const char* word, const int wordLength, struct Node *node);
int getText(const char* srcAddr, char* buffer, const int bufSize);

struct Node *indexPage(const char* url, int *pTotalNumTerms) 
{
  // Get text from URL
  struct Node *root = createNode();

  char *buffer = malloc(300000 * sizeof(char));
  int length = getText(url, buffer, 300000);
  
  //printf("%s\n", url);

  // ToLower
  for (int i = 0; i < length; i++)
  {
    buffer[i] = tolower(buffer[i]);
  }
  
  // Parse text into words
  char *word = buffer;
  int wordlen = 0;

    int counter = 0;
    
  // loop until the end of the array
  while(getWord(&word, &wordlen))
  {
    counter++;
   addWordOccurrence(word, wordlen, root);

    // Add the next word to the trie
    word[wordlen] = '\0';
    // Print the word
   
    // Skip the rest of the word to avoid double counting
    word += wordlen + 1;
   }

  *pTotalNumTerms = counter;
  free(buffer);

  return root;
}

int addWordOccurrence(const char* word, const int wordLength, struct Node *node)
{
  // Base case: word length = 0
  if(wordLength == 0) 
  {
    node->counter++;
    return node -> counter;
  }
  else // Recursive case: word length > 0
  {
    int index = getIndex(word[0]);

    // Child is null
    if(node->children[index] == NULL) 
    {
       node->children[index] = createNode();
    }

    // Call for the next character in the string
    return addWordOccurrence(word + 1, wordLength - 1, node->children[index]);
  }
}

void printTrieContents(struct Node *node, char *str)
{
  int length = strlen(str);
  
  if(node == NULL) 
  {
    return;
  }

  // Print the current node's string and occurences
  if(node->counter != 0) {
      printf("\t%s \n", str);  
      str[length] = '\0'; 

  }


  // Call print recursively on the children
  for (int i = 0; i < LETTER_COUNT; i++)
  {
    // Add character to string
    str[length] = getChar(i);
    // Call recursively
    printTrieContents(node->children[i], str);
  } 
  // Reset the end of the string
  str[length] = '\0'; 

}

int freeTrieMemory(struct Node *node)
{
  if(node == NULL)
    return -1;

  // Free all children of the current node
  for (int i = 0; i < LETTER_COUNT; i++)
  {
    freeTrieMemory(node -> children[i]);
  }
  
  // Free the node
  free(node);

  return 0;
}

/* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}
