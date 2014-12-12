// Size-Balanced Tree, taken from http://blog.tomtung.com/2007/06/size-balanced-tree-in-cpp
// TODO: How does this behave with duplicate keys?
struct SBTNode{
    SBTNode *ch[2],*p;
    long key;
    unsigned long size;
    SBTNode(long _key,unsigned long _size);
}NIL=SBTNode(0,0);
// To make an empty tree: SBTree t = &NIL;
typedef SBTNode *SBTree;
SBTNode::SBTNode(long _key,unsigned long _size=1){
    ch[0]=ch[1]=p=&NIL;
    size=_size;
    key=_key;
}

// Returns node if key is found, &NIL otherwise
SBTNode *SBT_Search(SBTree T,long key);

// Example invocation: SBT_Insert(t, new SBTNode(42));
void SBT_Insert(SBTree &T, SBTNode* x);

// Careful, if no matching key is found, this will still delete a node from the tree.
// The deleted node is not freed, and a pointer to it is returned.
SBTNode *SBT_Delete(SBTree &T, long key);

// Returns first node whose key is strictly less than key, or &NIL if no such node
SBTNode *SBT_Pred(SBTree T, long key);

// Returns first node whose key is strictly greater than key, or &NIL if no such node
SBTNode *SBT_Succ(SBTree T,long key);

// Returns node with ith smallest value (1 <= i <= T->size)
SBTNode *SBT_Select(SBTree T, unsigned long i);

// Returns k such that key is the kth smallest key in the tree (1 <= k <= T->size), or 0 if key not found.
unsigned long SBT_Rank(SBTree T, long key);

inline void SBT_Rotate(SBTree &x,bool flag){
    SBTNode *y=x->ch[!flag];
	// assert(x!=&NIL&&y!=&NIL);
    y->p=x->p;
    x->p=y;
    if(y->ch[flag]!=&NIL) y->ch[flag]->p=x;
    x->ch[!flag]=y->ch[flag];
    y->ch[flag]=x;
    y->size=x->size;
    x->size=x->ch[0]->size+x->ch[1]->size+1;
    x=y;
}
void SBT_Maintain(SBTree &T,bool flag){
    if(T->ch[flag]->ch[flag]->size>T->ch[!flag]->size)
        SBT_Rotate(T,!flag);
    else if(T->ch[flag]->ch[!flag]->size>T->ch[!flag]->size){
        SBT_Rotate(T->ch[flag],flag);
        SBT_Rotate(T,!flag);
    }
    else return;
    SBT_Maintain(T->ch[0],0),SBT_Maintain(T->ch[1],1);
    SBT_Maintain(T,0),SBT_Maintain(T,1);
}
 
SBTNode *SBT_Search(SBTree T,long key){
    return T==&NIL||T->key==key?T:SBT_Search(T->ch[key>T->key],key);
}

void SBT_Insert(SBTree &T, SBTNode* x){
    if(T==&NIL)    T=x;
    else{
        T->size++;
        x->p=T;
        SBT_Insert(T->ch[x->key>T->key],x);
        SBT_Maintain(T,x->key>T->key);
    }
}
 
SBTNode *SBT_Delete(SBTree &T, long key){
    if(T==&NIL)    return &NIL;
    T->size--;
    if(T->key==key||T->ch[key>T->key]==&NIL){
        SBTNode *toDel;
        if(T->ch[0]==&NIL||T->ch[1]==&NIL){
            toDel=T;
            T=T->ch[T->ch[1]!=&NIL];
            if(T!=&NIL)    T->p=toDel->p;
        }else{
            toDel=SBT_Delete(T->ch[1],key-1);
            T->key=toDel->key;
        }
        return toDel;
    }
    else return SBT_Delete(T->ch[key>T->key],key);
}
 
SBTNode *SBT_Pred(SBTree T, long key){
    if(T==&NIL)    return &NIL;
    if(key<=T->key)    return SBT_Pred(T->ch[0],key);
    else{
        SBTNode *pred=SBT_Pred(T->ch[1],key);
        return (pred!=&NIL?pred:T);
    }
}
 
SBTNode *SBT_Succ(SBTree T,long key){
    if(T==&NIL)    return &NIL;
    if(key>=T->key)    return SBT_Succ(T->ch[1],key);
    else{
        SBTNode *succ= SBT_Succ(T->ch[0],key);
        return(succ!=&NIL?succ:T);
    }
}
 
SBTNode *SBT_Select(SBTree T, unsigned long i){
    unsigned long r = T->ch[0]->size+1;
    if(i==r)    return T;
    else return SBT_Select(T->ch[i>r],i>r?i-r:i);
}
 
unsigned long SBT_Rank(SBTree T, long key){
    if(T==&NIL)    return 0;
    if(T->key==key)    return T->ch[0]->size+1;
    else if(key<T->key)    return SBT_Rank(T->ch[0],key);
    else{
        unsigned long r=SBT_Rank(T->ch[1],key);
        return r==0?0:r+T->ch[0]->size+1;
    }
}
