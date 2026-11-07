#ifndef BOARD_H
#define BOARD_H
#include "Group.h"
#include "Cube.h"
#include "Texture.h"

class Board : public Group
{
    public:
        Object* grid[8][8];
        Board(
              glm::vec3 pos = glm::vec3(0.0f),
              glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
              );

        virtual ~Board();
        void setPieceAt(int row, int col, Object* piece);
        void setupInitialPieces(std::vector<Object*> whitePieces, std::vector<Object*> blackPieces);

        void setHoveredSquare(int row, int col);
        void clearHover();

    protected:

    private:
    Cube base;
    Cube squares[8][8];
    Texture* texture = nullptr;

    int hoveredRow = -1;
    int hoveredCol = -1;

    glm::vec4 lightColor = glm::vec4(0.9f, 0.85f, 0.70f, 1.0f);
    glm::vec4 darkColor = glm::vec4(0.40f, 0.26f, 0.13f, 1.0f);

    glm::vec4 hoverColor = glm::vec4(0.15f, 0.45f, 0.95f, 1.0f);
};

#endif // BOARD_H
