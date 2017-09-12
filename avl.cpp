#include <stdio.h>
#include <math.h>

#include <algorithm>

#include "avl.h"

Node::Node(int _key){
  height = 0;
  key = _key;
  left = right = pred = NULL;
}

int Node::child_height(Node *node){
  return node ? node->height : -1;
}

bool Node::left_heavier(){
  return Node::child_height(left) > Node::child_height(right);
}

bool Node::right_heavier(){
  return Node::child_height(right) > Node::child_height(left);
}

void Node::fix_height(){
  this->height = std::max(Node::child_height(right),Node::child_height(left))+1;
}

bool Node::balanced(){
  return (abs(Node::child_height(this->left)-Node::child_height(this->right)) <= 1);
}

AVL::AVL(){
  this->root = NULL;
}

void AVL::insert(Node *node){
  Node *pred = NULL;
  for(Node *v = this->root; v; ){
    pred = v;
    v->height++;
    v = (v->key < node->key) ? v->right : v->left;
  }
  if(pred){
    if(pred->key < node->key) pred->right = node;
    else pred->left = node;
    node->pred = pred;
  }
  else{
    this->root = node;
    return;
  }
  //printf("Fixing: ");
  for(; node; node = node->pred){
    node->fix_height();
    this->make_avl(node);
  }
  //printf("\n");
}

//This method could be really more short, but not more optimized.
void AVL::remove(Node *node){
  if(!node) return;
  Node *fix = NULL;
  if(node->left && node->right){
    Node *replace = node->left;
    for(Node *v = replace->right; v; replace = v,v = v->right);
    if(replace == node->left){
      if(node == root){
        replace->right = root->right;
        replace->pred = NULL;
        root = replace;
      }
      else{
        if(node->pred->right == node) node->pred->right = replace;
        else node->pred->left = replace;
        replace->pred = node->pred;
        replace->right = node->right;
      }
      replace->right->pred = replace;
      fix = replace;
    }
    else{
      if(node == root){
        root = replace;
      }
      else{
        if(node->pred->right == node) node->pred->right = replace;
        else node->pred->left = replace;
      }
      fix = replace->pred;
      if(replace->left){
        fix->right = replace->left;
        fix->right->pred = fix;
      }
      else fix->right = NULL;
      replace->pred = node->pred;
      replace->left = node->left;
      replace->right = node->right;
      node->left->pred = replace;
      node->right->pred = replace;
    }
  }
  else if(node->left || node->right){
    Node *child = node->left ? node->left : node->right;
    if(node == root){
      child->pred = NULL;
      root = child;
      fix = root;
    }
    else{
      if(node->pred->right == node) node->pred->right = child;
      else node->pred->left = child;
      child->pred = node->pred;
      fix = child;
    }
  }
  else{
    if(node == root){
      delete node;
      root = NULL;
      return;
    }
    fix = node->pred;
    if(fix->right == node) fix->right = NULL;
    else fix->left = NULL;
  }
  delete node;
  for(; fix; fix = fix->pred){
    fix->fix_height();
    this->make_avl(fix);
  }
}

void AVL::LR(Node *node){
  if(node == root){
    root = node->right;
  }
  else{
    if(node->pred->left == node) node->pred->left = node->right;
    else node->pred->right = node->right;
  }
  node->right->pred = node->pred;
  node->pred = node->right;
  Node *fix = node->right;
  Node *LT_right_child = node->right->left;
  node->right->left = node;
  node->right = LT_right_child;
  if(LT_right_child) LT_right_child->pred = node;
  node->fix_height();
  fix->fix_height();
}

void AVL::RR(Node *node){
  if(node == root){
    root = node->left;
  }
  else{
    if(node->pred->left == node) node->pred->left = node->left;
    else node->pred->right = node->left;
  }
  node->left->pred = node->pred;
  node->pred = node->left;
  Node *fix = node->left;
  Node *RT_left_child = node->left->right;
  node->left->right = node;
  node->left = RT_left_child;
  if(RT_left_child) RT_left_child->pred = node;
  node->fix_height();
  fix->fix_height();
}

void AVL::make_avl(Node *node){
  if(node->balanced()) return;
  if(node->right_heavier()){
    if(node->right->right_heavier()){
      LR(node);
    }
    else{
      RR(node->right);
      LR(node);
    }
    return;
  }
  //node is left heavier.
  if(node->left->left_heavier()){
    RR(node);
  }
  else{
    LR(node->left);
    RR(node);
  }
}

Node *AVL::find(Node node){
  for(Node *v = this->root; v; ){
    if(v->key == node.key) return v;
    v = (v->key < node.key) ? v->right: v->left;
  }
  return NULL;
}

void AVL::clear(Node *node){
  if(!node) return;
  this->clear(node->left);
  this->clear(node->right);
  delete node;
}

AVL::~AVL(){
  this->clear(root);
}

void AVL::print_inorder(Node *node){
  if(!node) return;
  print_inorder(node->left);
  printf("key:%d height:%d\n",node->key,node->height);
  print_inorder(node->right);
}

void AVL::print(){
  this->print_inorder(this->root);
}

bool AVL::check_avl(Node *node){
  if(!node) return true;
  bool avl = check_avl(node->left);
  if(!node->balanced()){
    avl = false;
    printf("This node is breaking the AVL property: %d\n",node->key);
  }
  return avl & check_avl(node->right);
}

bool AVL::is_avl(){
  return check_avl(root);
}
