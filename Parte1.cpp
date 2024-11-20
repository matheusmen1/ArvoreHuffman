#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <ctype.h>
#include "TADPilha.h"
struct bits
{
	unsigned char b7:1; // - significativo
	unsigned char b6:1;
	unsigned char b5:1;
	unsigned char b4:1;
	unsigned char b3:1;
	unsigned char b2:1;
	unsigned char b1:1;
	unsigned char b0:1;// + significativo
};
union byte
{
	struct bits bi;
	unsigned char num;	
};
typedef union byte Byte;

struct floresta{
	Tree *no;
	floresta *prox;
};
typedef struct floresta Floresta;

struct tabela{
	char palavra[30], codigo[8];
	int simbolo;
	float freq;
};
typedef struct tabela Tabela;

struct lista{
	Tabela tabela;
	struct lista *prox;
};
typedef struct lista Lista;

Lista *criaNoLista(char *palavra)
{
	Lista *Caixa;
	Caixa = (Lista*)malloc(sizeof(Lista));
	strcpy(Caixa->tabela.palavra, palavra);
	Caixa->prox = NULL;
	Caixa->tabela.simbolo = 0;
	Caixa->tabela.freq = 0;
	strcpy(Caixa->tabela.codigo, " ");
	
	return Caixa;
}
Tree *criaNoArvore(Lista *L)
{
	Tree *Caixa;
	Caixa = (Tree*)malloc(sizeof(Tree));
	Caixa->dir = NULL;
	Caixa->esq = NULL;
	Caixa->freq = L->tabela.freq;
	Caixa->simbolo = L->tabela.simbolo;
	
	return Caixa;
	
}
Floresta *criaNoFloresta(Tree *raiz)
{
	Floresta *Caixa;
	Caixa = (Floresta*)malloc(sizeof(Floresta));
	Caixa->no = raiz;
	Caixa->prox = NULL;
	
	return Caixa;
	
}
void lerFrase(Lista **no, char *frase, int &contPalavras)
{
		
	int i = 0, j =0, num = 0, contEspacos =0;
	char auxPalavra[25];
	Lista *aux; 
	while(frase[i] != '\0')
	{

		if (frase[i] != ' ')
		{
			auxPalavra[j] = frase[i];
			j++;	
		}
		else
		{
			// caso de espaço
			aux = *no;
			while(aux != NULL && strcmp(aux->tabela.palavra, " ")) // busca o espaco
				aux = aux->prox;
			if (aux == NULL) // nao achou, insere
			{
				Lista *NC;
				NC = criaNoLista(" ");
				NC->tabela.simbolo = num;
				NC->tabela.freq++;
				if (*no == NULL)
				{
					*no = NC;
				}
				else // inserção no final
				{
					aux = *no;	
					while(aux->prox != NULL)
						aux = aux->prox;
					aux->prox = NC;
					
				}
				num++;
			}
			else
			{
				aux->tabela.freq++;
				
			}		
			contEspacos++;
			auxPalavra[j] = '\0';
			
			aux = *no;
			while(aux != NULL && strcmp(aux->tabela.palavra, auxPalavra)) // busca palavra
				aux = aux->prox;
			if (aux == NULL) // nao achou, insere
			{
				Lista *NC;
				NC = criaNoLista(auxPalavra);
				NC->tabela.simbolo = num;
				NC->tabela.freq++;
				if (*no == NULL)
				{
					*no = NC;
				}
				else // inserção no final
				{
					aux = *no;	
					while(aux->prox != NULL)
						aux = aux->prox;
					aux->prox = NC;
					
				}
					
				num++;
			}
			else
			{
				aux->tabela.freq++;
			
				
			}			
			auxPalavra[0] = '\0';
			j = 0;
			contPalavras++;
			
		}
		
		i++;
	}
	if (j > 0)
	{
		auxPalavra[j] = '\0';
			
		aux = *no;
		while(aux != NULL && strcmp(aux->tabela.palavra, auxPalavra)) // busca palavra
			aux = aux->prox;
		if (aux == NULL) // nao achou, insere
		{
			Lista *NC;
			NC = criaNoLista(auxPalavra);
			NC->tabela.simbolo = num;
			NC->tabela.freq++;
			if (*no == NULL)
			{
				*no = NC;
			}
			else // inserção no final
			{
				aux = *no;	
				while(aux->prox != NULL)
					aux = aux->prox;
				aux->prox = NC;
				
			}
				
			num++;
		}
		else
			aux->tabela.freq++;
		
		contPalavras++;		
	}
	contPalavras = contPalavras + contEspacos;
}

void calcFreq(Lista **no, int contPalavras)
{
	if (*no != NULL)
	{
		Lista *aux = *no;
		while(aux != NULL)
		{
			aux->tabela.freq = aux->tabela.freq / contPalavras;
			aux = aux->prox;
		}
	
	}
	
}

void InserirFloresta(Floresta **F, Lista *L)
{
	if (L != NULL)
	{
		while (L != NULL)
		{
			Floresta *NC;
			Tree *T;
			T = criaNoArvore(L);
			NC = criaNoFloresta(T);
			if (*F == NULL)
				*F = NC;
			else
			if((*F)->no->freq > NC->no->freq)
			{
				NC->prox = *F;
				*F = NC;
				
			}
			else
			{
				Floresta *aux;
				aux = *F;
				while(aux->prox != NULL && aux->prox->no->freq < NC->no->freq)
					aux = aux->prox;
				NC->prox = aux->prox;
				aux->prox = NC;
				
			}
			L = L->prox;
		}
		
	
	}
	
	
}
void construirArvore(Floresta **F)
{
	Floresta *primeiro, *segundo,*aux, *NCFloresta;
	Tree *NC;
	while((*F)->prox != NULL)
	{
		primeiro = *F;
		segundo = (*F)->prox;
		NC = (Tree*)malloc(sizeof(Tree));
		NC->freq = primeiro->no->freq + segundo->no->freq;
		NC->esq = primeiro->no;
		NC->dir = segundo->no;
		NC->simbolo = -1;
		(*F) = segundo->prox;
		free(primeiro);
		free(segundo);
		NCFloresta = criaNoFloresta(NC);
		if (*F == NULL)
			*F = NCFloresta;
		else
		if((*F)->no->freq > NCFloresta->no->freq)
		{
			NCFloresta->prox = *F;
			*F = NCFloresta;
		} 
		else
		{
			aux = *F;
			while(aux->prox != NULL && aux->prox->no->freq < NCFloresta->no->freq)
				aux = aux->prox;
			NCFloresta->prox = aux->prox;
			aux->prox = NCFloresta;
		}
	}
}

void gerarCodigoHuffman(Tree *raiz, Lista *L)
{
	Lista *aux;
	char codigoAtual[8], novoCodigo[8];
	int i = 0;
	Pilha *P;
	init(&P);
	push(&P, raiz, "");
	while(!isEmpty(P))
	{
		pop(&P, &raiz, codigoAtual); // substitui o valor
		if (raiz->dir == NULL && raiz->esq == NULL) // folha
		{
			aux = L;
			while(aux != NULL && aux->tabela.simbolo != raiz->simbolo) // percorre até encontrar na tabela o simbolo correspondente, adicionando o codigo 
				aux = aux->prox;
			if (aux != NULL)
			{
				strcpy(aux->tabela.codigo, codigoAtual);
				//printf ("codigo: %s simbolo: %d\n", aux->tabela.codigo, aux->tabela.simbolo);
			}
			
		}
		else
		{
			if(raiz ->dir != NULL)
			{
				novoCodigo[0] = '\0'; // zera o valor
				strcpy(novoCodigo, codigoAtual); 
				strcat(novoCodigo, "1"); // concatena 
				push(&P, raiz->dir, novoCodigo); // empilha 		
				
			}
			
			if(raiz->esq != NULL)
			{
				novoCodigo[0] = '\0';
				strcpy(novoCodigo, codigoAtual);
				strcat(novoCodigo, "0");
				push(&P, raiz->esq, novoCodigo);
			}
		}
		
	}
}
void codificarFrase (Lista *L, char *novaFrase, char *fraseCodificada)
{
	Lista *aux;
	char palavra[25];
	int i =0, j = 0, qtde;
	while(novaFrase[i] != '\0')
	{
		if (novaFrase[i] != ' ')
		{
			palavra[j] = novaFrase[i];
			j++;
		}	
		else // achou espaco e uma palvra chegou ao '\0'
		{
			palavra[j] = '\0';
			aux = L;
			while(aux != NULL && strcmp(aux->tabela.palavra, palavra) != 0)
				aux = aux->prox;
			if (aux != NULL)
			{
				strcat(fraseCodificada, aux->tabela.codigo);
			}	
						
			aux = L;
			while(aux != NULL && strcmp(aux->tabela.palavra, " ") != 0)
				aux = aux->prox;
			if (aux != NULL) // achou o espaco
			{
				strcat(fraseCodificada, aux->tabela.codigo);
				
			}
			palavra[0] = '\0';
			j = 0;
			
		}
		i++;
	
	}
	if (j > 0) // ultima palavra
	{
		palavra[j] = '\0';
		aux = L;
		while(aux != NULL && strcmp(aux->tabela.palavra, palavra) != 0)
			aux = aux->prox;
		if (aux != NULL)
		{
			strcat(fraseCodificada, aux->tabela.codigo);
		}	
		
	}
	
	qtde = strlen(fraseCodificada)%8;
	if (qtde != 0)
	{
		qtde = 8 - qtde;
		while(qtde > 0)
		{
			strcat(fraseCodificada, "0");
			qtde--;
		}
		
	}
	
}
void armazenarFraseCodificada(char *fraseCodificada)
{
	int i =0;
	Byte u;
	FILE * Ptr = fopen("frase.dat", "wb+");
	if (Ptr == NULL)
		printf ("Erro de Abertura");
	else
	{
		while(fraseCodificada[i] != '\0')
		{
			u.bi.b0 = fraseCodificada[i] - '0';
			u.bi.b1 = fraseCodificada[i+1] - '0';
			u.bi.b2 = fraseCodificada[i+2] - '0';
			u.bi.b3 = fraseCodificada[i+3] - '0';
			u.bi.b4 = fraseCodificada[i+4] - '0';
			u.bi.b5 = fraseCodificada[i+5] - '0';
			u.bi.b6 = fraseCodificada[i+6] - '0';
			u.bi.b7 = fraseCodificada[i+7] - '0';
			i = i+8;
			fwrite(&u.num, sizeof(char),1, Ptr);
		}
	}
	
	fclose(Ptr);
	
}
void exibirFloresta(Floresta *F)
{
	while (F != NULL)
	{
		printf ("\nFrequencia: %.2f\n", F->no->freq);
		printf ("Simbolo: %d\n", F->no->simbolo);
		F = F->prox;
	}
}
void exibirLista(Lista *no)
{
    if (no != NULL)
    {
        printf("\n%-15s%-12s%-10s%-s\n", "Palavra", "Frequencia", "Simbolo", "Codigo");
    

        while (no != NULL)
        {
            
            printf("%-15s%-12.2f%-10d%-s\n", 
                   no->tabela.palavra, 
                   no->tabela.freq, 
                   no->tabela.simbolo, 
                   no->tabela.codigo);

            no = no->prox;
        }
    }
    else
    {
        printf("Lista Vazia");
    }
}
void exibeh(Tree *raiz)
{
	static int n = -1;
	if (raiz != NULL)
	{
		n++;
		exibeh(raiz->dir);
		for (int i =0; i < 5*n; i++)
			printf (" ");
		printf ("(%d,%.2f)\n",raiz->simbolo,raiz->freq);
		exibeh(raiz->esq);
		n--; 
		
		
	}
	
}

void gravarArqBin(Lista *no)
{
	FILE * Ptr = fopen("registro.dat", "wb+");
	if (Ptr == NULL)
		printf ("Erro de Abertura");
	else
	{
		rewind(Ptr);
		Tabela Reg;
		while(no != NULL)
		{
			
			strcpy(Reg.codigo, no->tabela.codigo);
			Reg.freq = no->tabela.freq;
			Reg.simbolo = no->tabela.simbolo;
			strcpy(Reg.palavra, no->tabela.palavra);
			fwrite(&Reg, sizeof(Tabela),1, Ptr);
			
			no = no->prox;
		}
		
		
	}
	fclose(Ptr);
	
}
void lerArqBin(void)
{
	FILE * Ptr = fopen("registro.dat", "rb");
	if (Ptr == NULL)
		printf ("erro de abertura");
	else
	{
		Tabela Reg;
		rewind(Ptr);
		fread(&Reg, sizeof(Tabela),1, Ptr);
		while(!feof(Ptr))
		{
			printf ("\nPalavra: %s\n", Reg.palavra);
			printf ("Frequencia: %.2f\n" ,Reg.freq);
			printf ("Codigo: %s\n", Reg.codigo);
			printf ("Simbolo: %d\n", Reg.simbolo);
			
			fread(&Reg, sizeof(Tabela),1, Ptr);
		}
		
		
	}
	fclose(Ptr);
}
int main ()
{
	Lista *no = NULL;
	Floresta *F = NULL;
	char frase[500], fraseNova[500], fraseCodificada[200] ="";
	int contPalavras = 0;
	strcpy(frase, "amar e sonhar sonhar e viver viver e curtir curtir e amar cada um tera uma escolha cada um fara a escolha cada um escolhe a sua escolha levaremos um tempo para crescer levaremos um tempo para amadurecer levaremos um tempo para entender levaremos um tempo para envelhecer levaremos um tempo para morrer");
	strcpy (fraseNova, "cada um escolhe a sua escolha e cada um fara a escolha viver e amar levaremos um tempo para crescer amadurecer entender e sonhar curtir e viver e sonhar e amar cada um tera uma escolha levaremos um tempo para envelhecer e morrer");

	
	lerFrase(&no, frase, contPalavras);
	calcFreq(&no, contPalavras);
	
	InserirFloresta(&F, no);
	construirArvore(&F);
	
	//codificação do simbolo e frase
	gerarCodigoHuffman(F->no, no);
	codificarFrase(no, fraseNova, fraseCodificada);
	
	//exibir Arvore e Lista de Registros
	printf ("-----------------------------------------------------------\n");
	printf ("\t\t Arvore No Formato Horizontal\n");
	printf ("-----------------------------------------------------------\n");
	exibeh(F->no);
	printf ("\n -----------------------------------------------------------\n");
	printf ("\t\t\t Registros\n");
	printf ("-----------------------------------------------------------\n");
	exibirLista(no);
	printf ("\n-----------------------------------------------------------\n");

	// arquivos
	//printf ("\nFrase Codificada: %s\n", fraseCodificada);
	armazenarFraseCodificada(fraseCodificada);
	gravarArqBin(no);
	//lerArqBin();
	return 0;
}
