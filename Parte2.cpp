#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <ctype.h>


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

struct tree{
	int simbolo;
	struct tree *dir, *esq;
};
typedef struct tree Tree;
struct tabela{
	char palavra[30], codigo[8];
	int simbolo;
	float freq;
};
typedef struct tabela Tabela;

Tree *criaNo(int simbolo)
{
	Tree *NC;
	NC = (Tree*)malloc(sizeof(Tree));
	NC->dir = NULL;
	NC->esq = NULL;
	NC->simbolo = simbolo;
	return NC;
}
void lerArqFraseCodificada(char *fraseCodificada)
{
	FILE * Ptr = fopen("frase.dat", "rb");
	if (Ptr == NULL)
		printf ("Erro de Abertura");
	else
	{
		Byte u;
		int i=0;
		rewind(Ptr);
		fread(&u.num, sizeof(char),1, Ptr);
		while(!feof(Ptr))
		{
			
			fraseCodificada[i] = u.bi.b0 + '0';
			fraseCodificada[i+1] = u.bi.b1 + '0';
			fraseCodificada[i+2] = u.bi.b2 + '0';
			fraseCodificada[i+3] = u.bi.b3 + '0';
			fraseCodificada[i+4] = u.bi.b4 + '0';
			fraseCodificada[i+5] = u.bi.b5 + '0';
			fraseCodificada[i+6] = u.bi.b6 + '0';
			fraseCodificada[i+7] = u.bi.b7 + '0';
			
			i = i+8;

			fread(&u.num, sizeof(char),1, Ptr);
		}
		fraseCodificada[i] ='\0';
			
	}
	fclose(Ptr);
}	
void construirArvoreBB(Tree **raiz)
{
	FILE * Ptr = fopen("registro.dat","rb");
	if (Ptr == NULL)
		printf ("Erro de Abertura");
	else
	{
		Tabela Reg;
		rewind(Ptr);
		fread(&Reg, sizeof(Tabela),1, Ptr);
		while(!feof(Ptr))
		{
			Tree *NC, *aux;
			char codigo[8];
			int i =0;
			codigo[0] = '\0';
			strcpy(codigo, Reg.codigo);
			if (*raiz == NULL) // inicializa
				*raiz = criaNo(-1);
				
			aux = *raiz;
			while(codigo[i] != '\0')
			{
				if (codigo[i] == '0')
				{
					if (aux->esq == NULL)
					{
						aux->esq = criaNo(-1);
						aux = aux->esq;
					}
					else
					{
						aux = aux->esq;
					}
				}
				else
				if(aux->dir == NULL)
				{
					aux->dir = criaNo(-1);
					aux = aux->dir;
				}
				else
				{
					aux = aux->dir;
				}
				
				i++;
			}
			aux->simbolo = Reg.simbolo;
				
				
			
			fread(&Reg, sizeof(Tabela),1, Ptr);
		}
		
	}	
	fclose(Ptr);
}
void decodificarFrase(char *fraseCodificada, char *frase, Tree *raiz)
{
	int i =0;
	while(fraseCodificada[i] != '\0')
	{
		Tree *aux;
		aux = raiz;
		while(aux != NULL && aux->simbolo == -1)
		{
			
			if (fraseCodificada[i] == '0')
				aux = aux->esq;
			else
				aux = aux->dir;
				
			i++;
		}
		if (aux != NULL) // buscar palavra no arquivo binario
		{
			//printf ("simbolo: %d", aux->simbolo);
			//getch();
			FILE * Ptr = fopen("registro.dat", "rb");
			rewind(Ptr);
			Tabela Reg;
			
			fread(&Reg, sizeof(Tabela),1, Ptr);
			while(!feof(Ptr) && Reg.simbolo != aux->simbolo)
			{
				//printf ("\nSimbolo Binario: %d e Simbolo Arvore: %d\n", Reg.simbolo, aux->simbolo);
				//getch();
				fread(&Reg, sizeof(Tabela),1, Ptr);
			}
			
			if (!feof(Ptr)) // achou
			{
				
				strcat(frase, Reg.palavra);
			}
			
		}
		
		
	}
		
}
void imprimeArvore(Tree *raiz) {
    if (raiz != NULL) {
        imprimeArvore(raiz->esq);
        if (raiz->simbolo != -1) { // Exibe apenas nós com símbolos válidos
            printf("Simbolo: %d\n", raiz->simbolo);
        }
        imprimeArvore(raiz->dir);
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
		printf ("(%d)\n",raiz->simbolo);
		exibeh(raiz->esq);
		n--; 
		
		
	}
	
}
int main(void)
{
	Tree *raiz;
	raiz = NULL;
	char fraseCodificada[1024]= "", frase[200] = "";
	// le a frase codificada
	lerArqFraseCodificada(fraseCodificada);
	//printf ("Frase Codificada: %s", fraseCodificada);
	construirArvoreBB(&raiz);
	//imprimeArvore(raiz);
	//exibeh(raiz);
	
	// decodifica a frase
	decodificarFrase(fraseCodificada, frase, raiz);
	printf ("\n---------------------------------------------------------------------------------\n");
	printf ("\t\tFrase Decodificada");
	printf ("\n---------------------------------------------------------------------------------\n");
	printf ("%s\n", frase);
	printf ("\n---------------------------------------------------------------------------------\n");
	
	
	
}
