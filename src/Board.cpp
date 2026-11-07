#include "Board.h"
#include <Pawn.h>
#include <Rook.h>
#include <Queen.h>
#include <King.h>
#include <Knight.h>
#include <Bishop.h>

Board::Board(glm::vec3 pos, glm::vec4 color)
    : Group(pos),
      base(
          glm::vec3(0.0f, -0.15f, 0.0f),
          glm::vec3(0.0f),
          glm::vec3(9.0f, 0.3f, 9.0f),
          0.0f,
          glm::vec4(0.40f, 0.20f, 0.06f, 1.0f),
          false
      )
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            grid[i][j] = nullptr;
        }
    }

    addChild(&base);

    float tileSize = 1.0f;
    float startX = -3.5f;
    float startZ = -3.5f;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            float x = startX + j * tileSize;
            float z = startZ + i * tileSize;

            glm::vec4 tileColor =
                ((i + j) % 2 == 0) ? lightColor : darkColor;
//cria casas do tabuleiro 'ladrilhos'
            squares[i][j] = Cube(
                glm::vec3(x, 0.0f, z),
                glm::vec3(0.0f),
                glm::vec3(0.0f),
                0.0f,
                tileColor,
                true
            );

            squares[i][j].position = glm::vec3(x, 0.01f, z);
            squares[i][j].scale = glm::vec3(1.0f, 0.02f, 1.0f);

            addChild(&squares[i][j]);
        }
    }
    // PEÇAS BRANCAS
    Pawn* wPawns[8];
    for(int i = 0; i < 8; i++) {
        wPawns[i] = new Pawn(glm::vec3(0.0f), whiteColor);
    }

    Bishop* wbispo1 = new Bishop(glm::vec3(0.0f), whiteColor);
    Bishop* wbispo2 = new Bishop(glm::vec3(0.0f), whiteColor);

    Rook* wtorre1 = new Rook(glm::vec3(0.0f), whiteColor);
    Rook* wtorre2 = new Rook(glm::vec3(0.0f), whiteColor);

    Queen* wqueen = new Queen(glm::vec3(0.0f), whiteColor);

    King* wking = new King(glm::vec3(0.0f), whiteColor);

    Knight* wknight1 = new Knight(glm::vec3(0.0f), whiteColor);
    Knight* wknight2 = new Knight(glm::vec3(0.0f), whiteColor);

    wknight1->angle = 180;
    wknight2->angle = 180;

    whitePieces.push_back(wtorre1);
    whitePieces.push_back(wknight1);
    whitePieces.push_back(wbispo1);
    whitePieces.push_back(wqueen);
    whitePieces.push_back(wking);
    whitePieces.push_back(wbispo2);
    whitePieces.push_back(wknight2);
    whitePieces.push_back(wtorre2);
    for(int i = 0; i < 8; i++){
        whitePieces.push_back(wPawns[i]);
    }

    //PEÇAS PRETAS
    Pawn* bPawns[8];
    for(int i = 0; i < 8; i++) {
        bPawns[i] = new Pawn(glm::vec3(0.0f), blackColor);
    }

    Bishop* bbispo1 = new Bishop(glm::vec3(0.0f), blackColor);
    Bishop* bbispo2 = new Bishop(glm::vec3(0.0f), blackColor);

    Rook* btorre1 = new Rook(glm::vec3(0.0f), blackColor);
    Rook* btorre2 = new Rook(glm::vec3(0.0f), blackColor);

    Queen* bqueen = new Queen(glm::vec3(0.0f), blackColor);

    King* bking = new King(glm::vec3(0.0f), blackColor);

    Knight* bknight1 = new Knight(glm::vec3(0.0f), blackColor);
    Knight* bknight2 = new Knight(glm::vec3(0.0f), blackColor);

    blackPieces.push_back(btorre1);
    blackPieces.push_back(bknight1);
    blackPieces.push_back(bbispo1);
    blackPieces.push_back(bqueen);
    blackPieces.push_back(bking);
    blackPieces.push_back(bbispo2);
    blackPieces.push_back(bknight2);
    blackPieces.push_back(btorre2);
    for(int i = 0; i < 8; i++){
        blackPieces.push_back(bPawns[i]);
    }
}

Board::~Board()
{
}

void Board::setPieceAt(int row, int col, Object* piece)
{
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
        return;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (grid[i][j] == piece && piece != nullptr) {
                grid[i][j] = nullptr;
            }
        }
    }

    grid[row][col] = piece;

    if (piece != nullptr) {
        piece->position = getSquarePosition(row, col);
        piece->setMovementEnabled(true);

        addChild(piece);
    }
}

void Board::setupInitialPieces() {
    for (int j = 0; j < 8; j++) {
        setPieceAt(0, j, blackPieces[j]);
        setPieceAt(1, j, blackPieces[8 + j]);
    }

    for (int j = 0; j < 8; j++) {
        setPieceAt(6, j, whitePieces[8 + j]);
        setPieceAt(7, j, whitePieces[j]);
    }
}
//retorna casa
glm::vec3 Board::getSquarePosition(int row, int col) const
{
    float x = -3.5f + col;
    float z = -3.5f + row;

    return glm::vec3(x, 0.05f, z);
}

bool Board::isSquareEmpty(int row, int col) const
{
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
        return false;

    return grid[row][col] == nullptr;
}

void Board::highlightOccupiedSquares()
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            if (grid[i][j] == nullptr)
                continue;

            glm::vec4 color =
                ((i + j) % 2 == 0)
                ? occupiedLightColor
                : occupiedDarkColor;

            squares[i][j].setColor(color);
        }
    }
}

void Board::clearMoveHighlights()
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            glm::vec4 color =
                ((i + j) % 2 == 0)
                ? lightColor
                : darkColor;

            squares[i][j].setColor(color);
        }
    }
}

bool Board::movePieceInGrid(
    int fromRow,
    int fromCol,
    int toRow,
    int toCol
) {
    if (fromRow < 0 || fromRow >= 8 ||
        fromCol < 0 || fromCol >= 8 ||
        toRow < 0 || toRow >= 8 ||
        toCol < 0 || toCol >= 8)
        return false;

    if (grid[fromRow][fromCol] == nullptr ||
        grid[toRow][toCol] != nullptr)
        return false;

    Object* piece = grid[fromRow][fromCol];

    grid[fromRow][fromCol] = nullptr;
    grid[toRow][toCol] = piece;

    return true;
}
void Board::setHoveredSquare(int row, int col)
{
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        clearHover();
        return;
    }

    if (row == hoveredRow && col == hoveredCol)
        return;

    clearHover();

    squares[row][col].setColor(hoverColor);

    hoveredRow = row;
    hoveredCol = col;
}

void Board::clearHover()
{
    if (hoveredRow == -1 || hoveredCol == -1)
        return;

    glm::vec4 originalColor;

    if (movementMode && grid[hoveredRow][hoveredCol] != nullptr) {
        originalColor =
            ((hoveredRow + hoveredCol) % 2 == 0)
            ? occupiedLightColor
            : occupiedDarkColor;
    }
    else {
        originalColor =
            ((hoveredRow + hoveredCol) % 2 == 0)
            ? lightColor
            : darkColor;
    }

    squares[hoveredRow][hoveredCol].setColor(originalColor);

    hoveredRow = -1;
    hoveredCol = -1;
}

void Board::setMovementMode(bool active)
{
    movementMode = active;
}
