/**
 * @file database.c
 * @brief Implementação do sistema de ranking persistente em arquivo txt.
 */

#include "database/database.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// ─── Funções internas ─────────────────────────────────────────────────────────

/**
 * @brief Ordena o array de entradas em ordem crescente de tempo (menor = melhor).
 * @details Usa insertion sort — suficiente para no máximo DATABASE_MAX_ENTRIES entradas. 
 * @param data_saved Array de GameData a ordenar.
 * @param count      Quantidade de entradas no array.
 */
static void data_sort(GameData data_saved[], int count) {
    // insertion sort
    for (int i = 1; i < count; i++) {
        GameData chave = data_saved[i];
        int j = i - 1;

        // desloca os elementos menores que a chave uma posição à frente
        while (j >= 0 && data_saved[j].tempo > chave.tempo) {
            data_saved[j + 1] = data_saved[j];
            j--;
        }

        // insere a chave na posição correta
        data_saved[j + 1] = chave;
    }
}

// ─── Funções públicas ─────────────────────────────────────────────────────────

void DataLoad(GameData data_saved[], int *count) {
    *count = 0;

    FILE *file = fopen(DATABASE_FILE_PATH, "r");

    // Se o arquivo não existe ainda, não é erro — ranking está vazio
    if (file == NULL) return;

    while (*count < DATABASE_MAX_ENTRIES) {
        GameData entry;
        // lê uma linha no formato "NICK 1500"
        int lidos = fscanf(file, "%6s %d", entry.nickname, &entry.tempo);

        // fscanf retorna a quantidade de campos lidos — se não for 2, chegou no fim
        if (lidos != 2) break;

        data_saved[*count] = entry;
        (*count)++;
    }

    fclose(file);
}

void DataSave(GameData entries[], int count) {
    FILE *file = fopen(DATABASE_FILE_PATH, "w");

    if (file == NULL) return;

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d\n", entries[i].nickname, entries[i].tempo);
    }

    fclose(file);
}

bool DataInsert(GameData data_saved[], int *count, GameData new_entry) {
    // Se ainda há espaço, só adiciona ao final
    if (*count < DATABASE_MAX_ENTRIES) {
        data_saved[*count] = new_entry;
        (*count)++;
    } else {
        // Ranking cheio — substitui o último (maior tempo) se o novo for menor
        if (new_entry.tempo < data_saved[DATABASE_MAX_ENTRIES - 1].tempo) {
            data_saved[DATABASE_MAX_ENTRIES - 1] = new_entry;
        } else {
            return false; // tempo não entra no top 10
        }
    }

    // Reordena para manter ordem crescente
    data_sort(data_saved, *count);

    // Reordena para manter ordem decrescente
    data_sort(data_saved, *count);
    return true;
}