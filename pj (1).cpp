#include <iostream>
#include <cstring>
#include <vector>
#define MAX_PROC 11 //numero maximo de processos

typedef void (*func_ptr)(void); //ponteiro de função

std::vector<char*> itens{
    //nome de cada processo, inspirado em séries
    //vetor dinamico de ponteiros de char
   "Dark_Mysteries", "Breaking_Badlands", "Stranger_Crimes","Game_Thrones", "The_Walking_Justice", 
   "Friends_Forever", "Black_Mirrorverse", "The_Office_Adventures", "House_Medical","Lost_Island",
    "Westworld_Wonders",   "The_Crown_Jewels",   "The_Big_Detective",   "Better_Call_Justice", "Sherlock_Holmes Files", 
    "The_Handmaid's_Criminal",  "Narcos_Druglords", "The_Simpsons_Mystery", "Fargo_Crime",
    "The_Vampire_Crimes",  "Peaky_Blinders_Secrets",  "True_Detective_Mind",  "The_Witcher_Sagas", "Grey's_Anatomy_Life", 
    "Brooklyn_Nine-Nine_Police","Money_Heist_Legends", "The_Expanse_Universe","The_Good_Place_Beyond",
    "Vikings_Sagas","The_Mandalorian_Adventures",
};

//definição da estrutura do processo
typedef struct {
    char* nomeDoProcesso;
    int periodo;
    int inicio;
    int tempo;
    func_ptr func;
} processo;

// buffer para processos utilizados no algoritmo srtn
processo srtBuffer[MAX_PROC];
int inicio = 0; //indice do inicio do buffer
int fim = 0; //indice de fim do buffer

//ponteiro de leitura da entrada
FILE* in;
//ponteiro de escrita da saída
FILE* out;

//adiciona um processo ao buffer do SRTN
int adicionarProcSRTN(char* nome, int periodo, int tempo, func_ptr algumaFunc)
{
    if (fim + 1 != inicio) // verifica se o buffer está cheio
    {
        srtBuffer[fim].nomeDoProcesso = nome;
        srtBuffer[fim].periodo = periodo;
        srtBuffer[fim].inicio = periodo;
        srtBuffer[fim].tempo = tempo;
        srtBuffer[fim].func = algumaFunc;

        fim = (fim + 1) % MAX_PROC;
        return 0;  
    }
    return -1;
}

//remove um processo do buffer do SRTN
int removerProcSRTN()
{
    if (inicio != fim) // verifica se o buffer está vazio
    {
        inicio = (inicio + 1) % MAX_PROC;
        return 0;
    }
    return -1;
}

//impreme os processos presentes no buffer do SRTN
void poolSRTN()
{
    int proxy = inicio;
    if (fim != inicio) // não está vazio
    {
        fprintf(out, "                                Processos                                          \n");
        while (proxy != fim)
        {
            fprintf(out, "========  Nome do Processo: %s - Período: %d - Tempo: %d \n ", srtBuffer[proxy].nomeDoProcesso, srtBuffer[proxy].periodo, srtBuffer[proxy].tempo);

            proxy = (proxy + 1) % MAX_PROC;
        }
        return;
    }
    fprintf(out, "buffer vazio.\n");
    return;
}

//identifica e pega o processo com menor tempo restante no buffer do SRTN
int ProcSRTNComPeriodoDeTempo()
{
    int proxy = inicio;
    int proximo = inicio;
    while (proxy != fim)
    {
        if (srtBuffer[proxy].tempo < srtBuffer[proximo].tempo)
        {
            proximo = proxy;
        }
        proxy = (proxy + 1) % MAX_PROC;
    }

    processo temp = srtBuffer[inicio];
    srtBuffer[inicio] = srtBuffer[proximo];
    srtBuffer[proximo] = temp;
    return 0;
}

//executa o escalonador do SRTN
int escalonadorSRTN()
{
    if (fim != inicio)
    {
        ProcSRTNComPeriodoDeTempo();
    }
    return 0;
}

//executa o proximo processo no buffer do srtn
int execSRTN()
{
    if (inicio != fim)
    {
        srtBuffer[inicio].func();
        srtBuffer[inicio].inicio--;

        if (srtBuffer[inicio].inicio == 0)
        {
            removerProcSRTN();
        }
        else if (srtBuffer[inicio].inicio < srtBuffer[inicio].tempo)
        {
            srtBuffer[inicio].tempo = srtBuffer[inicio].inicio;
            escalonadorSRTN();
        }

        return 0;
    }
    return -1;
}

//função para melhorar a visualização dos prints
void imprimeSRTN()
{
    fprintf(out, " %s! \n \n", srtBuffer[inicio].nomeDoProcesso);
    return;
}

/*Realiza a inicialização do kernel do SRTN, lendo os 
dados de entrada de um arquivo e adicionando os processos ao buffer.*/
int inicializacaoKernelSRTN()
{
    int data, duracao, srt, i = 0;
    in = fopen("input.txt", "r");

    while (fscanf(in, "%d %d %d", &data, &duracao, &srt) != EOF)
    {
        adicionarProcSRTN(itens[i++], duracao, srt, imprimeSRTN);
    }

    fclose(in);

    return 0;
}

/*enquanto houver processos no buffer essa função irá executar e 
inicializar o kernel*/
int loopKernelSRTN()
{
    while (inicio != fim)
    {
        poolSRTN();
        escalonadorSRTN();
        execSRTN();
        if (srtBuffer[inicio].inicio > 0)
        {
            adicionarProcSRTN(srtBuffer[inicio].nomeDoProcesso, srtBuffer[inicio].inicio, srtBuffer[inicio].tempo, srtBuffer[inicio].func);
        }
        removerProcSRTN();
    }
    return 0;
}

//  buffer de Múltiplas Filas
processo buffer[2][MAX_PROC]; // Buffer com 2 filas e n processos
int inicio_[2];                   
int fim_[2];                      
int fila_ativa = 0;          

bool alocado; //variavel para controle de alocação de processos nas filas
bool nome; // Para atualizar a chave no nome
bool chave;  // Usado para alternar entre filas ao alocar processos

//intuitivamente, essa função adiciona um processo ao buffer de multiplas filas
int adicionarProcMultiFilas(char* nome, int periodo, int tempo, func_ptr algumaFunc)
{
    if (fim_[chave] + 1 != inicio_[chave]) // Verifica se o buffer está cheio
    {
        buffer[alocado][fim_[alocado]].nomeDoProcesso = nome; // Aloca o nome
        buffer[alocado][fim_[alocado]].periodo = periodo;    // Aloca o período na variável a ser decrementada
        buffer[alocado][fim_[alocado]].tempo = tempo;        // Aloca o tempo de tempo
        buffer[alocado][fim_[alocado]].periodo = tempo;      // Inicializa o tempo restante com o tempo de tempo
        buffer[alocado][fim_[alocado]].func = algumaFunc;    // Escolhe a função associada à thread

        fim_[alocado] = (fim_[alocado] + 1) % MAX_PROC; // Atualiza o fim da fila x
        return 0;
    }
    return -1; // Caso de erro
}

//remove sempre o processo das multiplas filas
int removerProcMultiFilas()
{
    if (inicio_[chave] != fim_[chave]) // Verifica se o buffer está vazio
    {
        inicio_[chave] = (inicio_[chave] + 1) % MAX_PROC; // Atualiza o início da fila
        return 0;
    }
    return -1; // Caso de erro
}

//imprime os processos presentes no buffer de multiplas filas 
void poolMultiFilas()
{
    fprintf(out, "                           Processos                                        \n");
    for (int f = 0; f < 2; f++) // Percorre as 2 filas
    {
        int proxy = inicio_[f];
        if (fim_[f] != inicio_[f]) // Não está vazio
        {
            while (proxy != fim_[f]) // Até não ser o fim da fila x
            {
                fprintf(out, "======== Nome do Processo: %s - Período: %d - Tempo: %d \n", buffer[f][proxy].nomeDoProcesso, buffer[f][proxy].periodo, buffer[f][proxy].tempo);
                proxy = (proxy + 1) % MAX_PROC; // Atualiza o proxy
            }
        }
    }
}

/*Obtém o próximo processo a ser executado na fila de 
tempo real (SRT) do buffer de Múltiplas Filas.*/
int obterProcessoSRTMultiFilas()
{
    fprintf(out, "\n            Fila SRT              \n");
    int proxy = inicio_[chave];        // Proxy para marcar o início
    int proximo = inicio_[chave];      // Para marcar o próximo
    while (proxy != fim_[chave]) {     // Enquanto o proxy não alcançar o fim
        // Compara o tempo restante do primeiro com o tempo restante do próximo e escolhe o menor
        if (buffer[chave][proxy].periodo < buffer[chave][proximo].periodo) {
            proximo = proxy;
        }
        // Atualiza o próximo
        proxy = (proxy + 1) % MAX_PROC;
    }

    // Troca o processo com o menor tempo restante pelo primeiro processo
    processo temp = buffer[chave][inicio_[chave]];
    buffer[chave][inicio_[chave]] = buffer[chave][proximo];
    buffer[chave][proximo] = temp;

    return 0;
}

//
int escalonadorMultiFilas()
{
    //verifica se ambas as filas estao vazias
    if (fim_[chave] != inicio_[chave] || fim_[!chave] != inicio_[!chave]) {  // Se ambas as filas não estiverem vazias
        //imprime a chave, para saber se a fila esta ativa
        fprintf(out, "Fila-atual = %d\n", chave);
        //obtem o proximo processo na fila de tempo real
        obterProcessoSRTMultiFilas();  // Obtém o processo com o menor tempo restante
        return 0;
    }

    //finaliza o programa, caso ele dê erro
    fprintf(out, "Buffer vazio.\n");
    return -1; 
}


int execMultiFilas()
{
    bool fila_atual = chave;    // Armazena a minha fila atual
    nome = fila_atual;          // Para saber em qual fila chamar o imprime()

    // Verifica se ambas as filas estão vazias, se estiverem, encerra o programa
    if (inicio_[fila_atual] == fim_[fila_atual] && inicio_[!fila_atual] == fim_[!fila_atual]) {
        fprintf(out, "Ambos os buffers estão vazios.\n");
        exit(0);
    }
    else {
        if (buffer[fila_atual][inicio_[fila_atual]].tempo < 4 && fila_atual == 0) {
                alocado = 1;
                adicionarProcMultiFilas( buffer[fila_atual][inicio_[fila_atual]].nomeDoProcesso,  buffer[fila_atual][inicio_[fila_atual]].periodo,
                                    buffer[fila_atual][inicio_[fila_atual]].tempo,  buffer[fila_atual][inicio_[fila_atual]].func);
                removerProcMultiFilas();
        }
        if (inicio_[fila_atual] != fim_[fila_atual]) {                  // Se a fila não estiver vazia
            buffer[fila_atual][inicio_[fila_atual]].func();             // Executa a função
            buffer[fila_atual][inicio_[fila_atual]].periodo--;    // Decrementa o tempo restante

            // Se a fila A estiver vazia
        }
        else {
            fila_atual = !fila_atual;
            nome = !fila_atual;
            if (inicio_[fila_atual] != fim_[fila_atual]) {                // Se a fila não estiver vazia
                buffer[fila_atual][inicio_[fila_atual]].func();           // Executa a função
                buffer[fila_atual][inicio_[fila_atual]].periodo--;  // Decrementa o tempo restante
            }
        }
        //verifica se o tempo restante do processo atual acabou
        if (buffer[fila_atual][inicio_[fila_atual]].periodo == 0) {  // Se o tempo restante for zero
            //se acabou, ele finaliza o processo
            fprintf(out, "\nFinalizando processo %s!\n\n", buffer[fila_atual][inicio_[fila_atual]].nomeDoProcesso);
            removerProcMultiFilas();                                   // Remove o processo
        }
    }

    return 0;
}

//printa quando o processo esta em execução, como se fosse um check point
void MsnMultiFilas()
{
    fprintf(out, " %s! \n\n", buffer[nome][inicio_[nome]].nomeDoProcesso);
    return;
}

//incializa o kernel das multiplas filas
int inicializacaoMultiFilas()
{
    alocado = 0;
    int data, duracao, tempo, i = 0;
    in = fopen("input.txt", "r");

    while (fscanf(in, "%d %d %d", &data, &duracao, &tempo) != EOF) {
        adicionarProcMultiFilas(itens[i++], duracao, tempo, MsnMultiFilas);
    }

    fclose(in);

    return 0;
}


//loop principal do kernel de multiplas filas
int loopMultiFilas()
{
    // Se uma fila estiver vazia
    while (inicio_[chave] != fim_[chave] || inicio_[!chave] != fim_[!chave]) {
        poolMultiFilas();
        escalonadorMultiFilas();
        // Chama o processo selecionado
        execMultiFilas();

        // Se ambas as filas estiverem cheias, permitir a alternância entre elas
        if (inicio_[chave] != fim_[chave] || inicio_[!chave] != fim_[!chave]) {
            chave = !chave;
            // Verifica se o processo existe na fila atual, caso contrário, muda o índice
            if (inicio_[chave] == fim_[chave] && inicio_[!chave] != fim_[!chave]) {
                chave = !chave;
            }
        }
    }

    return 0;
}

int main()
{
    out = fopen("output.txt", "w");

    fprintf(out, " Escalanodar Sistema Ooeracional Embarcado \n\n");
    fprintf(out, "Escalonador SRTL Cooperativo  e    Escalonador Multi-Filas\n\n\n");

    fprintf(out, " Executando o SRTN (Shortest Remaining-Time Next) : \n\n");
    inicializacaoKernelSRTN();
    loopKernelSRTN();

    fprintf(out, "\n\n\n");
    fprintf(out, " Executando o Múltiplas filas com realimentação: \n\n");
    
    inicializacaoMultiFilas();
    loopMultiFilas();

    fclose(out);
    return 0;
}
