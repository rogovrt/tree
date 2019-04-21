#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct tree_elem {
    int value;
    struct tree_elem* parent;
    struct tree_elem* left;
    struct tree_elem* right;
    int h; //глубина дерева в узле
} tree_elem;

tree_elem* create_elem(int i) { //создание корня дерева
    tree_elem* root = malloc(sizeof(tree_elem));
    root -> value = i;
    root -> parent = NULL;
    root -> left = NULL;
    root -> right = NULL;
    root -> h = 0;
    return root;
}

/*void insert_elem(int i, tree_elem* root) { //вставка итеративно
    tree_elem* pointer = root;
    tree_elem* insert = malloc(sizeof(tree_elem));
    while (insert -> value !=  i){
        if (i >= pointer -> value) {
            if (pointer -> right == NULL) {
                insert -> value = i;
                insert -> left = NULL;
                insert -> right = NULL;
                pointer -> right = insert;
                break;
            }
            else
                pointer = pointer -> right;
        }
        else {
            if (pointer -> left == NULL) {
                insert -> value = i;
                insert -> left = NULL;
                insert -> right = NULL;
                pointer -> left = insert;
                break;
            }
            else
                pointer = pointer -> left;
        }
    }
}*/

void rebalance(tree_elem* insert) { //перебалансировка после вставки нового, вызывается из вставки
    tree_elem* cur = insert;
    while (cur -> parent != NULL) {
        tree_elem* parent = cur -> parent;
        tree_elem* left = parent -> left;
        tree_elem* right = parent -> right;
        /*if ((left -> h > right -> h) || (right == NULL))
            parent -> h = left -> h + 1;
        else
            parent -> h = right -> h + 1;*/
        if (right == NULL)
            parent -> h = left -> h + 1;
        else {
            if (left == NULL)
                parent -> h = right -> h + 1;
            else {
                if (right -> h > left -> h)
                    parent -> h = right -> h + 1;
                else
                    parent -> h = left -> h + 1;
            }
        }
        cur = parent;
    }
}

tree_elem* check_balance(tree_elem* insert) { //правильный check_balance
	int dh;
	tree_elem* parent = insert -> parent;
	tree_elem* left = parent -> left;
	tree_elem* right = parent -> right;
	if ((left != NULL) && (right != NULL))
		dh = left -> h - right -> h;
	else {
		if (left != NULL)
			dh = left -> h;
		else
			dh = - right -> h;
	}
	if ((abs(dh) < 2) && (parent -> parent != NULL))
		check_balance(parent);
	else {
		if (abs(dh) == 2)
			return parent; //требуется поворот
		else {
			return NULL; //дерево сбалансированно
		}
	}
}

void small_right(tree_elem* b) { //может не быть L или C
    //printf("time to small right\n"); //добавить контроль изменения root-а
    int lh;
    int ch;
	tree_elem* a = b -> parent;
	tree_elem* r = a -> right;
	tree_elem* l = b -> left;
	tree_elem* c = b -> right;
	tree_elem* ap = a -> parent;
	if (l == NULL)
        lh = -1;
    else
        lh = l -> h;
    if (c == NULL)
        ch = -1;
    else
        ch = c -> h;
    if (ap != NULL) {
        if (ap -> right == a)
            ap -> right = b;
        else
            ap -> left = b;
    }
	b -> parent = ap;
	b -> right = a;
	if (c != NULL) {
        a -> left = c;
        c -> parent = a;
	}
	if (ch > r-> h)
		a -> h = ch + 1;
	else
		a -> h = r -> h + 1;
	if (lh > a -> h)
		b -> h = lh + 1;
	else
		b -> h = a -> h + 1;
    //printf("success small right\n");
}

void small_left(tree_elem* b) {
    int ch;
    int rh;
    //printf("time to small left\n");
	tree_elem* c = b -> left;
	tree_elem* r = b -> right;
	tree_elem* a = b -> parent;
	tree_elem* l = a -> left;
	tree_elem* ap = a -> parent;
	if (c == NULL)
        ch = -1;
	else
        ch = c -> h;
    if (r == NULL)
        rh = -1;
    else rh = r -> h;
    if (ap != NULL) {
        if (ap -> left == a)
            ap -> left = b;
        else
            ap -> right = b;
    }
	b -> parent = ap;
	a -> parent = b;
	b -> left = a;
	if (c != NULL) {
        a -> right = c;
        c -> parent = a;
	}
	if (l -> h > ch)
		a -> h = l -> h + 1;
	else
		a -> h = ch + 1;
	if (a -> h > rh)
		b -> h = a -> h + 1;
	else
		b -> h = rh + 1;
    //printf("success small left\n");
}

void big_right(tree_elem* b) {
    //printf("time to big right\n");
	tree_elem* c = b -> right;
	small_left(c);
	small_right(c);
}

void big_left(tree_elem* b) {
    //printf("time to big left\n");
	tree_elem* c = b -> left;
	small_right(c);
    small_left(c);
}

void rotate(tree_elem* parent) { //определение типа поворота с вызовом функции необходимого с необходимым пересчетом высот
	if (parent != NULL) {
        tree_elem* left = parent -> left;
        tree_elem* right = parent -> right;
        if (left -> h - right -> h == 2) {
            tree_elem* ll = left -> left;
            tree_elem* lr = left -> right;
            if (ll -> h == lr -> h)
                small_right(left);
            else
                big_right(left);
        }
        if (left -> h - right -> h == -2) {
            tree_elem* rl = right -> left;
            tree_elem* rr = right -> right;
            if (rl -> h == rr -> h)
                small_left(right);
            else
                big_left(right);
        }
    }
    else {
        //printf("there is no rotate, cause tree has already balanced\n");
    }
}

/*void change_root(tree_elem* root) {
    if (root -> parent != NULL)
        root = root -> parent;
    printf("ye %d\n", root -> value);
    //print_tree(root);
}*/

void insert_elem(int i, tree_elem* root) {//вставка рекурсией, можно без функции create_elem??, почему большая такая??
    //tree_elem* rooth = root;
    if (i >= root -> value) {
        if (root -> right != NULL)
            insert_elem(i, root -> right);
        else {
            tree_elem* insert = malloc(sizeof(tree_elem));
            insert -> value = i;
            insert -> parent = root;
            insert -> right = NULL;
            insert -> left = NULL;
            insert -> h = 0;
            root -> right = insert;
            rebalance(insert);
            rotate(check_balance(insert));
            //change_root(root);
        }
    }
    else {
        if (root -> left != NULL)
            insert_elem(i, root -> left);
        else {
            tree_elem* insert = malloc(sizeof(tree_elem));
            insert -> value = i;
            insert -> parent = root;
            insert -> right = NULL;
            insert -> left = NULL;
            insert -> h = 0;
            root -> left = insert;
            rebalance(insert);
            rotate(check_balance(insert));
            //change_root(root);
        }
    }
}

tree_elem* find_close_left(tree_elem* al, int av) { //функция нахождения ближайшего элемента слева; al - указатель налево от удаляемого элемента, av - значение в удаляемом узле
	int delta = av - (al -> value);
	if (delta == 1)
		return al;
	tree_elem* l = al -> left;
	tree_elem* r = al -> right;
	if ((l == NULL) || (r == NULL))
		return al;
	int lv = l -> value;
	int rv = r -> value;
	if (((av - lv) > 0) && ((av - lv) < delta)) {
		delta = av - lv;
		if (delta == 1)
			return l;
	}
	if (((av - rv) > 0) && ((av - rv) < delta)) {
		delta = av - rv;
		if (delta == 1)
			return r;
	}
	tree_elem* backl = find_close_left(l, av);
	tree_elem* backr = find_close_left(r, av);
	if (backl -> value > backr -> value)
		return backl;
	else
		return backr;
}

tree_elem* find_close_right(tree_elem* ar, int av) { //функция нахождения ближайшего элемента справа; ar - указатель направо от удаляемого элемента, av - значение в удаляемом узле
	int delta = (ar -> value) - av;
	if (delta == 0)
		return ar;
	tree_elem* l = ar -> left;
	tree_elem* r = ar -> right;
	int lv = l -> value;
	int rv = r -> value;
	if (((lv - av) >= 0) && ((lv - av) < delta)) {
		delta = lv - av;
		if (delta == 0)
			return l;
	}
	if (((rv - av) >= 0) && ((rv - av) < delta)) {
		delta = rv - av;
		if (delta == 0)
			return r;
	}
	tree_elem* backl = find_close_right(l, av);
	tree_elem* backr = find_close_right(r, av);
	if (backl -> value > backr -> value)
		return backl;
	else
		return backr;
}

void delete(tree_elem* root) {
	if ((root -> left == NULL) && (root -> right == NULL)) { //если вершина "лист", удаление, перебалансировка + соответствующие повороты, если нужно
		tree_elem* parent = root -> parent;
		if (root == parent -> right) {
			parent -> right = NULL;
			free(root);
			tree_elem* left = parent -> left;
			if (left != NULL)
				parent -> h = left -> h + 1;
			else
				parent -> h = 0;
			tree_elem* help = check_balance(parent);
			if (help != NULL)
                rotate(help);
		}
		else {
			parent -> left = NULL;
			free(root);
			tree_elem* right = parent -> right;
			if (right != NULL)
				parent -> h = right -> h + 1;
			else
				parent -> h = 0;
            tree_elem* help = check_balance(parent);
			if (help != NULL)
                rotate(help);
		}
	}
	//черной магией прикрутить check_balance и rebalance
	else {
		tree_elem* l = root -> left; //если вершина не лист, ищу ближайшее значение в наибольшем поддереве
		tree_elem* r = root -> right; //причем в левом ближайший снизу, в правом ближайший сверху
		if (l -> h > r -> h) {
			tree_elem* swap = find_close_left(l, root -> value);
			delete(swap);
		}
		else {
			tree_elem* swap = find_close_right(r, root -> value);
			delete(swap);
		}
	}
}

void find(int value, tree_elem* root) { //функция поиска элементов для удаления, вызов функции удаления
	if (root -> value == value)
		delete(root);
	find(value, root -> left);
	find(value, root -> right);
}

void print_tree(tree_elem* root) { //вывод дерева на экран строкой
    printf("%d %d\n", root -> value, root -> h);
    if (root -> left != NULL) {
        tree_elem* pointer = root -> left;
        print_tree(pointer);
    }
    if (root -> right != NULL) {
        tree_elem* pointer = root -> right;
        print_tree(pointer);
    }
}

int main()
{
    /*int n;
    scanf("%d", &n);
    int a;
    scanf("%d", &a);*/
    tree_elem* root = create_elem(15);
    printf("%d\n", root -> value);
    insert_elem(2, root);
    printf("%d\n", root -> value);
    insert_elem(16, root);
    printf("%d\n", root -> value);
    insert_elem(1, root);
    printf("%d\n", root -> value);
    insert_elem(3, root);
    printf("%d\n", root -> value);
    insert_elem(4, root);
    printf("%d\n", root -> value);
    if (root -> parent == NULL)
        printf("nope");
    print_tree(root);
    return 0;
}
