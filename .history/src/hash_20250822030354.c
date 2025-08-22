#include "hash.h" // Inclui os protótipos e a definição de TFunc
#include <unistd.h>
#include <string.h>

// Calcula o endereço hash para um dado código de funcionário.
// Usa o "Método da Divisão", que mapeia a chave para um índice válido na tabela.
int funcao_hash_funcionario(int cod) {
    return cod % TAMANHO_TABELA_HASH;
}

// Cria e inicializa o arquivo da tabela hash.
// Esta função apaga o conteúdo do arquivo e preenche cada posição com -1 (vazio).
void criar_tabela_hash_funcionario(FILE *tabela_hash) {
    // Leva o cursor para o início do arquivo.
    rewind(tabela_hash);
    // Trunca o arquivo para 0 bytes, limpando qualquer conteúdo anterior.
    ftruncate(fileno(tabela_hash), 0);

    // O valor -1 indica uma "gaveta" ou compartimento vazio na tabela hash.
    int pos_vazia = -1;
    printf("Inicializando tabela hash com %d posicoes...\n", TAMANHO_TABELA_HASH);
    
    // Itera por todas as posições e escreve o valor -1 em cada uma.
    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        fwrite(&pos_vazia, sizeof(int), 1, tabela_hash);
    }
    printf("Tabela Hash 'hash_funcionarios.dat' criada e inicializada.\n");
}

// Insere um funcionário em sua respectiva lista de colisão na tabela hash.
// Esta é a função central que implementa o encadeamento exterior.
void inserir_na_lista_hash(FILE *tabela_hash, FILE *arq_funcionarios, int pos_novo_func, int cod_novo_func) {
    // Calcula a "gaveta" (endereço) onde o novo funcionário deve ser inserido.
    int hash_addr = funcao_hash_funcionario(cod_novo_func);
    
    // Move o cursor no arquivo da tabela hash para a gaveta calculada.
    fseek(tabela_hash, hash_addr * sizeof(int), SEEK_SET);
    
    // Lê o conteúdo da gaveta para verificar se já existe uma lista iniciada.
    int pos_cabeca_lista;
    fread(&pos_cabeca_lista, sizeof(int), 1, tabela_hash);

    // Se a gaveta aponta para -1, ela está vazia. Não há colisão.
    if (pos_cabeca_lista == -1) {
        // Move o cursor de volta para a posição da gaveta.
        fseek(tabela_hash, hash_addr * sizeof(int), SEEK_SET);
        // Escreve a posição do novo funcionário, tornando-o o primeiro da lista.
        fwrite(&pos_novo_func, sizeof(int), 1, tabela_hash);
    } else {
        // Se a gaveta não estava vazia, houve uma colisão.
        // Precisamos percorrer a lista encadeada até o fim para adicionar o novo registro.
        int pos_atual = pos_cabeca_lista;
        TFunc *func_percorrido;

        while (1) {
            // No arquivo de dados, vai para a posição do funcionário atual da lista.
            fseek(arq_funcionarios, pos_atual * tamanho_registro_funcionario(), SEEK_SET);
            // Lê os dados do funcionário.
            func_percorrido = le_funcionario(arq_funcionarios);
            
            // Verifica se este é o último funcionário da lista (prox == -1).
            if (func_percorrido->prox == -1) {
                // Encontramos o fim. O 'prox' deste último registro apontará para o novo.
                func_percorrido->prox = pos_novo_func;
                
                // Retorna para a posição do registro que acabamos de modificar.
                fseek(arq_funcionarios, pos_atual * tamanho_registro_funcionario(), SEEK_SET);
                // Salva (reescreve) o registro com o campo 'prox' atualizado.
                salva_funcionario(func_percorrido, arq_funcionarios);
                
                // Libera a memória e encerra o loop.
                free(func_percorrido);
                break;
            }
            
            // Se não for o último, avança para a próxima posição indicada pelo campo 'prox'.
            pos_atual = func_percorrido->prox;
            // Libera a memória do registro lido nesta iteração.
            free(func_percorrido);
        }
    }
}

// Lê um ficheiro de dados de funcionários e constrói a tabela hash do zero.
void construir_hash_da_base_existente(FILE *tabela_hash, FILE *arq_funcionarios) {
    // Imprime uma mensagem informativa para o utilizador.
    printf("\nConstruindo tabela hash a partir da base de dados...\n");
    // Garante que a tabela hash esteja limpa e inicializada com -1 antes de começar.
    criar_tabela_hash_funcionario(tabela_hash);
    
    // Calcula o número total de registos no ficheiro para controlar o loop.
    // Para isso, movemos o cursor para o final do ficheiro (SEEK_END)...
    printf("Calculando total de registros  no ficheiro para controlar o loop...\n");
    fseek(arq_funcionarios, 0, SEEK_END);
    // ...pegamos a posição total em bytes com ftell() e dividimos pelo tamanho de um registo.
    printf("pegamos a posição total em bytes com ftell() e dividimos pelo tamanho de um registo...\n");
    int total_registros = ftell(arq_funcionarios) / tamanho_registro_funcionario();

    // Ponteiro temporário para armazenar os dados de cada funcionário lido.
    printf("Ponteiro temporário para armazenar os dados de cada funcionário lido...\n");
    TFunc *func_lido;
    
    // Itera por cada uma das posições de registo do ficheiro, de 0 até ao final.
    printf("Iterando por todas as posicoes do ficheiro de dados...\n");
    for (int pos_atual = 0; pos_atual < total_registros; pos_atual++) {
        
        // 1. Posiciona o cursor no início exato do registo que queremos ler.
        fseek(arq_funcionarios, pos_atual * tamanho_registro_funcionario(), SEEK_SET);

        // 2. Lê os dados completos do registo daquela posição.
        func_lido = le_funcionario(arq_funcionarios);
        
        // Medida de segurança: se a leitura falhar, encerra o loop.
        if (func_lido == NULL) {
            break; 
        }

        // 3. Processa o registo apenas se ele estiver marcado como ativo ('ocupado').
        if (func_lido->ocupado == 1) {
            // Chama a função de inserção para conectar este funcionário à sua lista na hash.
            inserir_na_lista_hash(tabela_hash, arq_funcionarios, pos_atual, func_lido->cod);
        }
        
        // 4. Liberta a memória alocada para o registo lido nesta iteração.
        free(func_lido);
    }

    // Imprime uma mensagem de sucesso ao finalizar o processo.
    printf("Construção da tabela hash concluída.\n");
}

// Encontra um funcionário no arquivo de dados usando a tabela hash.
// Retorna um ponteiro para TFunc em memória se encontrar, ou NULL.
// ATENÇÃO: Quem chama esta função deve liberar a memória do ponteiro retornado!
TFunc* buscar_funcionario_hash(int cod, FILE *tabela_hash, FILE *arq_funcionarios) {
    // Calcula a "gaveta" onde o funcionário deveria estar.
    int hash_addr = funcao_hash_funcionario(cod);
    printf("Buscando codigo %d no endereco de hash %d...\n", cod, hash_addr);

    // Vai até a gaveta na tabela hash para descobrir a posição inicial da lista.
    fseek(tabela_hash, hash_addr * sizeof(int), SEEK_SET);
    int pos_atual;
    if (fread(&pos_atual, sizeof(int), 1, tabela_hash) == 0 || pos_atual == -1) {
        // Se a gaveta estiver vazia, o funcionário não existe.
        printf("Nenhum funcionario encontrado nesse endereco.\n");
        return NULL;
    }
    
    // Inicia a travessia na lista de colisão.
    TFunc* func_atual;
    while (pos_atual != -1) { // O loop continua até o fim da lista (prox == -1).
        // Pula para a posição do funcionário no arquivo de dados.
        fseek(arq_funcionarios, pos_atual * tamanho_registro_funcionario(), SEEK_SET);
        // Lê os dados do funcionário.
        func_atual = le_funcionario(arq_funcionarios);

        // Verifica se o funcionário lido é o que procuramos E se ele está ativo.
        if (func_atual != NULL && func_atual->cod == cod && func_atual->ocupado == 1) {
            printf("Funcionario encontrado na posicao fisica %d do arquivo de dados.\n", pos_atual);
            // Sucesso! Retorna o ponteiro para o funcionário encontrado.
            return func_atual; 
        }
        
        // Se não for o funcionário certo, avança para o próximo da lista.
        pos_atual = func_atual->prox;
        // Libera a memória do funcionário verificado nesta iteração.
        free(func_atual); 
    }

    // Se o loop terminar, a lista foi percorrida e o funcionário não foi encontrado.
    printf("Funcionario com codigo %d nao encontrado na lista de colisao.\n", cod);
    return NULL;
}

// Remove um funcionário do sistema (remoção lógica).
// Marca o funcionário como inativo e ajusta a lista encadeada para "pular" ele.
void remover_funcionario_hash(int cod, FILE *tabela_hash, FILE *arq_funcionarios) {
    // Acha a gaveta e a cabeça da lista, como na busca.
    int hash_addr = funcao_hash_funcionario(cod);
    fseek(tabela_hash, hash_addr * sizeof(int), SEEK_SET);
    int pos_atual;
    if (fread(&pos_atual, sizeof(int), 1, tabela_hash) == 0 || pos_atual == -1) {
        printf("ERRO: Funcionario nao encontrado.\n");
        return;
    }
    
    // Inicia a travessia, guardando também a posição do nó ANTERIOR.
    // O 'pos_anterior' é crucial para religar os ponteiros da lista.
    int pos_anterior = -1;
    TFunc *func_atual = NULL;

    while (pos_atual != -1) {
        fseek(arq_funcionarios, pos_atual * tamanho_registro_funcionario(), SEEK_SET);
        func_atual = le_funcionario(arq_funcionarios);

        // Procura pelo funcionário a ser removido.
        if (func_atual != NULL && func_atual->cod == cod && func_atual->ocupado == 1) {
            
            // ETAPA 1: REMOÇÃO LÓGICA
            // Marca o funcionário como inativo ('ocupado' = 0).
            func_atual->ocupado = 0;
            // Salva a alteração no arquivo de dados.
            fseek(arq_funcionarios, pos_atual * tamanho_registro_funcionario(), SEEK_SET);
            salva_funcionario(func_atual, arq_funcionarios);
            
            // ETAPA 2: RELIGAR OS PONTEIROS DA LISTA ENCADEADA
            if (pos_anterior == -1) {
                // Caso A: O funcionário a ser removido era o PRIMEIRO da lista.
                // A tabela hash precisa ser atualizada para apontar para o segundo (func_atual->prox).
                int proxima_pos = func_atual->prox;
                fseek(tabela_hash, hash_addr * sizeof(int), SEEK_SET);
                fwrite(&proxima_pos, sizeof(int), 1, tabela_hash);
            } else {
                // Caso B: O funcionário estava no MEIO ou no FIM da lista.
                // O funcionário ANTERIOR deve ser atualizado para pular o registro removido.
                fseek(arq_funcionarios, pos_anterior * tamanho_registro_funcionario(), SEEK_SET);
                TFunc *func_anterior = le_funcionario(arq_funcionarios);
                // O 'prox' do anterior agora aponta para o 'prox' do registro atual, "pulando-o".
                func_anterior->prox = func_atual->prox;
                // Salva a alteração no registro anterior.
                fseek(arq_funcionarios, pos_anterior * tamanho_registro_funcionario(), SEEK_SET);
                salva_funcionario(func_anterior, arq_funcionarios);
                free(func_anterior);
            }
            
            printf("Funcionario de codigo %d removido com sucesso.\n", cod);
            free(func_atual);
            return; // Encerra a função.
        }

        // Se não for o funcionário certo, avança na lista.
        pos_anterior = pos_atual;           // O atual se torna o anterior na próxima iteração.
        pos_atual = func_atual->prox;       // Avança para o próximo.
        free(func_atual);                   // Libera a memória.
    }

    // Se o loop terminar, o funcionário não foi encontrado na lista.
    printf("ERRO: Funcionario de codigo %d nao foi encontrado para remocao.\n", cod);
}

// Insere um novo funcionário a partir de dados digitados pelo usuário.
void inserir_novo_funcionario_hash(FILE *tabela_hash, FILE *arq_funcionarios) {
    // 1. Coleta os dados do novo funcionário do usuário
    int cod;
    char nome[50], cpf[15], data_nascimento[11];
    double salario;

    printf("\n--- Inserir Novo Funcionario ---\n");
    printf("Digite o codigo: ");
    scanf("%d", &cod);
    // Consome o '\n' que o scanf deixou no buffer
    getchar(); 
    printf("Digite o nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0'; // Remove o '\n' do final
    printf("Digite o CPF: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = '\0';
    printf("Digite a data de nascimento (DD/MM/AAAA): ");
    fgets(data_nascimento, sizeof(data_nascimento), stdin);
    data_nascimento[strcspn(data_nascimento, "\n")] = '\0';
    printf("Digite o salario: ");
    scanf("%lf", &salario);

    // 2. Cria a struct TFunc em memória com os dados
    TFunc *novo_func = funcionario(cod, nome, cpf, data_nascimento, salario);

    // 3. Salva o novo funcionário no FINAL do arquivo de dados
    fseek(arq_funcionarios, 0, SEEK_END);
    
    // Calcula a posição (índice) onde ele será salvo
    int pos_novo_func = ftell(arq_funcionarios) / tamanho_registro_funcionario();
    
    salva_funcionario(novo_func, arq_funcionarios);

    // 4. Conecta este novo funcionário à tabela hash
    inserir_na_lista_hash(tabela_hash, arq_funcionarios, pos_novo_func, novo_func->cod);

    printf("Funcionario de codigo %d inserido com sucesso.\n", novo_func->cod);
    free(novo_func);
}


// Imprime a estrutura completa da Tabela Hash para depuração.
void imprimir_tabela_hash_completa(FILE *tabela_hash, FILE *arq_funcionarios) {
    printf("\n--- ESTRUTURA DA TABELA HASH ---\n");
    rewind(tabela_hash);

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        int pos_cabeca_lista;
        fread(&pos_cabeca_lista, sizeof(int), 1, tabela_hash);
        printf("[%03d]: ", i);
        
        if (pos_cabeca_lista == -1) {
            printf("-> VAZIO\n");
        } else {
            int pos_atual = pos_cabeca_lista;
            while(pos_atual != -1) {
                fseek(arq_funcionarios, pos_atual * tamanho_registro_funcionario(), SEEK_SET);
                TFunc* f = le_funcionario(arq_funcionarios);
                if (f != NULL) {
                    // Imprime apenas se o registro não foi removido
                    if (f->ocupado == 1) {
                        printf("-> (Pos %d, Cod %d) ", pos_atual, f->cod);
                    }
                    pos_atual = f->prox;
                    free(f);
                } else { break; }
            }
            printf("\n");
        }
    }
    printf("---------------------------------------\n");
}