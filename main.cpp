#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.h>
#include <Application.h>
#include <Cube.h>
#include <Cylinder.h>
#include <Pyramid.h>
#include <Sphere.h>
#include <Group.h>
#include <Pawn.h>
#include <Rook.h>
#include <Queen.h>
#include <King.h>
#include <Knight.h>
#include <Bishop.h>
#include <Texture.h>
#include <Board.h>
#include <Moviments.h>
#include <iostream>

bool firstMouse = true;

float lastX = 512.0f;
float lastY = 384.0f;
//função de retorno deslocamento do mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if (!Moviments::mouseAtivo){
     firstMouse = true;
     return;
    }

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.0005f;

    Moviments::globalXOffset = xoffset * sensitivity;
    Moviments::globalYOffset = yoffset * sensitivity;
}
//retorno cordenadas click
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Botão esquerdo clicado!" << std::endl;

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << "X: " << xpos << " | Y: " << ypos << std::endl;
    }
}

std::pair<int, int> getBoardCoords(double mouseX, double mouseY, int larguraInt, int alturaInt, glm::mat4 projection, glm::mat4 view) {

    float larguraJanela = static_cast<float>(larguraInt);
    float alturaJanela = static_cast<float>(alturaInt);

    float x_ndc = 2.0f * static_cast<float>(mouseX) / larguraJanela - 1.0f;
    float y_ndc = 1.0f - (2.0f * static_cast<float>(mouseY) / alturaJanela);

    glm::vec4 ray_clip_near = glm::vec4(x_ndc, y_ndc, -1.0f, 1.0f);
    glm::vec4 ray_clip_far  = glm::vec4(x_ndc, y_ndc,  1.0f, 1.0f);

    // 4. Inverte a matriz (Projection * View) para trazer para o Mundo
    glm::mat4 invMatriz = glm::inverse(projection * view);

    glm::vec4 ray_world_near = invMatriz * ray_clip_near;
    glm::vec4 ray_world_far  = invMatriz * ray_clip_far;

    // Normalização homogênea (divide pelo W)
    if (ray_world_near.w != 0.0f) ray_world_near /= ray_world_near.w;
    if (ray_world_far.w  != 0.0f) ray_world_far  /= ray_world_far.w;

    glm::vec3 pNear = glm::vec3(ray_world_near);
    glm::vec3 pFar  = glm::vec3(ray_world_far);


    // 5. Acha a direção do raio e faz a intersecção com o plano Y = 0
    glm::vec3 direcaoRaio = glm::normalize(pFar - pNear);

    int j = -1;
    int i = -1;

    // Evita divisão por zero caso o raio esteja perfeitamente paralelo ao plano
    if (std::abs(direcaoRaio.y) > 0.0001f) {
        float t = -pNear.y / direcaoRaio.y;
        glm::vec3 pontoNoTabuleiro = pNear + (direcaoRaio * t);

        // 6. Traduz o X e Z do mundo para os índices da sua matriz de xadrez [i][j]
        float startX = -3.5f;
        float startZ = -3.5f;

        j = static_cast<int>(std::floor(pontoNoTabuleiro.x - startX + 0.5f)); // Coluna
        i = static_cast<int>(std::floor(pontoNoTabuleiro.z - startZ + 0.5f)); // Linha
    }

    return {i, j};

}

std::pair<double, double> getMousePosition(GLFWwindow* window, bool mouseAtivo) {

    int largura, altura;
    glfwGetWindowSize(window, &largura, &altura);

    if (mouseAtivo) {
        return {
            largura / 2.0,
            altura / 2.0
        };
    }

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    return {mouseX, mouseY};
}

void drawCrosshair()
{
    static unsigned int VAO = 0;
    static unsigned int VBO = 0;

    if (VAO == 0)
    {
        // Quatro segmentos formando uma mira
        float vertices[] = {
            // Horizontal
            -0.015f,  0.0f,
             0.015f,  0.0f,

            // Vertical
             0.0f, -0.025f,
             0.0f,  0.025f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                              2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    glBindVertexArray(VAO);

    glDisable(GL_DEPTH_TEST);

    glLineWidth(2.0f);

    glDrawArrays(GL_LINES, 0, 4);

    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);
}

int main() {
    // Cria janela e inicializa OpenGL
    Application app(1024, 768, "Cubos OOP");
    if (!app.init()) return -1;

    glfwSetCursorPosCallback(app.getWindow(), mouse_callback);
    glfwSetMouseButtonCallback(app.getWindow(), mouse_button_callback);

    Moviments cameraMovs;
    // Shader
    Shader shader("vertex.glsl", "fragment.glsl");
    Shader crosshairShader("crosshair_vertex.glsl", "crosshair_fragment.glsl");

    shader.use();

    // Carrega texturas
    Texture tex1("wood.jpg");

    shader.setInt("texture1", 0);

//grupo de peças
    Group pecas(
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f),
        0.0f
    );
    //cria tabuleiro
    Board tabuleiro(glm::vec3(0.0f),glm::vec4(1.0f,0.0f,0.0f,1.0f));
    tabuleiro.setupInitialPieces();

    // Ativa depth test
    glEnable(GL_DEPTH_TEST);

glm::mat4 view;
//controle da tecla i
bool iKeyPressionadaUltimoFrame = false;

Object* obj_Selected=nullptr;
int selectedRow = -1;
int selectedCol = -1;

int destinationRow = -1;
int destinationCol = -1;

bool moveInProgress = false;
bool moveCancelled = false;
bool cancelInProgress = false;

bool escPressionadaUltimoFrame = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(app.getWindow())) {

        if (glfwGetKey(app.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
           glfwSetWindowShouldClose(app.getWindow(), true);

        // Limpa tela e depth buffer
        glClearColor(0.45f, 0.45f, 0.45f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 model = glm::mat4(1.0f);

        shader.setMat4("model", model);

        // Configura projection
        int larguraInt, alturaInt;
        glfwGetWindowSize(app.getWindow(), &larguraInt, &alturaInt);

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)larguraInt / (float)alturaInt,
            0.1f,
            100.0f
        );

    bool iKeyTriggered = (glfwGetKey(app.getWindow(), GLFW_KEY_I) == GLFW_PRESS);
//volta as peças para posição inicial
    if (iKeyTriggered && !iKeyPressionadaUltimoFrame)
    {
        tabuleiro.setupInitialPieces();
    }
    iKeyPressionadaUltimoFrame = iKeyTriggered;
//calculo para sincronizar com a taxa de atualização
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

// movimentações de camera
   cameraMovs.update(app.getWindow(),deltaTime);
       // Configura view
    view = cameraMovs.getViewMatrix();

    // Posição do mouse ou da mira central
    auto [mouseX, mouseY] = getMousePosition(app.getWindow(), Moviments::mouseAtivo);

    int largura, altura;
    glfwGetWindowSize(app.getWindow(), &largura, &altura);

    // Identifica a casa apontada
    auto [i, j] = getBoardCoords(
        mouseX,
        mouseY,
        largura,
        altura,
        projection,
        view
    );

        bool mouseLeftPressed =
        glfwGetMouseButton(app.getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

        static bool clickUltimoFrame = false;

        if (mouseLeftPressed && !clickUltimoFrame) {

            if (i >= 0 && i < 8 && j >= 0 && j < 8) {
                std::cout << "Casa clicada -> Linha: " << i
                          << " | Coluna: " << j << std::endl;
            } else {
                std::cout << "Clicou fora do tabuleiro!" << std::endl;
            }
        }

        clickUltimoFrame = mouseLeftPressed;

        // Atualiza o hover
        if(!cameraMovs.isTransitioning()){
        tabuleiro.setHoveredSquare(i, j);
        }
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);


        // =====================================================
        // CONTROLE DE SELEÇÃO E MOVIMENTAÇÃO DAS PEÇAS
        // =====================================================

        // ESC cancela a seleção ou o movimento.
        // Sem peça selecionada, não executa nenhuma ação.
        bool escPressionada =
            glfwGetKey(app.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS;

        if (escPressionada && !escPressionadaUltimoFrame) {

            if (obj_Selected != nullptr) {

                cancelInProgress = true;

                if (moveInProgress) {
                    moveCancelled = true;
                }

                obj_Selected->cancelMovement();
            }
        }

        escPressionadaUltimoFrame = escPressionada;


        // Atualiza a animação da peça selecionada.
        if (obj_Selected != nullptr) {

            obj_Selected->updateMovement(deltaTime);

            // A animação terminou.
            if (!obj_Selected->isAnimating() &&
                !obj_Selected->isSelected()) {

                // Se havia um movimento, atualiza a matriz.
                if (moveInProgress && !moveCancelled) {

                    tabuleiro.movePieceInGrid(
                        selectedRow,
                        selectedCol,
                        destinationRow,
                        destinationCol
                    );
                }

                // Limpa o estado da seleção.
                if (moveInProgress || cancelInProgress) {

                    obj_Selected = nullptr;

                    selectedRow = -1;
                    selectedCol = -1;

                    destinationRow = -1;
                    destinationCol = -1;

                    moveInProgress = false;
                    moveCancelled = false;
                    cancelInProgress = false;

                    tabuleiro.clearMoveHighlights();
                }
            }
        }


        // Detecta o clique do mouse.
        mouseLeftPressed =
            glfwGetMouseButton(app.getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

        clickUltimoFrame = false;

        if (mouseLeftPressed && !clickUltimoFrame) {

            // Não aceita novos cliques enquanto a peça está animando
            // ou retornando à posição original.
            bool podeInteragir =
                obj_Selected == nullptr ||
                (!obj_Selected->isAnimating() &&
                 !moveInProgress &&
                 !cancelInProgress);

            if (podeInteragir) {

                double mouseX, mouseY;

                if (Moviments::mouseAtivo) {
                    // Primeira pessoa: utiliza o centro da tela.
                    mouseX = larguraInt / 2.0;
                    mouseY = alturaInt / 2.0;
                }
                else {
                    glfwGetCursorPos(app.getWindow(), &mouseX, &mouseY);
                }

                auto [i, j] = getBoardCoords(
                    mouseX,
                    mouseY,
                    larguraInt,
                    alturaInt,
                    projection,
                    view
                );

                // Só processa cliques dentro do tabuleiro.
                if (i >= 0 && i < 8 && j >= 0 && j < 8) {

                    // =============================================
                    // NENHUMA PEÇA SELECIONADA
                    // =============================================

                    if (obj_Selected == nullptr) {

                        Object* clickedPiece = tabuleiro.grid[i][j];

                        // Só permite selecionar casas ocupadas.
                        if (clickedPiece != nullptr) {

                            if (clickedPiece->selectForMovement()) {

                                obj_Selected = clickedPiece;

                                selectedRow = i;
                                selectedCol = j;

                                tabuleiro.highlightOccupiedSquares();
                                tabuleiro.setMovementMode(true);
                            }
                        }
                    }

                    // =============================================
                    // JÁ EXISTE UMA PEÇA SELECIONADA
                    // =============================================

                    else {

                        // Clicou novamente na casa original:
                        // cancela a seleção.
                        if (i == selectedRow && j == selectedCol) {

                            cancelInProgress = true;

                            obj_Selected->cancelMovement();
                            tabuleiro.setMovementMode(false);
                        }

                        // Clicou em uma casa vazia:
                        // inicia a movimentação.
                        else if (tabuleiro.isSquareEmpty(i, j)) {

                            destinationRow = i;
                            destinationCol = j;

                            glm::vec3 destination =
                                tabuleiro.getSquarePosition(i, j);

                            if (obj_Selected->moveTo(destination)) {

                                moveInProgress = true;
                                moveCancelled = false;
                            }
                            tabuleiro.setMovementMode(false);
                        }

                        // Casa ocupada: não faz nada.
                    }
                }
            }
        }

        clickUltimoFrame = mouseLeftPressed;



        // Bind texturas
        tex1.bind(0);
        //tex2.bind(1);

        pecas.draw(shader, model);
        tabuleiro.draw(shader,model);

        // Crosshair
        if (Moviments::mouseAtivo)
        {
            crosshairShader.use();
            drawCrosshair();

            shader.use();
        }


        // Swap buffers e eventos
        glfwSwapBuffers(app.getWindow());
        glfwPollEvents();
    }

    return 0;
}
