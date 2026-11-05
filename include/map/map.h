/** @file map.h
 * @brief Módulo de mapa.
 * @details Aqui é onde fica o gerenciamento dos diferentes tipos de mapa
 *          e suas respectivas formas de renderização.
 */
#ifndef MAP_H
#define MAP_H

#include <raylib.h>

/** @enum MapType
 *
 * @brief Define o tipo de mapa que está sendo utilizado.
 *
 * @details Cada tipo determina como o mapa será recortado e desenhado na tela.
 */
typedef enum MapType {
    MAP_SINGLE,    // imagem enorme, sem recorte
    MAP_CROP,  // recorte manual de uma região
    MAP_TILEMAP,  // grade de tiles (futuro)
    MAPA_COUNT
} MapType;

/** @struct MapCrop
 *
 * @brief Armazena as coordenadas de um recorte manual de mapa.
 *
 * @param source  Região recortada dentro da textura de origem.
 * @param dest    Onde e com qual tamanho o recorte será desenhado na tela.
 */
typedef struct MapCrop {
    Rectangle source;   // coordenadas na imagem fonte
    Rectangle dest;  // onde e como desenhar na tela
} MapCrop;

/** @struct MapBody
 *
 * @brief Estrutura que armazena o estado completo de um mapa.
 *
 * @details Essa estrutura age como se fosse uma classe, guardando
 *          todos os atributos necessários para gerenciar o mapa.
 *          Dependendo do tipo, campos diferentes são utilizados.
 *
 * @param type          Tipo de mapa que determina como ele será desenhado.
 * @param texture       Textura principal do mapa.
 * @param crop          Dados de recorte, usado quando type == MAP_CROP.
 * @param camera_offset Deslocamento da câmera sobre o mapa, usado quando type == MAP_SINGLE.
 */
typedef struct MapBody {
    MapType  type;
    Texture2D texture;

    // usado em MAPA_RECORTE
    MapCrop crop;

    // usado em MAPA_UNICO — posição da câmera sobre o mapa
    Vector2 camera_offset;
} MapBody;

/** @brief Carrega a textura e inicializa o estado do mapa.
 *
 * @details Carrega a textura a partir do caminho fornecido e define
 *          os valores padrão para os demais campos da estrutura.
 *          Deve ser chamada uma única vez antes do game loop.
 *
 * @param self             Ponteiro para uma entidade mapa.
 * @param type             Tipo de mapa (MAP_SINGLE, MAP_CROP ou MAP_TILEMAP).
 * @param texture_path     Caminho para o arquivo de imagem do mapa.
 */
void MapInit(MapBody *self, MapType type, const char *texture_path);

/** @brief Define as coordenadas de recorte para MAP_CROP.
 *
 * @details Configura de qual região da textura será feito o recorte
 *          e em qual posição/tamanho ele será desenhado na tela.
 *          Só tem efeito quando o tipo do mapa é MAP_CROP.
 *
 * @param self    Ponteiro para uma entidade mapa.
 * @param source  Região de origem na textura (em pixels).
 * @param dest    Região de destino na tela (posição + escala).
 */
void MapSetCrop(MapBody *self, Rectangle source, Rectangle dest);

/** @brief Define o deslocamento da câmera para MAP_SINGLE.
 *
 * @details Move o ponto de vista sobre o mapa inteiro. Só tem efeito
 *          quando o tipo do mapa é MAP_SINGLE.
 *
 * @param self    Ponteiro para uma entidade mapa.
 * @param offset  Deslocamento (x, y) da câmera sobre a textura.
 */
void MapSetCamera(MapBody *self, Vector2 offset);

/** @brief Desenha o mapa na tela.
 *
 * @details Verifica o tipo do mapa e aplica a lógica de renderização
 *          correspondente: imagem completa, recorte manual ou tilemap.
 *
 * @param self Ponteiro para uma entidade mapa.
 */
void MapDraw(MapBody *self);

/** @brief Descarrega a textura do mapa.
 *
 * @details Libera a textura do buffer de memória da GPU.
 *          Deve ser chamada ao encerrar o jogo ou trocar de cena.
 *
 * @param self Ponteiro para uma entidade mapa.
 */
void MapUnload(MapBody *self);

#endif 