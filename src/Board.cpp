#include "Board.h"

Board::Board(glm::vec3 pos, glm::vec4 color)
    : Group(pos),
      base(glm::vec3(0.0f, -0.15f, 0.0f), glm::vec3(0.0f), glm::vec3(9.0f, 0.3f, 9.0f), 0.0f, glm::vec4(0.40f, 0.20f, 0.06f, 1.0f), false)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            grid[i][j] = nullptr;
        }
    }

    addChild(&base);

    float tileSize = 1.0f;
    float startX = -3.5f * tileSize;
    float startZ = -3.5f * tileSize;

    float tileY = 0.0f;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            float x = startX + j * tileSize;
            float z = startZ + i * tileSize;

            glm::vec4 tileColor = ((i + j) % 2 == 0) ? lightColor : darkColor;

            squares[i][j] = Cube(glm::vec3(x, tileY, z), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, tileColor, true);
            squares[i][j].position = glm::vec3(x, 0.01f, z);
            squares[i][j].scale = glm::vec3(tileSize, 0.02f, tileSize);

            addChild(&squares[i][j]);
        }
    }
}

Board::~Board()
{
    //dtor
}

void Board::setPieceAt(int row, int col, Object* piece) {
    if (row < 0 || row > 7 || col < 0 || col > 7) return;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (grid[i][j] == piece) {
                grid[i][j] = nullptr;
                break;
            }
        }
    }

    grid[row][col] = piece;

    if (piece != nullptr) {
        float tileSize = 1.0f;
        float startX = -3.5f * tileSize;
        float startZ = -3.5f * tileSize;

        float x = startX + col * tileSize;
        float z = startZ + row * tileSize;
        float pieceY = 0.05f;

        piece->position = glm::vec3(x, pieceY, z);
        addChild(piece);
    }
}

void Board::setupInitialPieces(std::vector<Object*> whitePieces, std::vector<Object*> blackPieces) {
    //16 peças pretas
    for (int j = 0; j < 8; j++) {
        setPieceAt(0, j, blackPieces[j]);
        setPieceAt(1, j, blackPieces[8 + j]);
    }

    //meio do tabuleiro
    for (int i = 2; i <= 5; i++) {
        for (int j = 0; j < 8; j++) {
            grid[i][j] = nullptr;
        }
    }

    //  as 16 peças brancas
    for (int j = 0; j < 8; j++) {
        setPieceAt(6, j, whitePieces[8 + j]);
        setPieceAt(7, j, whitePieces[j]);
    }
}

void Board::setHoveredSquare(int row, int col) {

    // Verifica se está dentro do tabuleiro
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        clearHover();
        return;
    }

    // Se continua na mesma casa, não precisa atualizar
    if (row == hoveredRow && col == hoveredCol)
        return;

    // Restaura a cor da casa anterior
    clearHover();

    // Aplica a cor azul na nova casa
    squares[row][col].setColor(hoverColor);

    // Armazena a posição atual
    hoveredRow = row;
    hoveredCol = col;
}


void Board::clearHover() {

    if (hoveredRow == -1 || hoveredCol == -1)
        return;

    // Restaura a cor original considerando o padrão do tabuleiro
    glm::vec4 originalColor =
        ((hoveredRow + hoveredCol) % 2 == 0)
        ? lightColor
        : darkColor;

    squares[hoveredRow][hoveredCol].setColor(originalColor);

    hoveredRow = -1;
    hoveredCol = -1;
}


