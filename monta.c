// Luan Nunes Utimura

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 15

//                  0      1      2      3      4      5     6      7     8      9     10     11     12     13
char *mnemo[] = { "MOV", "ADD", "SUB", "CMP", "JMP", "JC", "JNC", "JZ", "JNZ", "JBE", "JA", "CALL", "HLT", "RET" };

int Busca(char col0[][20], char col1[][20], char OP[])
{
	int i;
	for(i = 0; i < MAX; i++)
	{
		if(strcmp(col0[i], OP) == 0)
		{
			strcpy(OP, col1[i]);
			return 1;
		}
	}
	return 0;
}

void upper(char baseString[])
{
	int i;
	for(i = 0; baseString[i]; i++)
		baseString[i] = toupper(baseString[i]);
}

void removeColchetes(char OP[], char AUX[])
{
	int i = 0;
	memset(AUX, 0, 10);
	while(OP[i+1] != ']')
	{
		AUX[i] = OP[i+1];
		i++;
	}

	strcpy(OP, AUX);
}

void separaInput(char baseString[], char RRR[], char MNE[], char OP1[], char OP2[])
{

	int i, param = 1, virgula = 0, indice = 0;

	// Limpa os dados de entrada
	memset(RRR, 0, 10);
	memset(MNE, 0, 10);
	memset(OP1, 0, 10);
	memset(OP2, 0, 10);

	// Procura por um possível comentário no input, se tiver, ele remove da string
	for(i = 0; baseString[i] != ';' && baseString[i] != '\0'; i++);
		baseString[i] = '\0';

	// Verifica se o input tem vírgula, portanto, OP1 e OP2
	for(i = 0; baseString[i] && !virgula; i++)
		if(baseString[i] == ',')
			virgula = 1;

	// A partir daqui, a variável "indice" será utilizada como ponteiro de análise do input
	// verificando parâmetros de entrada (RRR, MNE, OP1, OP2)

	// get Rótulo
	if(baseString[indice] != ' ')
	{
		i = 0;
		for(indice = 0; baseString[indice] != ' '; indice++)
		{
			RRR[i] = baseString[indice];
			i++;
		}
	}

	// Ajusta ponteiro para próximo parâmetro
	indice += 1;

	// get Mnemônico
	i = 0;
	for( ; baseString[indice] != ' ' && baseString[indice] != '\0'; indice++)
	{
		MNE[i] = baseString[indice];
		i++;
	}

	strtok(MNE, "\n");

	// Checa se o mnemônico é HLT ou RET, se true, ignora OP1 e OP2
	if((strcmp(MNE, "HLT") == 0) || (strcmp(MNE, "RET") == 0))
		param = 0;

	if(param)
	{
		// Ajusta ponteiro para próximo parâmetro
		indice += 1;

		if(virgula)
		{
			// get OP1
			i = 0;
			for( ; baseString[indice] != ','; indice++)
			{
				OP1[i] = baseString[indice];
				i++;
			}
			indice += 1;

			//get OP2
			i = 0;
			for( ; baseString[indice]; indice++)
			{
				OP2[i] = baseString[indice];
				i++;
			}
		}
		else
		{
			// get OP1
			i = 0;
			for( ; baseString[indice]; indice++)
			{
				OP1[i] = baseString[indice];
				i++;
			}
		}
		strtok(OP1, "\n");
	}
	/*
	printf("\nRot. %s:", RRR);
	printf("\nMne.: %s", MNE);
	printf("\nOP1.: %s", OP1);
	printf("\nOP2.: %s", OP2);
	getch();
	*/
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("use: monta <arquivo_de_entrada>\n");
		exit(0);
	}

	FILE *arq;
	FILE *out;

	arq = fopen(argv[1], "rb");

	argv[1][(strlen(argv[1]))-4] = '\0';
	strcat(argv[1],".opc");

	out = fopen(argv[1], "wt");

	char RRR[10];
	char MNE[10];
	char OP1[10];
	char OP2[10];
	char AUX[10];

	char baseString[256];
	char outString[20];
	char outOP[10];

	char col0[MAX][20];
	char col1[MAX][20];

	int i, exist, find, max = 0, count = 0;

	// Passo 1: Criar tabela de endereços
	while(fgets(baseString, 80, arq) != NULL)
	{
		upper(baseString);
		separaInput(baseString, RRR, MNE, OP1, OP2);
		// Cria tabelas de endereços
		if(strcmp(RRR, "") != 0)
		{
			if(strcmp(MNE, "EQU") == 0)
			{
				strcpy(col0[max], RRR);
				strcpy(col1[max], OP1);
				max++;
			}
			else
			{
				itoa(count, AUX, 10);
				strcpy(col0[max], RRR);
				strcpy(col1[max], AUX);
				max++;
				count += 2;
			}
		}
		else count += 2;
	}

	// Exibe tabela de endereços
	system("cls");
	printf("Passo 1\n");
	for(i = 0; i < max; i++)
	{
		printf("\n%s - %s", col0[i], col1[i]);
	}

	// Retorna o ponteiro
	fseek(arq, 0, SEEK_SET);

	// Passo 2: Traduz para Código de Máquina
	printf("\n\nPasso 2");
	while(fgets(baseString, 80, arq) != NULL)
	{
		upper(baseString);

		// Limpa variáveis
		memset(outString, 0, 20);
		memset(outOP, 0, 10);

		find = 0;
		exist = 0;

		separaInput(baseString, RRR, MNE, OP1, OP2);

		if(strcmp(MNE, "EQU") != 0)
		{
			for(i = 0; i < 14 && !find; i++)
			{
				if(strcmp(MNE, mnemo[i]) == 0)
					find = 1;
			}
			i--;

			// Correspondência de Mnemônicos
			if(i == 13)
				strcpy(outString, "f4h");
			else if(i == 12)
				strcpy(outString, "c3h");
			else if(i == 11) // ----------------------
				strcpy(outString, "e8h");
			else if(i == 10)
				strcpy(outString, "77h");
			else if(i == 9)
				strcpy(outString, "76h");
			else if(i == 8)
				strcpy(outString, "75h");
			else if(i == 7)
				strcpy(outString, "74h");
			else if(i == 6)
				strcpy(outString, "73h");
			else if(i == 5)
				strcpy(outString, "72h");
			else if(i == 4) // -----------------------
				strcpy(outString, "ebh");
			else if(i == 3) // CMP
			{
				if(OP2[0] != '[') // é dado
				{
					strcpy(outString, "3ch");
					strtok(OP2, "\n\r");
					if(OP2[0] >= 48 && OP2[0] <= 57) // Ex.: AL,50
						strcpy(outOP, OP2);
					else // Ex.: AL,NUM
					{
						exist = Busca(col0, col1, OP2);
						if(exist)
							strcpy(outOP, OP2);
						else
							strcpy(outOP, "NULL");
					}
				}
				else // é endereço
				{
					strcpy(outString, "3ah");
					removeColchetes(OP2, AUX);
					strtok(OP2, "\n\r");
					if(OP2[0] >= 48 && OP2[0] <= 57) // Ex.: [20]
						strcpy(outOP, OP2);
					else // Ex.: [Teste]
					{
						exist = Busca(col0, col1, OP2);
						if(exist)
							strcpy(outOP, OP2);
						else
							strcpy(outOP, "NULL");
					}
				}
			}
			else if(i == 2) // SUB
			{
				if(OP2[0] != '[') // é dado
				{
					strcpy(outString, "2ch");
					strtok(OP2, "\n\r");
					if(OP2[0] >= 48 && OP2[0] <= 57) // Ex.: AL,50
						strcpy(outOP, OP2);
					else // Ex.: AL,NUM
					{
						exist = Busca(col0, col1, OP2);
						if(exist)
							strcpy(outOP, OP2);
						else
							strcpy(outOP, "NULL");
					}
				}
				else // é endereço
				{
					strcpy(outString, "2ah");
					removeColchetes(OP2, AUX);
					strtok(OP2, "\n\r");
					if(OP2[0] >= 48 && OP2[0] <= 57) // Ex.: [20]
						strcpy(outOP, OP2);
					else // Ex.: [Teste]
					{
						exist = Busca(col0, col1, OP2);
						if(exist)
							strcpy(outOP, OP2);
						else
							strcpy(outOP, "NULL");
					}
				}
			}
			else if(i == 1) // ADD
			{
				if(OP2[0] != '[') // é dado
				{
					strcpy(outString, "04h");
					strtok(OP2, "\n\r");
					if(OP2[0] >= 48 && OP2[0] <= 57) // Ex.: AL,50
						strcpy(outOP, OP2);
					else // Ex.: AL,NUM
					{
						exist = Busca(col0, col1, OP2);
						if(exist)
							strcpy(outOP, OP2);
						else
							strcpy(outOP, "NULL");
					}
				}
				else // é endereço
				{
					strcpy(outString, "02h");
					removeColchetes(OP2, AUX);
					strtok(OP2, "\n\r");
					if(OP2[0] >= 48 && OP2[0] <= 57) // Ex.: [20]
						strcpy(outOP, OP2);
					else // Ex.: [Teste]
					{
						exist = Busca(col0, col1, OP2);
						if(exist)
							strcpy(outOP, OP2);
						else
							strcpy(outOP, "NULL");
					}
				}
			}
			else if(i == 0) // MOV
			{
				if(OP1[0] == '[') // [endereço],AL
				{
					strcpy(outString, "a2h");
					removeColchetes(OP1, AUX);
					strtok(OP1, "\n\r");
					if(OP1[0] >= 48 && OP1[0] <= 57) // Ex.: [20]
						strcpy(outOP, OP1);
					else // Ex.: [Teste]
					{
						exist = Busca(col0, col1, OP1);
						if(exist)
							strcpy(outOP, OP1);
						else
							strcpy(outOP, "NULL");
					}
				}
				else
				{
					if(OP2[0] != '[') // é dado
					{
						strtok(OP2, "\n\r");
						strcpy(outString, "b0h");
						if(OP2[0] >= 48 && OP2[0] <= 57) // Ex.: AL,50
							strcpy(outOP, OP2);
						else // Ex.: AL,NUM
						{
							exist = Busca(col0, col1, OP2);
							if(exist)
								strcpy(outOP, OP2);
							else
								strcpy(outOP, "NULL");
						}
					}
					else // é endereço
					{
						strcpy(outString, "a0h");
						removeColchetes(OP2, AUX);
						strtok(OP2, "\n\r");
						if(OP2[0] >= 48 && OP2[0] <= 57) // Ex.: [20]
							strcpy(outOP, OP2);
						else // Ex.: [Teste]
						{
							exist = Busca(col0, col1, OP2);
							if(exist)
								strcpy(outOP, OP2);
							else
								strcpy(outOP, "NULL");
						}
					}
				}
			}

			if(i >= 4 && i <= 11) // Todos os JUMPs + CALL
			{
				if(OP1[0] >= 48 && OP1[0] <= 57) // Ex.: [20]
					strcpy(outOP, OP1);
				else // Ex.: [Teste]
				{
					exist = Busca(col0, col1, OP1);
					if(exist)
						strcpy(outOP, OP1);
					else
						strcpy(outOP, "NULL");
				}
			}

			// Valida a saída, se for diferente de "NULL", pode ser impresso no output
			if(i != 13 && i != 12)
			{
				if(strcmp(outOP, "NULL") != 0) // Válido
				{
					fprintf(out, "%s\n%s\n", outString, outOP);
				}
				else
				{
					printf("Ocorreu um erro durante a execucao. Os enderecos estao corretos?");
					getch();
				}
			}
			else
			{
				fprintf(out, "%s\n", outString);
			}
		}
	}
	fclose(arq);
	fclose(out);
}