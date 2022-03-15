#include "LList.h"

void _fe(Node *node, int index, NodeList *list) {
  printf("  %d.%ld %s (%p)\n", index, node->value.data.integer, node->key.data.text, node);
}

void printList(NodeList *list) {
  printf("List (%p)\n", list);
  NodeListForEach(list, _fe);
}

extern void _printNode(Node *node);

BOOL _fl(Node *node, int index, NodeList *list) {
  return (node && node->value.data.integer > 1);
}

Node *_mp(Node *node, int index, NodeList *list) {
  Node *nuNode = NodeClone(node);
  if (nuNode) {
    switch (nuNode->value.type) {
      case NT_Decimal: 
        nuNode->value.data.decimal += 1.0;
        break;

      case NT_Integer:
        nuNode->value.data.integer+=2;
        break;
      
      default:
        break;
    }
  }

  return nuNode;
}

Node *_rd(Node *previous, Node *current, int index, NodeList *list) {
  if (!previous) { return NodeClone(current); }

  previous->value.data.integer += current->value.data.integer;

  return previous;
}

int main(int argc, char **argv) {
  NodeList *list = NodeListAlloc();
  NodeList *list2 = NULL;
  NodeList *list3 = NULL;
  Node *reduced = NULL;
  Node *a = NodeAllocTextKeyedInteger("Brielle", 1);
  Node *b = NodeAllocTextKeyedInteger("was here", 2);
  Node *c = NULL;

  printList(list);

  NodeListAdd(list, a);
  NodeListAdd(list, b);

  NodeListForEach(list, _fe);
  
  list2 = NodeListMap(list, _mp);
  printList(list2);

  list3 = NodeListFilter(list, _fl);
  printList(list3);

  reduced = NodeListReduce(list2, _rd, NULL);
  printf("Reduced: %ld %s (%p)\n", reduced->value.data.integer, reduced->key.data.text, reduced);

  c = NodeListFindValue(list, NodeDataInteger(2));
  if (c) {
    printf("value 2 was found. It matches b? %s\n", c == b ? "true" : "false");
  }
  else {
    printf("No value was found.\n");
  }

  printf("\n");

  NodeListDealloc(list);
  NodeListDealloc(list2);
  NodeListDealloc(list3);

  return 0;
}