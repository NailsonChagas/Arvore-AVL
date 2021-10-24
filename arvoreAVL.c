#define INT_MAX 2147483647
#define INT_MIN -2147483648

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "arvoreAVL.h"

typedef struct noh{
    int item, altura;
    NohArvoreAVL *left, *right;
}NohArvoreAVL;

static int maior_valor(int a, int b){
    if(a > b){
        return a;
    }
    return b;
}

static NohArvoreAVL* alocar_noh(int itemEntrada){
    NohArvoreAVL *a = NULL;

    a = (NohArvoreAVL*)malloc(sizeof(NohArvoreAVL));

    a->item = itemEntrada;
    a->left = a->right = NULL;
    a->altura = 1; //começa como folha

    return a;
}

static int altura_noh(NohArvoreAVL *entrada){
    if(entrada == NULL){
        return 0;
    }
    return entrada->altura;
}

/*Rotacionar para direita*/
static NohArvoreAVL* rotacionar_right(NohArvoreAVL *raizArvore){
    NohArvoreAVL *auxA, *auxB;

    auxA = raizArvore->left;
    auxB = auxA->right;

    //realizar rotação
    auxA->right = raizArvore;
    raizArvore->left = auxB;

    //atualizar alturas
    raizArvore->altura = maior_valor(altura_noh(raizArvore->left), altura_noh(raizArvore->right)) + 1;
    auxA->altura = maior_valor(altura_noh(auxA->left), altura_noh(auxA->right)) + 1;

    //retornar a nova raiz
    return auxA; 
}

/*Rotacionar para esquerda*/
static NohArvoreAVL* rotacionar_left(NohArvoreAVL *raizEntrada){
    NohArvoreAVL *auxA, *auxB;

    auxA = raizEntrada->right;
    auxB = auxA->left;

    //realizar rotação
    auxA->left = raizEntrada;
    raizEntrada->right = auxB;

    //atualizar alturas 
    raizEntrada->altura = maior_valor(altura_noh(raizEntrada->left), altura_noh(raizEntrada->right)) + 1;
    auxA->altura = maior_valor(altura_noh(auxA->left), altura_noh(auxA->right)) + 1;

    return auxA;
}

static int balanceamento_noh(NohArvoreAVL *entrada){
    if(entrada == NULL){
        return 0;
    }

    int balanceamento = altura_noh(entrada->left) - altura_noh(entrada->right);

    return balanceamento;
}

static NohArvoreAVL* inserir(NohArvoreAVL *raiz, int item){
    int balanceamento;

    /*Inserção em árvore binaria de busca*/
    if(raiz == NULL){//caso a raiz n exista
        NohArvoreAVL *novo = NULL;

        novo = alocar_noh(item);
        
        return novo;
    }

    if(item < raiz->item){
        raiz->left = inserir(raiz->left, item);
    }
    else if(item > raiz->item){
        raiz->right = inserir(raiz->right, item);
    }
    else{//itens(chaves iguais não são permitidas)
        return raiz;
    }

    /*atualizar altura da raiz*/
    raiz->altura = 1 + maior_valor(altura_noh(raiz->left), altura_noh(raiz->right));

    /*obter o fator de balanceamento*/
    balanceamento = balanceamento_noh(raiz);

    /*caso esteja desbalanceado, existem quatro operações que podem vir a ser necessarias*/
    //left left:
    if(balanceamento > 1 && item < raiz->left->item){
        return rotacionar_right(raiz);
    }
    //right right 
    if(balanceamento < -1 && item > raiz->right->item){
        return rotacionar_left(raiz);
    }
    //left right
    if(balanceamento > 1 && item > raiz->left->item){
        raiz->left = rotacionar_left(raiz->left);
        return rotacionar_right(raiz);
    }
    //right left
    if(balanceamento < -1 && item < raiz->right->item){
        raiz->right = rotacionar_right(raiz->right);
        return rotacionar_left(raiz);
    }

    /*Caso nenhum dos casos seja aplicado*/
    return raiz;
}

static NohArvoreAVL* menor_noh(NohArvoreAVL *raiz){
    NohArvoreAVL *atual = raiz;

    while(atual->left != NULL){
        atual = atual->left;
    }

    return atual;
}

static NohArvoreAVL* remover(NohArvoreAVL *raiz, int item){
    /*Passo 1: remover da arvore bin*/
    if(raiz == NULL){
        return raiz;
    }

    if(item < raiz->item){
        raiz->left = remover(raiz->left, item);
    }
    else if(item > raiz->item){
        raiz->right = remover(raiz->right, item);
    }
    else{//noh encontrado
        if(raiz->left==NULL || raiz->right==NULL){//Nó com nenhum ou um descendente
            NohArvoreAVL *aux = NULL;
        
            if(raiz->left == NULL){
                aux = raiz->right;
            }
            else if(raiz->right == NULL){
                aux = raiz->left;
            }

            if(aux == NULL){//caso não possua descendente
                aux = raiz;
                raiz = NULL;
            }
            else{//caso possua um descendente
                *raiz = *aux; //copiando valor de uma struct para outra
                free(aux);
            }
        }
        else{//dois decendentes
            NohArvoreAVL *aux = menor_noh(raiz->right); //sucessor: menor da arvore a direita

            raiz->item = aux->item;
            
            raiz->right = remover(raiz->right, aux->item);
        }
    }
    if(raiz == NULL){ //se a arvore ficar vazia
        return raiz;
    }

    /*Passo 2: atualizar altura*/
    raiz->altura = 1 + maior_valor(altura_noh(raiz->left), altura_noh(raiz->right));

    /*Passo 3: achar o balanceamento desse nó*/
    int balanceamento = balanceamento_noh(raiz);

    //left left
    if(balanceamento > 1 && balanceamento_noh(raiz->left) >= 0){
        return rotacionar_right(raiz);
    }
    //left right
    if(balanceamento > 1 && balanceamento_noh(raiz->left) < 0){
        raiz->left = rotacionar_left(raiz->left);
        return rotacionar_right(raiz);
    }
    //right right
    if(balanceamento < -1 && balanceamento_noh(raiz->right) <= 0){
        return rotacionar_left(raiz);
    }
    //right left
    if(balanceamento < -1 && balanceamento_noh(raiz->right) > 0){
        raiz->right = rotacionar_right(raiz->right);
        return rotacionar_left(raiz);
    }

    return raiz;
}

static void desalocar_arvore_bin(NohArvoreAVL *raiz){
    if(raiz != NULL){
        desalocar_arvore_bin(raiz->left);
        desalocar_arvore_bin(raiz->right);
        free(raiz);
    }
}

static void imprimir_posfix(NohArvoreAVL *raiz){
    if(raiz != NULL){
        printf("%d ", raiz->item);
        imprimir_posfix(raiz->left);
        imprimir_posfix(raiz->right);
    }
}

struct arv{
    NohArvoreAVL *raiz;
};

typedef struct arv ArvoreAVL;

ArvoreAVL* alocar_arvore_AVL(){//funciona
    ArvoreAVL *a = NULL;

    a = (ArvoreAVL*)malloc(sizeof(ArvoreAVL));
    a->raiz = NULL;

    return a;
}

bool inserir_elemento_arvore_AVL(ArvoreAVL *arvore, int itemEntrada){//funciona
    if(arvore != NULL){
        arvore->raiz = inserir(arvore->raiz, itemEntrada);

        return true;
    }
    return false;
}

void imprimir_posfix_arvore_AVL(ArvoreAVL *arvore){//funciona
    if(arvore != NULL){
        imprimir_posfix(arvore->raiz);
        printf("\n");
    }
}

bool remover_elemento_arvore_AVL(ArvoreAVL *arvore, int itemRemover){//funciona
    if(arvore != NULL){
        arvore->raiz = remover(arvore->raiz, itemRemover);
        return true;
    }
    return false;
}

bool desalocar_arvore_AVL(ArvoreAVL *arvore){
    if(arvore != NULL){
        if(arvore->raiz != NULL){
            desalocar_arvore_bin(arvore->raiz);
        }
        free(arvore);
        return true;
    }
    return false;
}