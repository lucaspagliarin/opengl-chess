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

    protected:

    private:
    Cube base;
    Cube squares[8][8];
    Texture* texture = nullptr;
};

#endif // BOARD_H
