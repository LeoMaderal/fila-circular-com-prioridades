#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio2.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "filaP.h"

void moldura(int CI, int LI, int CF, int LF, int CorT, int CorF)
{
	int i;
	textcolor(CorT);
	textbackground(CorF);
	gotoxy(CI,LI);
	printf("%c",201);
	
	gotoxy(CF,LI);
	printf("%c",187);
	
	gotoxy(CI,LF);
	printf("%c",200);
	
	gotoxy(CF,LF);
	printf("%c",188);
	
	for(i=CI+1;i<CF;i++)
	{
		gotoxy(i,LI);
		printf("%c",205);
		gotoxy(i,LF);
		printf("%c",205);
	}
	
	for(i=LI+1;i<LF;i++)
	{
		gotoxy(CI,i);
		printf("%c",186);
		gotoxy(CF,i);
		printf("%c",186);
	}	
}

char prioridade(char funcao[60], int cont[5])
{
	char aux;
	if(stricmp("Gravar Dispositivo Interno", funcao) == 0)
	{
		aux = 'A';
		cont[0]++;
	}
	else if(stricmp("Gravar Dispositivo Externo", funcao) == 0)
	{
		aux = 'B';
		cont[1]++;
	}
	else if(stricmp("Ler", funcao) == 0)
	{
		aux = 'C';
		cont[2]++;
	}
	else if(stricmp("Deletar", funcao) == 0)
	{
		aux = 'D';
		cont[3]++;
	}
	else if(stricmp("Imprimir", funcao) == 0)
	{
		aux = 'E';
		cont[4]++;
	}
	return aux;
}

void titulo(void)
{
	moldura(48,2,73,4,7,0);
	gotoxy(50,3);
	printf("FILA DE PROCESSADORES");
}

void colocaCaixas(void)
{
	moldura(10,7,28,11,7,0);
	gotoxy(13,6);
	printf("Processador 1");
	
	moldura(38,7,56,11,7,0);
	gotoxy(41,6);
	printf("Processador 2");
	
	moldura(66,7,84,11,7,0);
	gotoxy(69,6);
	printf("Processador 3");
	
	moldura(94,7,112,11,7,0);
	gotoxy(97,6);
	printf("Processador 4");
}

void fazCaixaTarefa(int i, int k)
{
	int x, y;
	x = 11 + i * 28;
	y = 15 + (k - 1) * 5;
	moldura(x,y,x+16,y+3,7,0);
}

void desfazCaixaTarefa(int i, int k)
{
	int x, y;
	x = 11 + i * 28;
	y = 15 + (k - 1) * 5;
	gotoxy(x,y);
	for(int j=x;j<=x+17;j++)
		for(int l=y;l<=y+4;l++)
		{
			gotoxy(j,l);
			printf(" ");
		}
}

void limpaTela(void)
{
    int j, l;
    for (j=2;j<118;j++)
    {
        for (l=12;l<29;l++)
        {
            gotoxy(j, l);
            printf(" ");
        }
    }
}

void abreviaFuncao(char prio, char funcao[60])
{
	if(prio == 'A')
		strcpy(funcao,"GravarDispInt");
	else
		strcpy(funcao,"GravarDispExt");
}

void abreviacao(char arq[60])
{
	int j, cont=0;
	char aux[60];
	if(strlen(arq) >= 15)
	{
		j=0;
		while(j<strlen(arq) && arq[j] != '.')
		{
			if(cont < 10)
				aux[cont++] = arq[j];
			j++;
		}
		while(arq[j] != '\0')
			aux[cont++] = arq[j++];
		aux[cont] = '\0';
		strcpy(arq,aux);
	}
}

void insereFila(TpFilaP f[4], int ut, int contTarefa[5], FILE *PtrTxt)
{
	TpElemento elem;
	int proc, tempo;
	char arquivo[60], funcao[60]; 
	fscanf(PtrTxt,"%d,%[^,],%[^,],%d\n",&proc, &arquivo, &funcao, &tempo);
	if(!filaCheia(f[proc-1]))
	{
		elem.prioridade = prioridade(funcao, contTarefa);
		abreviacao(arquivo);
		if(elem.prioridade == 'A' || elem.prioridade == 'B')
			abreviaFuncao(elem.prioridade,funcao);
		strcpy(elem.arquivo, arquivo);
		elem.tempo = tempo;
		elem.utEntrada = ut;
		strcpy(elem.funcao,funcao);
		inserir(f[proc-1],elem);
	}
}

void decrementa(TpElemento atend[4], int i)
{
	int x;
	x = 11 + i * 28;
	if(atend[i].tempo > 0)
		atend[i].tempo--;
	if(atend[i].tempo == 0)
	{
		for(int j=x;j<x+16;j++)
			for(int l=9;l<11;l++)
			{
				gotoxy(j,l);
				printf(" ");
			}
	}
}		

void recebeFila(int i, TpElemento atend[4])
{
	int x;
	x = 11 + i * 28;
	
	gotoxy(x,9);
	printf("                ");
	gotoxy(x,9);
	printf("%s",atend[i].arquivo);
	gotoxy(x,10);
	printf("                ");
	gotoxy(x,10);
	printf("%s|%d",atend[i].funcao, atend[i].tempo);
}

int contaFila(TpFilaP f)
{
	int cont=0;
	while(!filaVazia(f))
	{
		retirar(f);
		cont++;
	}
	if(cont > 3)
		return -1;
	return cont;
}

void retiraFila(TpFilaP f[4], TpElemento atend[4], int ut, int contProc[4], int mediaTemp[4])
{
	int i, aux;
	
	for(i=0;i<4;i++)
	{
		if(!filaVazia(f[i]) && atend[i].tempo == 0)
		{
			contProc[i]++;
			mediaTemp[i] += (ut - atend[i].utEntrada);
			atend[i] = retirar(f[i]);
			aux = contaFila(f[i])+1;
			if(aux > -1)
				desfazCaixaTarefa(i,aux);
		}
		else
			decrementa(atend, i);
		if(atend[i].tempo > 0)
			recebeFila(i, atend);
	}
}

void mediaTempo(int cont[4], int media[4])
{
	moldura(10, 13, 60, 20, 7, 0);
	gotoxy(18, 12);
	printf("[ DESEMPENHO DOS PROCESSADORES ]");
	gotoxy(13, 14);
	printf("Proc.");
	gotoxy(25, 14);
	printf("Atendimentos");
	gotoxy(42, 14);
	printf("Tempo Medio");
	gotoxy(12, 15);
	printf("----------------------------------------------");
	for(int i=0;i<4;i++)
	{
		gotoxy(13, 16 + i);
		printf("P%d", i + 1);
		gotoxy(25, 16 + i);
		printf("%d", cont[i]);
		gotoxy(42, 16 + i);
		if(cont[i] > 0)
			printf("%.2f UT", (float)media[i] / cont[i]);
		else
			printf("0.00 UT");
	}
	gotoxy(10, 21);
	printf("Pressione qualquer tecla para sair...");
	getch();
}

void tarefas(int cont[5], char nome[5][60])
{
	moldura(10, 3, 60, 11, 7, 0);
	gotoxy(20, 2);
	printf("[ TAREFAS PROCESSADAS ]");
	gotoxy(13, 4);
	printf("Tipo de Tarefa");
	gotoxy(48, 4);
	printf("Qtd");
	gotoxy(12, 5);
	printf("----------------------------------------------");
	for(int i=0;i<5;i++)
	{
		
		gotoxy(13, 6 + i);
		printf("%s", nome[i]);
		gotoxy(48, 6 + i);
		printf("%d", cont[i]);
	}
}

int main(void)
{
	textcolor(7);
	TpFilaP f[4];
	TpElemento atend[4];
	int ut=0, contTarefa[5], contProc[4], mediaTemp[4], i, k;
	char aux[60], nomeTarefa[5][60];
	strcpy(nomeTarefa[0], "Gravar em dispositivo interno");
	strcpy(nomeTarefa[1], "Gravar em dispositivo externo");
	strcpy(nomeTarefa[2], "Ler");
	strcpy(nomeTarefa[3], "Deletar");
	strcpy(nomeTarefa[4], "Imprimir");
	FILE *PtrTxt = fopen("Trabalho.txt", "r");
	fgets(aux,60,PtrTxt);
	titulo();
	colocaCaixas();
	for(i=0;i<5;i++)
		contTarefa[i] = 0;
	for(i=0;i<4;i++)
	{
		inicializar(f[i]);
		atend[i].tempo = 0; 
		contProc[i] = 0;
		mediaTemp[i] = 0;
	}
		
	do
	{
		moldura(1,1,120,30,7,0);
		if(ut %2 == 0)
			insereFila(f,ut,contTarefa,PtrTxt);
		retiraFila(f,atend,ut,contProc,mediaTemp);
		gotoxy(106,2);
		printf("Tempo: %d",ut);
		limpaTela();
		for(i=0;i<4;i++)
    	{
        	if(atend[i].tempo > 0)
         	   recebeFila(i,atend);
    	}
		for(i=0,k=1;i<4;i++,k++)
		{
			for(int l=1; l <= contaFila(f[i]) && l < 4; l++)
				fazCaixaTarefa(i, l);
			exibir(f[i], i, k);
		}
		Sleep(2000);
		ut++;
	}while(!kbhit() && !feof(PtrTxt));
	fclose(PtrTxt);
	clrscr();
	tarefas(contTarefa,nomeTarefa);
	mediaTempo(contProc,mediaTemp);
	getch();
	return 0;
}
