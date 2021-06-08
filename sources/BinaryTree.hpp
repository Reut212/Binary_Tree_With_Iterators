#include <ostream>
#include <vector>

namespace ariel{
    
    template <typename T> class BinaryTree{
        
        private:
        struct Node{
            T value;
            Node* right;
            Node* left;
            Node(const T& v): value(v), right(nullptr), left(nullptr){}
            
            Node(const Node& other): value(other.value), right(nullptr), left(nullptr){
                this->left = other.left;
                this->right = other.right;
                this->value = other.value;
            }

            //https://codereview.stackexchange.com/questions/91132/a-binary-tree-its-copy-constructor-and-assignment-operator
            Node& operator=(const Node other) noexcept{
                if(this == other) {return *this;}
                delete right;
                delete left;
                this->right = other.right;
                this->left = other.left;
                this->value = other.value;
                return *this;
            }

            Node(Node&& other) noexcept{ //Move consructor
                this->right = other.right;
                this->left = other.left;
                this->value = other.value; 
                other.right = nullptr;
                other.left = nullptr;
                other.value = nullptr;
            }

            Node& operator=(Node&& other) noexcept{    
                delete left;
                delete right;
                this->right = other.right;
                this->left = other.left;
                this->value = other.value; 
                other.right = nullptr;
                other.value = nullptr;
                other.left = nullptr;            
                return *this;
            }                                   

            ~Node(){ //Destructor
                if (left != nullptr )
                {
                    delete left;
                }
                if (right != nullptr)
                {
                    delete right;
                }
            }

            bool LeftOrRight(int i){
                if(i == 1){
                    return left != nullptr;
                }
                if(i == 2){
                    return right!= nullptr;
                }
            }

        };
        
        Node* root;

        public:
        BinaryTree():root(nullptr){}

        BinaryTree(const BinaryTree &newTree){
            if(newTree.root != NULL){
                root = new Node(newTree.root->value);
                copyOfTree(*newTree.root, *root);
            }
        }
        
        ~BinaryTree(){}

        BinaryTree<T>& operator=(BinaryTree othertree){
            if(this == &othertree){
                return *this;
            }
            if(root != NULL){
                delete this->root;
            }
            root = new Node(othertree.root->value);
            copyOfTree(*othertree.root,*this->root);
            return *this;
            
        }

        BinaryTree<T>(BinaryTree<T> &&other) noexcept: root(std::move(other.root)) { //Move consructor
        }
        BinaryTree<T>& operator=(BinaryTree&& newTree)noexcept{
            *this->root = newTree.root;
        }

        BinaryTree const& add_root(const T& new_root_val){
            if(this->root == nullptr){
                this->root = new Node(new_root_val);
            }else{
                this->root->value= new_root_val;
            }
            return *this;
        }

        Node *found_node(Node *node, T value){
            if (node == nullptr){return nullptr;}
            for (auto it = begin_inorder(); it != end_inorder(); it++){
                if (*it == value){return it.currNode();}
            }
            return nullptr;
        }

        BinaryTree& add_left(const T& node_exist_in_tree, const T& add_new_left_child){
            Node* exist = found_node(this->root, node_exist_in_tree);
            if(exist == nullptr) {throw std::invalid_argument("the node is not exist");}
            if(exist->left == nullptr){
                exist->left = new Node(add_new_left_child);
                
            }else{
                exist->left->value = add_new_left_child;
            }
            return *this;
        }

        BinaryTree& add_right(const T& exist_in_tree, const T& add_new_right_child){
            Node* exist = found_node(this->root, exist_in_tree);
            if(exist == nullptr) {throw std::invalid_argument("the node is not exist");}
            if(exist->right == nullptr){
                exist->right = new Node(add_new_right_child);
                
            }else{
                exist->right->value = add_new_right_child;
            }
            return *this;
        }
        
        friend std::ostream& operator<<(std::ostream& out, BinaryTree const& b){
            return out;
        }

        class iterator{

            std::vector<Node*> v;
            Node* current_node= nullptr;

            public:
            iterator():current_node(nullptr){}
            iterator(Node* n):current_node(n){}
            iterator(Node* runner, const std::string& order){
                if(runner != nullptr) {
                createVector(runner, order);
                this->current_node = v[0];
                }else{
                    this->current_node = nullptr;
                }
            }
            
            void createVector(Node* runner,const std::string& order){
                if(runner == nullptr){return;}
                if(order == "PREORDER"){
                    v.push_back(runner);
                }
                createVector(runner->left, order);
                if(order == "INORDER"){
                    v.push_back(runner);
                }
                createVector(runner->right, order);
                if(order == "POSTORDER"){
                    v.push_back(runner);
                }
            }
            
            Node* topNode(){return v[0];}
            Node* currNode(){return current_node;}
            T* operator->() const {return &(current_node->value);}
            T& operator*() const {return current_node-> value;}

            iterator& operator++() {
                if(v.size()>1){
                    v.erase(v.begin());
                    current_node = v[0];
                }
                else{
                    current_node = nullptr;
                }
            return *this;
            }

            iterator operator++(int) {
                iterator it(v[0]);
                ++*this; 
                return it;
            }

            bool operator==(const iterator& rhs) const {return current_node == rhs.current_node;}
            bool operator!=(const iterator& rhs) const {return current_node != rhs.current_node;}
        
        };

        void copyOfTree(Node& from,Node& to){
            if(from.left != NULL){
                Node* copy = new Node(from.left->value);
                to.left = copy;
                Node& left = *(from).left;
                copyOfTree(left, *to.left);
            }
            if(from.right != NULL){
                Node* copy = new Node(from.right->value);
                to.right = copy;
                Node& right = *(from).right;
                copyOfTree(right, *to.right);
            }
        }

        // BinaryTree<T>& getCopyOfTree(Node*root,Node* moshikRoot){
        //     BinaryTree<T> moshik;
        //     root = this->root;
        //     moshikRoot = moshik.root;
        //     if(this->root==nullptr) {return;
        //     }else{
        //         moshikRoot = root;
        //         getCopyOfTree(root->left, moshikRoot->left);
        //         getCopyOfTree(root->right, moshikRoot->right);
        //     }
        //     return *moshik;
        // }
        
        iterator begin_preorder() {return iterator{root, "PREORDER"};}
        iterator end_preorder() {return iterator{};}
        iterator begin_inorder(){return iterator{root, "INORDER"};}
        iterator end_inorder(){return iterator{};}
        iterator begin_postorder(){return iterator{root, "POSTORDER"};}
        iterator end_postorder(){return iterator{};}

        iterator begin(){ return iterator{root, "INORDER"};}
        iterator end() {return iterator{};}
    };
    
}