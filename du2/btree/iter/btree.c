/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree)
{
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value)
{
  while (tree != NULL)
  {
    if (key > tree->key)
    {
      if(!tree->right)
        return false;
      tree = tree->right;
    }
    else if (key < tree->key)
    {
      if (!tree->left)
        return false;
      tree = tree->left;
    }
    else
    {
      *value = tree->value;
      return true;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value)
{
  bst_node_t *tmp = NULL;
  bst_node_t *create_this = *tree;
  
  while (create_this != NULL)
  {
    tmp = create_this;
    if(key == create_this->key){
      create_this->value = value;
      return;
    }

    else if (key < create_this->key)
    {
      create_this = create_this->left;
    }
    else if (key > create_this->key)
    {
      create_this = create_this->right;
    }
    else
    {
      return;
    }
  }

  bst_node_t *create_new = malloc(sizeof(struct bst_node));
  if (create_new == NULL)
  {
    return;
  }
  // strom je prazdny
  create_new->key = key;
  create_new->value = value;
  create_new->left = NULL;
  create_new->right = NULL;

  if (tmp == NULL)
  {
    *tree = create_new;
    return;
  }
  if (key < tmp->key)
  {
    tmp->left = create_new;
    return;
  }
  tmp->right = create_new;
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  bst_node_t *before_node = target;
  bst_node_t *right_tree = *tree;

  while (right_tree->right)
  {
    before_node = right_tree;
    right_tree = right_tree->right;
  }
  target->key = right_tree->key;
  target->value = right_tree->value;
  if(before_node->key == right_tree->key){
    before_node->left = right_tree->left;
  }else
  {
   before_node->right = right_tree->left; 
  }
  free(right_tree->right);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key)
{
  if (tree == NULL)
  {
    return;
  }
  if (!(*tree)->key || !(*tree)->value)
  {
    return;
  }
  bst_node_t *node = (*tree);
  bst_node_t *parent;
  bool isnotfound = true;
  if (node->key == key)
  {
    if ((node->left == NULL) &&(node->right == NULL))
      {
        *tree = NULL;
        }
        else if (node->left == NULL)
        {
          *tree = node->right;
        }
        else if (node->right == NULL)
        {
          *tree = node->left;
        }
        else
        {
          bst_replace_by_rightmost(node, &node->left);
        }
      }
  else{
  while (isnotfound)
  {
    
    if (key > node->key)
    {
      if(!node->right)
        return;
      parent = node;
      node = node->right;
    }
    else if (key < node->key)
    {
      if(!node->left)
        return;
      parent = node;
      node = node->left;
    }
    else if (node->key == key)
    {
      
      isnotfound = false;
    }
  }

  if (!node->right && node->left)
  {
    if (parent->right == node)
    {
      parent->right = node->left;
    }
    else
    {
      parent->left = node->left;
    }
  }
  else if (node->right && !node->left)
  {
    if (parent->right == node)
    {
      parent->right = node->right;
    }
    else
    {
      parent->left = node->right;
    }
  }
  else if (node->right && node->left)
  {
    if (parent->right == node)
    {
      bst_replace_by_rightmost(parent->right, &node->left);
    }
    else
    {
      bst_replace_by_rightmost(parent->left, &node->left);
    }
    return;
  }
  else if (!node->right && !node->left) 
  {
    if (parent->right == node)
    {
      parent->right = NULL;
    }
    else
    {
      parent->left = NULL;
    }
  }
  
  else
  {
    return;
  }
  
  free(node);
  }
  
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree)
{
  if (*tree == NULL)
  {
    return;
  }

  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bst_push(&stack, *tree);

  while (!stack_bst_empty(&stack))
  {
    bst_node_t *delete = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    if (delete->left != NULL)
    {
      stack_bst_push(&stack, delete->left);
    }
    if (delete->right != NULL)
    {
      stack_bst_push(&stack, delete->right);
    }
    free(delete);
  }
  bst_init(tree);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    bst_print_node(tree);
    tree = tree->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree)
{
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_preorder(tree, &stack);
  while (!stack_bst_empty(&stack))
  {
    tree = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_leftmost_preorder(tree->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree)
{
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_inorder(tree, &stack);
  while (!stack_bst_empty(&stack))
  {
    tree = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree)
{
  stack_bst_t stack;
  stack_bst_init(&stack);
  bool fromleft;
  stack_bool_t stackBool;
  stack_bool_init(&stackBool);
  bst_leftmost_postorder(tree, &stack, &stackBool);
  while (!stack_bst_empty(&stack))
  {
    tree = stack_bst_top(&stack);
    fromleft = stack_bool_top(&stackBool);
    stack_bool_pop(&stackBool);
    if (fromleft)
    { //pridem z prava a pojdem doprava
      stack_bool_push(&stackBool, false);
      bst_leftmost_postorder(tree->right, &stack, &stackBool);
    }
    else //z prava odstrani a zpracuje uzol
    {
      stack_bst_pop(&stack);
      bst_print_node(tree);
    }
  }
}
