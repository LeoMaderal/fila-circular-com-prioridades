#include <conio2.h>
#define MAXFILA 1000

struct TpElemento
{
	char prioridade, arquivo[60], funcao[60];
	int utEntrada, tempo;	//utEntrada é a hora que entra na fila e tempo é a quantidade de uts exportado do txt
};

struct TpFilaP
{
	int inicio, fim, qtde;
	TpElemento fila[MAXFILA];
};

void inicializar(TpFilaP &f)
{
	f.inicio = 0;
	f.fim = -1;
	f.qtde = 0;
}

void inserir(TpFilaP &f, TpElemento elem)
{
	int i, cont=0,ant;
	TpElemento aux;
	if(f.fim == MAXFILA-1)
		f.fim = -1;	
	f.fila[++f.fim] = elem;
	f.qtde++;
	i = f.fim;
	if(i > 0)
		ant = i-1;
	else
		ant = MAXFILA-1;
	while(cont < f.qtde-1 && f.fila[i].prioridade < f.fila[ant].prioridade)
	{
		aux = f.fila[i];
		f.fila[i] = f.fila[ant];
		f.fila[ant] = aux;
		i = ant;
		cont++;
		if(i > 0)
			ant = i-1;
		else
			ant = MAXFILA-1;
	}
}

TpElemento retirar(TpFilaP &f)
{
	TpElemento aux;
	aux = f.fila[f.inicio++];
	if(f.inicio == MAXFILA)
		f.inicio = 0;
	f.qtde--;
	return aux;
}

char filaVazia(TpFilaP f)
{
	return f.qtde == 0;
}

char filaCheia(TpFilaP f)
{
	return f.qtde == MAXFILA;
}

TpElemento elementoInicio(TpFilaP f)
{
	return retirar(f);
}

TpElemento elementoFim(TpFilaP f)
{
	return f.fila[f.fim];
}

void exibir(TpFilaP f, int i, int k)
{
	TpElemento aux;
	int pos=0, x, y, l;
	x = 12 + i * 28;
	while(!filaVazia(f))
	{
		y = 16 + pos * 5;
		aux = retirar(f);
		gotoxy(x, y);
		printf("               ");
		gotoxy(x, y);
		printf("%s",aux.arquivo);
		gotoxy(x, y+1);
		printf("               ");
		gotoxy(x, y+1);
		printf("%s|%d",aux.funcao, aux.tempo);
		pos++;
	}
}
