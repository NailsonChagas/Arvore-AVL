typedef struct noh NohArvoreAVL;
typedef struct arv ArvoreAVL;

ArvoreAVL* alocar_arvore_AVL();
bool inserir_elemento_arvore_AVL(ArvoreAVL *arvore, int itemEntrada);
void imprimir_posfix_arvore_AVL(ArvoreAVL *arvore);
bool remover_elemento_arvore_AVL(ArvoreAVL *arvore, int itemRemover);
bool desalocar_arvore_AVL(ArvoreAVL *arvore);