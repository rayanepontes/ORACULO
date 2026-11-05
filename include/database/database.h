#ifndef DATABASE_H
#define DATABASE_H

#include <stdbool.h>

/** @brief Número máximo de entradas no ranking. */
#define DATABASE_MAX_ENTRIES 10

/** @brief Caminho do arquivo de saves. */
#define DATABASE_FILE_PATH "database/ranking_db.txt"

/**
 * @brief Dados de uma entrada do ranking.
 */
typedef struct GameData {
    char nickname[7]; // Nickname do jogador (máx 5 chars + '\0').
    int  score;       // Pontuação obtida na partida.
} GameData;

/**
 * @brief Carrega o ranking do arquivo txt para um array.
 * @param data_saved Array de GameData a ser preenchido.
 * @param count   Quantidade de entradas lidas.
 */
void DataLoad(GameData data_saved[], int *count);

/**
 * @brief Salva o array de entradas no arquivo txt.
 * @param entries Array de GameData a ser salvo.
 * @param count   Quantidade de entradas no array.
 */
void DataSave(GameData entries[], int count);

/**
 * @brief Insere uma nova entrada no ranking, ordena e limita a DATABASE_MAX_ENTRIES.
 * @param data_saved Array de GameData existente.
 * @param count   Ponteiro para a quantidade atual de dados (pode ser incrementado).
 * @param new_entry Nova entrada a inserir.
 */
bool DataInsert(GameData data_saved[], int *count, GameData new_entry);

#endif