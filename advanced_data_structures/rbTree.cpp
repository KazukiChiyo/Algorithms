template <class K, class V>
void rbTree<K, V>::rotateLeft(Node* t) {
    Node* temp = t->right;
    t->right = temp->left;
    if (temp->left != nil)
        temp->left->parent = t;
    temp->parent = t->parent;
    if (t->parent == nil) // t is root
        root = temp;
    else if (t == t->parent->left) // t is parent's left child
        t->parent->left = temp;
    else // t is parent's right child
        t->parent->right = temp;
    temp->left = t;
    t->parent = temp;
}

template <class K, class V>
void rbTree<K, V>::rotateRight(Node* t) {
    Node* temp = t->left;
    t->left = temp->right;
    if (temp->right != nil)
        temp->right->parent = t;
    temp->parent = t->parent;
    if (t->parent == nil) // t is root
        root = temp;
    else if (t == t->parent->left) // t is parent's left child
        t->parent->left = temp;
    else // t is parent's right child
        t->parent->right = temp;
    temp->right = t;
    t->parent = temp;
}

template <class K, class V>
void rbTree<K, V>::insert(const K & key, const V & value) {
    Node* newNode = new Node(key, value);
    Node* prev = nil;
    Node* curr = root;
    // iterates to the proper place
    while (curr != nil) {
        prev = curr;
        if (newNode->key < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    newNode->parent = prev;
    if (prev == nil) // if tree is previously empty
        root = newNode;
    else if (newNode->key < prev->key) // if new node should insert to the left
        prev->left = newNode;
    else // if new node should insert to the right
        prev->right = newNode;
    newNode->left = nil;
    newNode->right = nil;
    newNode->color = red;
    insertFixup(newNode);
    _size++;
}

template <class K, class V>
void rbTree<K, V>::insertFixup(Node* newNode) {
    Node* uncle = NULL;
    while (newNode->parent->color == red) { // newNode and its parents are both red
        if (newNode->parent == newNode->parent->parent->left) { // if parent is left child
            uncle = newNode->parent->parent->right;
            if (uncle->color == red) {
                newNode->parent->color = black;
                uncle->color = black;
                newNode->parent->parent->color = red;
                newNode = newNode->parent->parent;
            }
            else {
                if (newNode == newNode->parent->right) { // if node is right child
                    rotateLeft(newNode->parent);
                    newNode = newNode->parent;
                }
                newNode->parent->color = black;
                newNode->parent->parent->color = red;
                rotateRight(newNode->parent->parent);
            }
        }
        else { // if parent is right child
            uncle = newNode->parent->parent->left;
            if (uncle->color == red) {
                newNode->parent->color = black;
                uncle->color = black;
                newNode->parent->parent->color = red;
                newNode = newNode->parent->parent;
            }
            else {
                if (newNode == newNode->parent->left) { // if node is left child
                    newNode = newNode->parent;
                    rotateRight(newNode);
                }
                newNode->parent->color = black;
                newNode->parent->parent->color = red;
                rotateLeft(newNode->parent->parent);
            }
        }
    }
    root->color = black; // set root to black
}

template <class K, class V>
V rbTree<K, V>::find(const K& key) const {
    Node* curr = root;
    while (curr != NULL) {
        if (key == curr->key)
            return curr->value;
        else if (key < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return V();
}

template <class K, class V>
typename rbTree<K, V>::Node* rbTree<K, V>::getNode(const K& key) const {
    Node* curr = root;
    while (curr != nil) {
        if (key == curr->key)
            return curr;
        else if (key < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return nil;
}

template <class K, class V>
void rbTree<K, V>::replace(Node*& u, Node*& v) {
    if (u->parent == nil) // if root of the tree
        root = v;
    else if (u == u->parent->left) // if u is a left subtree
        u->parent->left = v;
    else // if u is a right subtree
        u->parent->right = v;
    v->parent = u->parent;
}

template <class K, class V>
void rbTree<K, V>::deleteFixup(Node* target) {
    Node* sibling = NULL;
    while (target != root && target->color == black) {
        if (target == target->parent->left) { // if node is in left subtree
            sibling = target->parent->right; // find sibling
            if (sibling->color == red) {
                sibling->color = black;
                target->parent->color = red;
                rotateLeft(target->parent);
                sibling = target->parent->right;
            }
            if (sibling->left->color == black && sibling->right->color == black) {
                sibling->color = red;
                target = target->parent;
            }
            else {
                if (sibling->right->color == black) {
                    sibling->left->color = black;
                    sibling->color = red;
                    rotateRight(sibling);
                    sibling = target->parent->right;
                }
                sibling->color = target->parent->color;
                target->parent->color = black;
                sibling->right->color = black;
                rotateLeft(target->parent);
                target = root;
            }
        }
        else { // if node is in right subtree
            sibling = target->parent->left; // find sibling
            if (sibling->color == red) {
                sibling->color = black;
                target->parent->color = red;
                rotateRight(target->parent);
                sibling = target->parent->left;
            }
            if (sibling->left->color == black && sibling->right->color == black) {
                sibling->color = red;
                target = target->parent;
            }
            else {
                if (sibling->left->color == black) {
                    sibling->right->color = black;
                    sibling->color = red;
                    rotateLeft(sibling);
                    sibling = target->parent->left;
                }
                sibling->color = target->parent->color;
                target->parent->color = black;
                sibling->left->color = black;
                rotateRight(target->parent);
                target = root;
            }
        }
    }
    target->color = black;
}

template <class K, class V>
void rbTree<K, V>::remove(const K & key) {
    Node* target = getNode(key);
    if (target == nil) // if key does not exist
        return;
    Node* ios = NULL;
    Node* fixPoint = NULL;
    bool targetColor = target->color;
    if (target->left == nil) {
        replace(target, target->right);
        fixPoint = target->right; // set fix point
        delete target;
    }
    else if (target->right == nil) {
        replace(target, target->left);
        fixPoint = target->left; // set fix point
        delete target;
    }
    else {
        ios = getMinNode(target->right); // find in-order successor
        targetColor = ios->color;
        if (ios->parent != target) {
            replace(ios, ios->right);
            ios->right = target->right;
            ios->right->parent = ios;
        }
        else
            ios->right->parent = ios;
        replace(target, ios);
        ios->left = target->left;
        ios->left->parent = ios;
        ios->color = target->color;
        fixPoint = ios->right; // set fix point
        delete target;
    }
    if (targetColor == black)
        deleteFixup(fixPoint);
    _size--;
}

template <class K, class V>
typename rbTree<K, V>::Node* rbTree<K, V>::getMinNode(Node* root) const {
    while (root->left != nil)
        root = root->left;
    return root;
}

template <class K, class V>
typename rbTree<K, V>::Node* rbTree<K, V>::getMaxNode(Node* root) const {
    while (root->right != nil)
        root = root->right;
    return root;
}