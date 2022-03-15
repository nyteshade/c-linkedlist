#include "LList.h"

/***************************************************************************/

char *_strdup_(char *text) {
  char *result = calloc(1, strlen(text) + 1);

  if (result) {
    strcpy(result, text);
  }

  return result;
}

BOOL _nodeDefaultComparator(NodeData *left, NodeData *right) {
  if (!left || !right)
    return FALSE;
  if (left->type != right->type)
    return FALSE;

  switch (left->type) {
    case NT_Pointer:
      return left->data.pointer == right->data.pointer;
    
    case NT_Integer:
      return left->data.integer == right->data.integer;

    case NT_Decimal:
      return left->data.decimal == right->data.decimal;

    case NT_Text:
      return strcmp(left->data.text, right->data.text) == 0;
  }
}

void _printNode(Node *node) {
  char output[255];
  NodeData pair[2];
  int i;

  sprintf(output, "Node(%p)", node);
  if (!node) {
    printf("%s: NULL\n", output);
    return; 
  }

  pair[0] = node->key;
  pair[1] = node->value;

  for (i = 0; i < 2; i++) {
    char *name = i == 0 ? "key" : "value";

    switch (pair[i].type) {
      case NT_Text:
        sprintf(
          output, 
          "%s %s(%s):%s", 
          output, 
          name, 
          "text", 
          pair[i].data.text
        );
        break;

      case NT_Pointer:
        sprintf(
          output, 
          "%s %s(%s):%p", 
          output, 
          name, 
          "pointer", 
          pair[i].data.pointer
        );
        break;
        
      case NT_Integer:
        sprintf(
          output, 
          "%s %s(%s):%ld", 
          output, 
          name, 
          "integer", 
          pair[i].data.integer
        );
        break;

      case NT_Decimal:
        sprintf(
          output, 
          "%s %s(%s):%f", 
          output, 
          name, 
          "decimal", 
          pair[i].data.decimal
        );
        break;
    };    
  }

  printf("%s\n", output);
}

/***************************************************************************/

NodeData NodeDataText(char *text, BOOL duplicate) {
  NodeData data;

  data.type = NT_Text;
  data.length = strlen(text);
  data.data.text = duplicate ? _strdup_(text) : text;
  data.compare = _nodeDefaultComparator;

  return data;
}

NodeData NodeDataPointer(void *pointer) {
  NodeData data;

  data.type = NT_Pointer;
  data.data.pointer = pointer;
  data.compare = _nodeDefaultComparator;

  return data;
}

NodeData NodeDataInteger(long integer) {
  NodeData data;

  data.type = NT_Integer;
  data.data.integer = integer;
  data.compare = _nodeDefaultComparator;

  return data;
}

NodeData NodeDataDecimal(double decimal) {
  NodeData data;

  data.type = NT_Decimal;
  data.data.decimal = decimal;
  data.compare = _nodeDefaultComparator;

  return data;
}

/***************************************************************************/

Node *NodeAlloc(void) {
  return (Node*)calloc(1, sizeof(Node));
}

Node *NodeClone(Node *node) {
  if (!node) {
    return NULL;
  }

  switch (node->key.type) {
    case NT_Text:
      switch (node->value.type) {
        case NT_Text:
          return NodeAllocTextKeyedText(node->key.data.text, node->value.data.text);
        case NT_Pointer:
          return NodeAllocTextKeyedPointer(node->key.data.text, node->value.data.pointer);
        case NT_Integer:
          return NodeAllocTextKeyedInteger(node->key.data.text, node->value.data.integer);
        case NT_Decimal:
          return NodeAllocTextKeyedDecimal(node->key.data.text, node->value.data.decimal);
      }
    case NT_Pointer:
      switch (node->value.type) {
        case NT_Text:
          return NodeAllocPointerKeyedText(node->key.data.pointer, node->value.data.text);
        case NT_Pointer:
          return NodeAllocPointerKeyedPointer(node->key.data.pointer, node->value.data.pointer);
        case NT_Integer:
          return NodeAllocPointerKeyedInteger(node->key.data.pointer, node->value.data.integer);
        case NT_Decimal:
          return NodeAllocPointerKeyedDecimal(node->key.data.pointer, node->value.data.decimal);
      }
    case NT_Integer:
      switch (node->value.type) {
        case NT_Text:
          return NodeAllocIntegerKeyedText(node->key.data.integer, node->value.data.text);
        case NT_Pointer:
          return NodeAllocIntegerKeyedPointer(node->key.data.integer, node->value.data.pointer);
        case NT_Integer:
          return NodeAllocIntegerKeyedInteger(node->key.data.integer, node->value.data.integer);
        case NT_Decimal:
          return NodeAllocIntegerKeyedDecimal(node->key.data.integer, node->value.data.decimal);
      }
    case NT_Decimal:
      switch (node->value.type) {
        case NT_Text:
          return NodeAllocDecimalKeyedText(node->key.data.decimal, node->value.data.text);
        case NT_Pointer:
          return NodeAllocDecimalKeyedPointer(node->key.data.decimal, node->value.data.pointer);
        case NT_Integer:
          return NodeAllocDecimalKeyedInteger(node->key.data.decimal, node->value.data.integer);
        case NT_Decimal:
          return NodeAllocDecimalKeyedDecimal(node->key.data.decimal, node->value.data.decimal);
      }
  }
}

void NodeDealloc(Node *node) {
  if (node) {
    printf("Freeing Node(%p)\n", node);
    if (node->key.type == NT_Text && node->key.data.text) {
      printf("  ..freeing key text (%p)\n", node->key.data.text);
      free(node->key.data.text);
    }

    if (node->value.type == NT_Text && node->value.data.text) {
      printf("  ..freeing value text (%p)\n", node->value.data.text);
      free(node->value.data.text);
    }

    printf("  ..done\n");
    free(node);
  }
}

/***************************************************************************/

Node *NodeAllocTextKeyedText(char *key, char *value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataText(key, TRUE);
    node->value = NodeDataText(value, TRUE);
  }

  return node;
}

Node *NodeAllocTextKeyedPointer(char *key, void *value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataText(key, TRUE);
    node->value = NodeDataPointer(value);
  }

  return node;
}

Node *NodeAllocTextKeyedInteger(char *key, long value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataText(key, TRUE);
    node->value = NodeDataInteger(value);
  }

  return node;
}

Node *NodeAllocTextKeyedDecimal(char *key, double value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataText(key, TRUE);
    node->value = NodeDataDecimal(value);
  }

  return node;
}

Node *NodeAllocPointerKeyedText(void *key, char *value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataPointer(key);
    node->value = NodeDataText(value, TRUE);
  }

  return node;
}

Node *NodeAllocPointerKeyedPointer(void *key, void *value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataPointer(key);
    node->value = NodeDataPointer(value);
  }

  return node;
}

Node *NodeAllocPointerKeyedInteger(void *key, long value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataPointer(key);
    node->value = NodeDataInteger(value);
  }

  return node;
}

Node *NodeAllocPointerKeyedDecimal(void *key, double value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataPointer(key);
    node->value = NodeDataDecimal(value);
  }

  return node;
}

Node *NodeAllocIntegerKeyedText(long key, char *value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataInteger(key);
    node->value = NodeDataText(value, TRUE);
  }

  return node;
}

Node *NodeAllocIntegerKeyedPointer(long key, void *value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataInteger(key);
    node->value = NodeDataPointer(value);
  }

  return node;
}

Node *NodeAllocIntegerKeyedInteger(long key, long value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataInteger(key);
    node->value = NodeDataInteger(value);
  }

  return node;
}

Node *NodeAllocIntegerKeyedDecimal(long key, double value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataInteger(key);
    node->value = NodeDataDecimal(value);
  }

  return node;
}

Node *NodeAllocDecimalKeyedText(double key, char *value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataDecimal(key);
    node->value = NodeDataText(value, TRUE);
  }

  return node;
}

Node *NodeAllocDecimalKeyedPointer(double key, void *value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataDecimal(key);
    node->value = NodeDataPointer(value);
  }

  return node;
}

Node *NodeAllocDecimalKeyedInteger(double key, long value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataDecimal(key);
    node->value = NodeDataInteger(value);
  }

  return node;
}

Node *NodeAllocDecimalKeyedDecimal(double key, double value) {
  Node *node = NodeAlloc();

  if (node) {
    node->key = NodeDataDecimal(key);
    node->value = NodeDataDecimal(value);
  }

  return node;
}

/***************************************************************************/

NodeList *NodeListAlloc(void) {
  return calloc(1, sizeof(NodeList));
}

void NodeListDealloc(NodeList *list) {
  Node *current = NULL;
  Node *previous = NULL;

  if (!list || !list->head) {
    return;
  }

  printf("Freeing list(%p)\n", list);

  current = list->head;
  while (current) {
    previous = current;
    current = current->next;

    NodeDealloc(previous);
  }

  free(list);
  printf("  ..list done\n");
}

/***************************************************************************/

BOOL NodeListIsEmpty(NodeList *list) {
  return !list || !list->head;
}

void NodeListAdd(NodeList *list, Node *node) {
  Node *n = NULL;
  
  if (!list) {
    return;
  }

  n = list->head;
  if (!n) {
    list->head = node;
    return;
  }

  while (n->next) {
    n = n->next;
  }

  n->next = node;
}

Node *NodeListDelete(NodeList *list, NodeData key, BOOL freeDeleted) {
  Node *current = NULL;
  Node *previous = NULL;

  if (!list || !list->head) 
    return NULL;

  current = list->head;
  while (!current->key.compare(&current->key, &key)) {
    if (current->next == NULL) {
      return NULL;
    }

    previous = current;
    current = current->next;
  }

  if (current == list->head) {
    list->head = list->head->next;
  }
  else {
    previous->next = current->next;
  }

  if (freeDeleted && current) {
    NodeDealloc(current);
    return NULL;
  }

  return current;
}

Node *__NodeFindData__(NodeList *list, NodeData key, BOOL isKey) {
  Node *node = NULL;
  Node *match = NULL;

  if (!list || !list->head) { 
    return NULL;
  }

  node = list->head;
  while (node && !match) {
    if (isKey) {
      if (_nodeDefaultComparator(&node->key, &key)) {
        match = node;
      }
    }
    else {
      if (_nodeDefaultComparator(&node->value, &key)) {
        match = node;
      }
    }
    node = node->next;
  }

  return match;
}

Node *NodeListFindKey(NodeList *list, NodeData key)
{
  return __NodeFindData__(list, key, TRUE);
}
Node *NodeListFindValue(NodeList *list, NodeData value) {
  return __NodeFindData__(list, value, FALSE);
}

void NodeListForEach(NodeList *list, NodeListIterator iterator) {
  Node *node = NULL;
  int index = 0;

  if (!list || !list->head) {
    return;
  }

  node = list->head;
  while (node) {
    iterator(node, index, list);
    index++;
    node = node->next;
  }
}

NodeList *NodeListMap(NodeList *list, NodeListMapIterator iterator) {
  NodeList *nuList = NodeListAlloc();
  Node *node = NULL;
  int index = 0;

  if (!list || !list->head || !nuList) {
    if (nuList) {
      NodeListDealloc(nuList);
    }
    return NULL;
  }

  node = list->head;
  while (node) {
    Node *nuNode = iterator(node, index, list);
    if (nuNode) {
      NodeListAdd(nuList, nuNode);
    }
    index++;
    node = node->next;
  }

  return nuList;
}

NodeList *NodeListFilter(NodeList *list, NodeListFilterIterator iterator) { 
  NodeList *nuList = NodeListAlloc();
  Node *node = NULL;
  int index = 0;

  if (!list || !list->head || !nuList) {
    if (nuList) {
      NodeListDealloc(nuList);
    }
    return NULL;
  }

  node = list->head;
  while (node) {
    BOOL accepted = iterator(node, index, list);
    if (accepted) {
      NodeListAdd(nuList, NodeClone(node));
    }
    index++;
    node = node->next;
  }

  return nuList;
}

Node *NodeListReduce(
  NodeList *list, 
  NodeListReduceIterator iterator,
  Node *initial
) {
  Node *previous = initial;
  Node *node = NULL;
  int index = 0;

  if (!list || !list->head) {
    return NULL;
  }

  node = list->head;
  while (node) {
    previous = iterator(previous, node, index, list);
    index++;
    node = node->next;
  }

  return previous;
}

/***************************************************************************/
