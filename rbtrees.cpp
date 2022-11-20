#include<bits/stdc++.h>
using namespace std;
struct node{
    struct node* left;
    struct node* right;
    struct node* parent;
    int num;
    int col;
};
void initialise_tree(struct node* head,int k){
    head->col=0;
    head->left=NULL;
    head->right=NULL;
    head->parent= NULL;
    head->num=k;
}
void right_rotate(struct node* pivot){
    pivot->left->parent=pivot->parent;
    if(pivot->parent!=NULL){
        if(pivot->parent->left==pivot){
            pivot->parent->left=pivot->left;
        }else{
            pivot->parent->right=pivot->left;
        }
    }
    pivot->parent=pivot->left;
    pivot->left=pivot->left->right;
    pivot->parent->right=pivot;
    return;
}
void left_rotate(struct node* pivot){
    pivot->right->parent=pivot->parent;
    if(pivot->parent!=NULL){
        if(pivot->parent->left==pivot){
            pivot->parent->left=pivot->right;
        }else{
            pivot->parent->right=pivot->right;
        }
    }
    
    pivot->parent=pivot->right;
    pivot->right=pivot->right->left;
    pivot->parent->left=pivot;
    return;
}
struct node* search_insert(struct node* head,int k){
    if(k>head->num){
        if(head->right==NULL){
            head->right=new struct node();
            initialise_tree(head->right,k);
            head->right->col=1;
            head->right->parent=head;
            return head->right;
        }else{
            return search_insert(head->right,k);
        }
    }else{
        if(head->left==NULL){
            head->left=new struct node();
            initialise_tree(head->left,k);
            head->left->col=1;
            head->left->parent=head;
            return head->left;
        }else{
            return search_insert(head->left,k);
        }
    }
}
void settle_insert(struct node* problem){
    if(problem->parent==NULL){
        problem->col=0;
        return;
    }
    if(problem->parent->col==0){
        return;
    }
    if(problem->parent->parent->left==problem->parent){
        if(problem->parent->parent->right->col==1){
            problem->parent->col=0;
            problem->parent->parent->col=1;
            problem->parent->parent->right->col=0;
            settle_insert(problem->parent->parent);
        }else{
            if(problem->parent->right==problem){
                left_rotate(problem->parent);
                problem=problem->parent;
            }
            problem->parent->col=0;
            problem->parent->parent->col=1;
            right_rotate(problem->parent->parent);
        }
    }else{
        if(problem->parent->parent->left!=NULL && problem->parent->parent->left->col==1){
            problem->parent->col=0;
            problem->parent->parent->col=1;
            problem->parent->parent->left->col=0;
            settle_insert(problem->parent->parent);
        }else{
            if(problem->parent->left==problem){
                // cout<<"hello"<<endl;
                right_rotate(problem->parent);
                problem=problem->right;
                // cout<<problem->parent->parent->num<<endl;
            }
            problem->parent->col=0;
            problem->parent->parent->col=1;
            left_rotate(problem->parent->parent);
            // cout<<problem->parent->left->num<<endl;
        }
    }
    return;
}
struct node* root(struct node* n){
    if(n->parent==NULL){
        return n;
    }else{
        return root(n->parent);
    }
}
struct node* insert_node(struct node* head,int k){
    struct node* target=new struct node();
    target=search_insert(head,k);
    settle_insert(target);
    return root(head);
}
void inorder(struct node* parent){
    if(parent==NULL){
        return;
    }
    inorder(parent->left);
    cout<<parent->num<<" "<<parent->col<<" ";
    inorder(parent->right);
}
struct node* search_delete(struct node* head,int k){
    if(head==NULL){
        return NULL;
    }
    if(head->num==k){
        // cout<<k<<endl;
        return head;
    }
    if(k>head->num){
            return search_delete(head->right,k);
    }else{
            return search_delete(head->left,k);
    }
}
struct node* succ(struct node* head){
    struct node* successor= new struct node();
    successor=head->right;
    while(successor->left!=NULL){
        successor=successor->left;
    }
    return successor;
}
void settle_delete(struct node* problem){
    while(problem->col==0){
        if(problem->parent->left==problem){
            struct node* sib=new struct node();
            sib=problem->parent->right;
            if(sib->col==1){
                problem->parent->col=1;
                sib->col=0;
                left_rotate(problem->parent);
                sib=problem->parent->right;
            }
            if((sib->left->col==0 || sib->left==NULL) && (sib->right->col==0 || sib->right==NULL)){
                sib->col=1;
                problem=problem->parent;
            }else {
                if(sib->left->col==1){
                    sib->left->col=0;
                    sib->col=1;
                    right_rotate(sib);
                    sib=problem->parent->right;
                }
                sib->col=sib->parent->col;
                sib->parent->col=0;
                sib->right->col=0;
                left_rotate(problem->parent);
                break;
            }
        }else{
            struct node* sib=new struct node();
            sib=problem->parent->left;
            if(sib->col==1){
                problem->parent->col=1;
                sib->col=0;
                right_rotate(problem->parent);
                sib=problem->parent->left;
            }
            if((sib->right->col==0 || sib->right==NULL) && (sib->left->col==0 || sib->left==NULL)){
                sib->col=1;
                problem=problem->parent;
            }else {
                if(sib->right->col==1){
                    sib->right->col=0;
                    sib->col=1;
                    left_rotate(sib);
                    sib=problem->parent->left;
                }
                sib->col=sib->parent->col;
                sib->parent->col=0;
                sib->left->col=0;
                right_rotate(problem->parent);
                break;
            }
        }
    }
    problem->col=0;
    return;
}
struct node* delete_node(struct node* head,int k){

    struct node* target=new struct node();
    target=search_delete(head,k);
    if(target==NULL){
        return root(head);
    }
    int nullity=0;
    struct node* pointer=new struct node();
    int color=target->col;
    if(target->left==NULL && target->right==NULL){
        if(target->parent->left==target){
            target->parent->left=new struct node();
            target->parent->left->col=0;
            pointer=target->parent->left;
        }else{
            target->parent->right=new struct node();
            target->parent->right->col=0;
            pointer=target->parent->right;
        }
        pointer->parent=target->parent;
        pointer->left=NULL;
        pointer->right=NULL;
        nullity=1;
    }else if(target->left==NULL){
        pointer=target->right;
        if(target->parent->left==target){
            target->parent->left=target->right;
            target->right->parent=target->parent;
        }else{
            target->parent->right=target->right;
            target->right->parent=target->parent;
        }
    }else if(target->right==NULL){
        pointer=target->left;
        if(target->parent->left==target){
            target->parent->left=target->left;
            target->left->parent=target->parent;
        }else{
            target->parent->right=target->left;
            target->left->parent=target->parent;
        }
    }else{
        struct node* successor=new struct node();
        successor=succ(target);
        color=successor->col;
        target->num=successor->num;
        if(successor->right==NULL){
            if(successor->parent->left==successor){
                successor->parent->left=new struct node();
                successor->parent->left->col=0;
                pointer=successor->parent->left;
            }else{
                successor->parent->right=new struct node();
                successor->parent->right->col=0;
                pointer=successor->parent->right;
            }
            pointer->parent=successor->parent;
            pointer->left=NULL;
            pointer->right=NULL;
            nullity=1;
        }else{
            pointer=successor->right;
            if(successor->parent->left==successor){
                successor->parent->left=successor->right;
                successor->right->parent=successor->parent;
            }else{
                successor->parent->right=successor->right;
                successor->right->parent=successor->parent;
            }
        }
    }
    if(color==0){
        settle_delete(pointer);
    }
    if(nullity){
        // cout<<pointer->parent->num<<endl;
        if(pointer==pointer->parent->left){
            pointer->parent->left=NULL;
        }else{
            pointer->parent->right=NULL;
        }
    }
    return root(head);
}
int main(){
	struct node* tree=new struct node();
    initialise_tree(tree,1);
    tree=insert_node(tree,2);
    tree=insert_node(tree,3);
    tree=insert_node(tree,4);
    tree=insert_node(tree,5);  
    tree=delete_node(tree,1);                                                             
    // left_rotate(tree);
    inorder(tree);
}
