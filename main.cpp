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
#include <iostream>

//globais
float globalXOffset = 0.0f;
float globalYOffset = 0.0f;

bool mouseAtivo = false;
bool firstMouse = true;

float lastX = 512.0f;
float lastY = 384.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if (!mouseAtivo) return;

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

    float sensitivity = 0.005f;

    globalXOffset = xoffset * sensitivity;
    globalYOffset = yoffset * sensitivity;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Botão esquerdo clicado!" << std::endl;

        // Aqui dentro você pode pegar a posição atual do cursor na tela
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

    // Shader
    Shader shader("vertex.glsl", "fragment.glsl");
    Shader crosshairShader("crosshair_vertex.glsl", "crosshair_fragment.glsl");

    shader.use();

    // Carrega texturas
    Texture tex1("wood.jpg");

    shader.setInt("texture1", 0);


    Group pecas(
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f),
        0.0f
    );

    pecas.position = glm::vec3(0.0f, 0.0f, 0.0f);
    pecas.rotation = glm::vec3(0.0f, 1.0f,0.0f);
    pecas.angle = 0.0f;
    pecas.scale = glm::vec3(1.0f);

    // Cores das equipes
    glm::vec4 whiteColor(0.9f, 0.9f, 0.9f, 1.0f);
    glm::vec4 blackColor(0.16f, 0.16f, 0.16f, 1.0f);

    std::vector<Object*> whitePieces;
    std::vector<Object*> blackPieces;

    Pawn* wPawns[8];
    for(int i = 0; i < 8; i++) {
        wPawns[i] = new Pawn(glm::vec3(0.0f), whiteColor);
    }

    Bishop wbispo1(
        glm::vec3(0.0f),
        whiteColor
    );
    Bishop wbispo2(
        glm::vec3(0.0f),
        whiteColor
    );

    Rook wtorre1(
        glm::vec3(0.0f),
        whiteColor
    );
    Rook wtorre2(
        glm::vec3(0.0f),
        whiteColor
    );

    Queen wqueen(
        glm::vec3(0.0f),
        whiteColor
    );

    King wking(
        glm::vec3(0.0f),
        whiteColor
    );

    Knight wknight1(
        glm::vec3(0.0f),
        whiteColor
    );
    Knight wknight2(
        glm::vec3(0.0f),
        whiteColor
    );
    wknight1.angle = 180;
    wknight2.angle = 180;
    whitePieces.push_back(&wtorre1);
    whitePieces.push_back(&wknight1);
    whitePieces.push_back(&wbispo1);
    whitePieces.push_back(&wqueen);
    whitePieces.push_back(&wking);
    whitePieces.push_back(&wbispo2);
    whitePieces.push_back(&wknight2);
    whitePieces.push_back(&wtorre2);
    for(int i=0;i<8;i++){
        whitePieces.push_back(wPawns[i]);
    }

Pawn* bPawns[8];
    for(int i = 0; i < 8; i++) {
        bPawns[i] = new Pawn(glm::vec3(0.0f), blackColor);
    }

    Bishop bbispo1(
        glm::vec3(0.0f),
        blackColor
    );
    Bishop bbispo2(
        glm::vec3(0.0f),
        blackColor
    );

    Rook btorre1(
        glm::vec3(0.0f),
        blackColor
    );
    Rook btorre2(
        glm::vec3(0.0f),
        blackColor
    );

    Queen bqueen(
        glm::vec3(0.0f),
        blackColor
    );

    King bking(
        glm::vec3(0.0f),
        blackColor
    );

    Knight bknight1(
        glm::vec3(0.0f),
        blackColor
    );
    Knight bknight2(
        glm::vec3(0.0f),
        blackColor
    );

    blackPieces.push_back(&btorre1);
    blackPieces.push_back(&bknight1);
    blackPieces.push_back(&bbispo1);
    blackPieces.push_back(&bqueen);
    blackPieces.push_back(&bking);
    blackPieces.push_back(&bbispo2);
    blackPieces.push_back(&bknight2);
    blackPieces.push_back(&btorre2);
    for(int i=0;i<8;i++){
        blackPieces.push_back(bPawns[i]);
    }

    Board tabuleiro(glm::vec3(0.0f),glm::vec4(1.0f,0.0f,0.0f,1.0f));

    tabuleiro.setupInitialPieces(whitePieces,blackPieces);
    // Ativa depth test
    glEnable(GL_DEPTH_TEST);


glm::vec3 cameraPos = glm::vec3(0.0f, 7.0f, 12.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 view;

view = glm::lookAt(cameraPos, cameraTarget, cameraUp);


// Velocidade de movimento da câmera
float speedFP = 4.0f ; // wasd primeira pessoa
float cameraSpeed = 15.0f; //camera 3 pessoa
float t = 0.0f;//fator curva
float transitionDuration = 1.5f; // transição em seg
bool emPrimeiraPessoa = false;
bool estaTransicionando = false;
bool pKeyPressionadaUltimoFrame = false;
bool mKeyPressionadaUltimoFrame = false;

Object* obj_Selected=nullptr;
int selectedRow = -1;
int selectedCol = -1;

int destinationRow = -1;
int destinationCol = -1;

bool moveInProgress = false;
bool moveCancelled = false;
bool cancelInProgress = false;

bool escPressionadaUltimoFrame = false;


glm::vec3 cameraPosInicial = glm::vec3(0.0f);
glm::vec3 cameraPosDestino = glm::vec3(0.0f);
glm::vec3 cameraTargetInicial= glm::vec3(0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
    // Loop principal
    while (!glfwWindowShouldClose(app.getWindow())) {

        // Processa input
//        if (glfwGetKey(app.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
//           glfwSetWindowShouldClose(app.getWindow(), true);

        // Limpa tela e depth buffer
        glClearColor(0.3f, 0.3f, 0.5f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, (angle * (float) glfwGetTime()) / 20, glm::vec3(0.0f, 0.3f, 0.0f));
        //model = glm::rotate(model, -60.0f, glm::vec3(0.0f, 0.3f, 0.0f));

        shader.setMat4("model", model);

        // Configura view e projection
        int larguraInt, alturaInt;
        glfwGetWindowSize(app.getWindow(), &larguraInt, &alturaInt);

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)larguraInt / (float)alturaInt,
            0.1f,
            100.0f
        );


    float currentFrame = glfwGetTime();

    deltaTime = currentFrame - lastFrame;

    lastFrame = currentFrame;

    bool pKeyTriggered = (glfwGetKey(app.getWindow(), GLFW_KEY_P) == GLFW_PRESS);

    if (pKeyTriggered && !pKeyPressionadaUltimoFrame && !estaTransicionando) {
        emPrimeiraPessoa = !emPrimeiraPessoa;
        estaTransicionando = true;

        if (emPrimeiraPessoa) {
            cameraPosInicial = cameraPos;
            cameraPosDestino = glm::vec3(-5.0f, 0.8f, 0.0f);
            t=0.0f;
        }
        else {
            cameraPosInicial = glm::vec3(0.0f, 7.0f, 12.0f);
            cameraPosDestino = cameraPos;
            cameraTargetInicial = cameraTarget;
            t=1.0f;
        }

        if (mouseAtivo) {
            mouseAtivo = false;
            glfwSetInputMode(app.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    pKeyPressionadaUltimoFrame = pKeyTriggered;

    if (estaTransicionando) {
        if (emPrimeiraPessoa) {
            t += deltaTime/transitionDuration;
            if (t >= 1.0f) {
                t = 1.0f;
                estaTransicionando = false;
            }
        }
        else {
            t -= deltaTime/transitionDuration;
            if (t <= 0.0f) {
                t = 0.0f;
                estaTransicionando = false;
            }
        }
    }

    if (estaTransicionando) {
        float fatorCurva = log(1.0f + t * (2.71828f - 1.0f));

        cameraPos = glm::mix(cameraPosInicial, cameraPosDestino, fatorCurva);
        if(!emPrimeiraPessoa){
            glm::vec3 targetFinal = glm::vec3(0.0f, 0.0f, 0.0f);
            cameraTarget = glm::mix(targetFinal, cameraTargetInicial, fatorCurva);
        }

    }

    bool mKeyTriggered = (glfwGetKey(app.getWindow(), GLFW_KEY_M) == GLFW_PRESS);

    if (mKeyTriggered && !mKeyPressionadaUltimoFrame
        && emPrimeiraPessoa && !estaTransicionando) {

        mouseAtivo = !mouseAtivo;

        // Reinicia os deslocamentos acumulados
        globalXOffset = 0.0f;
        globalYOffset = 0.0f;
        firstMouse = true;

        if (mouseAtivo) {
            glfwSetInputMode(app.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(app.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    mKeyPressionadaUltimoFrame = mKeyTriggered;

    if(emPrimeiraPessoa && !estaTransicionando){

        glm::vec3 direction = glm::normalize(cameraTarget - cameraPos);
        glm::vec3 right = glm::normalize(glm::cross(direction, cameraUp));

        if (mouseAtivo && (globalXOffset != 0.0f || globalYOffset != 0.0f)) {

            glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), -globalXOffset, glm::vec3(0.0f, 1.0f, 0.0f));
            direction = glm::vec3(rotX * glm::vec4(direction, 0.0f));

            glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), globalYOffset, right);
            direction = glm::vec3(rotY * glm::vec4(direction, 0.0f));

            direction = glm::normalize(direction);

            globalXOffset = 0.0f;
            globalYOffset = 0.0f;
        }


        glm::vec3 moveDirXZ = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));

        if (glfwGetKey(app.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += moveDirXZ * speedFP * deltaTime;
        if (glfwGetKey(app.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= moveDirXZ * speedFP * deltaTime;
        if (glfwGetKey(app.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= right * speedFP * deltaTime;
        if (glfwGetKey(app.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += right * speedFP * deltaTime;


        cameraPos.y = 1.5f;
        cameraTarget = cameraPos + direction;

    }
        else if (!estaTransicionando){
        glm::vec3 direction = glm::normalize(cameraTarget - cameraPos);
        glm::vec3 right = glm::normalize(glm::cross(direction, cameraUp));

        if (glfwGetKey(app.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= right * cameraSpeed * deltaTime;
        if (glfwGetKey(app.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += right * cameraSpeed * deltaTime;
        if (glfwGetKey(app.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
            cameraPos.y += cameraSpeed * deltaTime;
        if (glfwGetKey(app.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
            cameraPos.y -= cameraSpeed * deltaTime;
        if (glfwGetKey(app.getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
            cameraPos += direction * cameraSpeed * deltaTime;
        if (glfwGetKey(app.getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
            cameraPos -= direction * cameraSpeed * deltaTime;
        }

    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    // Posição do mouse ou da mira central
    auto [mouseX, mouseY] = getMousePosition(app.getWindow(), mouseAtivo);

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
        tabuleiro.setHoveredSquare(i, j);

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

                if (mouseAtivo) {
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
        if (mouseAtivo)
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
