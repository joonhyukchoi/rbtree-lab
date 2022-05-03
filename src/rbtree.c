#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

size_t sizearr = 0;
key_t **ptrkey = NULL;

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t*)malloc(sizeof(node_t));
  p->nil->color = 1;
  p->nil->key = -1;
  p->nil->parent = NULL;
  p->nil->left = NULL;
  p->nil->right = NULL;
  p->root = p->nil;

  return p;
}

void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *y) {
  node_t *x = y->left;
  y->left = x->right;
  if (x->right != t->nil) {
    x->right->parent = y;
  }
  x->parent = y->parent;
  if (y->parent == t->nil) {
    t->root = x;
  } else if (y == y->parent->left) {
    y->parent->left = x;
  } else {
    y->parent->right = x;
  }
  x->right = y;
  y->parent = x;
}

void delete_rbtree_node(rbtree *t, node_t *nod) {
  if (nod == t->nil) {
    return;
  }
  delete_rbtree_node(t, nod->right);
  delete_rbtree_node(t, nod->left);
  free(nod);

  return;
}

void delete_rbtree(rbtree *t) {
  if (t->root != t->nil) {
    node_t *temp = t->root;
    delete_rbtree_node(t, temp);
  }
  free(t->nil);
  free(t);
}

void rbtree_insert_fixup(rbtree *t, node_t* z) {
  while (z->parent->color == 0) {
    if (z->parent == z->parent->parent->left) {
      node_t *y = z->parent->parent->right;
      if (y->color == 0) {
        z->parent->color = 1;
        y->color = 1;
        z->parent->parent->color = 0;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(t, z);
        } 
        z->parent->color = 1;
        z->parent->parent->color = 0;
        right_rotate(t, z->parent->parent);
      }
    }
    else {
      node_t *y = z->parent->parent->left;
      if (y->color == 0) {
        z->parent->color = 1;
        y->color = 1;
        z->parent->parent->color = 0;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        } 
        z->parent->color = 1;
        z->parent->parent->color = 0;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = 1;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *z = (node_t *)malloc(sizeof(node_t));
  z->key = key;
  node_t *y = t->nil;
  node_t *x = t->root;
  while (x != t->nil) {
    y = x;
    if (z->key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  z->parent = y;
  if (y == t->nil) {
    t->root = z;
  } else if (z->key < y->key) {
    y->left = z;
  } else {
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = 0;

  rbtree_insert_fixup(t, z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *temp = t->root;
  if (temp == t->nil) {
    return NULL;
  }

  while (temp != t->nil) {
    if (key == temp->key) {
      return temp;
    } else if (key > temp->key) {
      temp = temp->right;
    } else {
      temp = temp->left;
    }
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *temp = t->root;
  if (temp == t->nil) {
    return NULL;
  }

  while (temp->left != t->nil) {
    temp = temp->left;
  }

  return temp;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *temp = t->root;
  if (temp == t->nil) {
    return NULL;
  }

  while (temp->right != t->nil) {
    temp = temp->right;
  }

  return temp;
}

void rbtree_erase_fixup(rbtree *t, node_t *x) {
  while (x != t->root && x->color == 1) {
    if (x == x->parent->left) {
      node_t *w = x->parent->right;
      if (w->color == 0) {
        w->color = 1;
        x->parent->color = 0;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == 1 && w->right->color == 1) {
        w->color = 0;
        x = x->parent;
      } else {
        if (w->right->color == 1) {
        w->left->color = 1;
        w->color = 0;
        right_rotate(t, w);
        w = x->parent->right;
        }
        if (w->right->color == 0) {
          w->color = x->parent->color;
          x->parent->color = 1;
          w->right->color = 1;
          left_rotate(t, x->parent);
          x = t->root;
        }
      }
    } else {
      node_t *w = x->parent->left;
      if (w->color == 0) {
        w->color = 1;
        x->parent->color = 0;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == 1 && w->left->color == 1) {
        w->color = 0;
        x = x->parent;
      } else {
        if (w->left->color == 1) {
        w->right->color = 1;
        w->color = 0;
        left_rotate(t, w);
        w = x->parent->left;
        }
        if (w->left->color == 0) {
          w->color = x->parent->color;
          x->parent->color = 1;
          w->left->color = 1;
          right_rotate(t, x->parent);
          x = t->root;
        }
      }
    }
  }
  x->color = 1;
}
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

node_t *rbtree_min_ofnode(const rbtree *t, node_t *temp) {
  if (temp == t->nil) {
    return NULL;
  }

  while (temp->left != t->nil) {
    temp = temp->left;
  }

  return temp;
}

int rbtree_erase(rbtree *t, node_t *p) {
  if (t->root == t->nil) {
    return 0;
  }

  if (rbtree_find(t, p->key) == NULL) {
    return 0;
  }

  node_t *z = p;
  node_t *y = z;
  node_t *x;
  color_t y_orgin_color = y->color;
  if (z->left == t->nil) {
    x = z->right;
    rbtree_transplant(t, z, z->right);
  } else if (z->right == t->nil) {
    x = z->left;
    rbtree_transplant(t, z, z->left);
  } else {
    y = rbtree_min_ofnode(t, z->right);
    y_orgin_color = y->color;
    x = y->right;
    if (y->parent == z) {
      x->parent = y;
    } else {
      rbtree_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rbtree_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_orgin_color == 1) {
    rbtree_erase_fixup(t, x);
  }
  free(z);

  return 1;
}

void search_array(const rbtree *t, node_t *curnode, size_t n) {
  if (curnode == t->nil) {
    return;
  }
  search_array(t, curnode->left, n);

  if (sizearr < n) {
  (*ptrkey)[sizearr] = curnode->key;
  // printf("sizearr: %ld, %d", sizearr, (*ptrkey)[sizearr]);
  sizearr++;
  // *ptrkey = *ptrkey + sizearr;
  } else {
    return;
  }

  search_array(t, curnode->right, n);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if (n == 0) {
    arr = NULL;
    return 0;
  }

  key_t *temparr = (key_t *)calloc(n, sizeof(key_t));
  ptrkey = &temparr;
  search_array(t, t->root, n);
  memcpy(arr, temparr, n * sizeof(key_t));
  // printf("test:: %d, %d %d %d", temparr, arr, (*ptrkey)[0], arr[0]);
  sizearr = 0;
  ptrkey = NULL;
  free(temparr);
  return 1;
}
