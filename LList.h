#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Boolean support, define BOOL_SUPPORTED to skip this code */
#ifndef BOOL_SUPPORTED
typedef int BOOL;
enum { FALSE = 0, TRUE = 1 };
#endif

typedef enum NodeType {
  NT_Pointer, NT_Text, NT_Integer, NT_Decimal
} NodeType;

typedef union NodeDataValue {
  char *text;
  void *pointer;
  long integer;
  double decimal;
} NodeDataValue;

typedef struct NodeData {
  unsigned long length;
  NodeDataValue data;
  NodeType type;
  BOOL (*compare)(struct NodeData *left, struct NodeData *right);
} NodeData;

typedef struct Node {
  NodeData key;
  NodeData value;
  struct Node *next;
} Node;

typedef struct NodeList {
  Node *head;
  long length;
} NodeList;

/* Callback for iterating through a NodeList */
typedef void (*NodeListIterator)(Node *node, int index, NodeList *list);
typedef Node *(*NodeListMapIterator)(Node *node, int index, NodeList *list);
typedef BOOL (*NodeListFilterIterator)(Node *node, int index, NodeList *list);
typedef Node *(*NodeListReduceIterator)(
  Node *previous,
  Node *current,
  int index,
  NodeList *list
);

NodeData NodeDataText(char *text, BOOL duplicate);
NodeData NodeDataPointer(void *pointer);
NodeData NodeDataInteger(long integer);
NodeData NodeDataDecimal(double decimal);

Node *NodeAlloc(void);
Node *NodeClone(Node *node);
void NodeDealloc(Node *node);

Node *NodeAllocTextKeyedText(char *key, char *value);
Node *NodeAllocTextKeyedPointer(char *key, void *value);
Node *NodeAllocTextKeyedInteger(char *key, long value);
Node *NodeAllocTextKeyedDecimal(char *key, double value);

Node *NodeAllocPointerKeyedText(void *key, char *value);
Node *NodeAllocPointerKeyedPointer(void *key, void *value);
Node *NodeAllocPointerKeyedInteger(void *key, long value);
Node *NodeAllocPointerKeyedDecimal(void *key, double value);

Node *NodeAllocIntegerKeyedText(long key, char *value);
Node *NodeAllocIntegerKeyedPointer(long key, void *value);
Node *NodeAllocIntegerKeyedInteger(long key, long value);
Node *NodeAllocIntegerKeyedDecimal(long key, double value);

Node *NodeAllocDecimalKeyedText(double key, char *value);
Node *NodeAllocDecimalKeyedPointer(double key, void *value);
Node *NodeAllocDecimalKeyedInteger(double key, long value);
Node *NodeAllocDecimalKeyedDecimal(double key, double value);

NodeList *NodeListAlloc(void);
void NodeListDealloc(NodeList *list);

BOOL NodeListIsEmpty(NodeList *list);
void NodeListAdd(NodeList *list, Node *node);
Node *NodeListDelete(NodeList *list, NodeData key, BOOL freeDeleted);

void NodeListForEach(NodeList *list, NodeListIterator iterator);
NodeList *NodeListMap(NodeList *list, NodeListMapIterator iterator);
NodeList *NodeListFilter(NodeList *list, NodeListFilterIterator iterator);
Node *NodeListReduce(
  NodeList *list, 
  NodeListReduceIterator iterator, 
  Node *initial
);