#ifndef __QNODE_IMP_H
#define __QNODE_IMP_H

typedef struct QNode
{
  void         *data;
  struct QNode *next;
} QNode;

#endif