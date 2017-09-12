class Node{
public:
  int key;
  int height;
  Node *left,*right,*pred;
  Node(int);
  bool balanced();
  bool left_heavier();
  bool right_heavier();
  void fix_height();
  static int child_height(Node *);
};

class AVL{
  Node *root;
  void RR(Node *);
  void LR(Node *);
  void make_avl(Node *);
  bool check_avl(Node *);
public:
  AVL();
  ~AVL();
  bool is_avl();
  void insert(Node *);
  void remove(Node *);
  Node *find(Node);
  void clear(Node *);
  void print_inorder(Node *);
  void print();
};
