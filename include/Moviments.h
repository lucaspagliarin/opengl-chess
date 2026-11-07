#ifndef MOVIMENTS_H
#define MOVIMENTS_H


class Moviments
{
    public:
        Moviments();
        virtual ~Moviments();

    protected:

    private:
        float speedFP = 4.0f ; // wasd primeira pessoa
        float cameraSpeed = 15.0f; //camera 3 pessoa
        float t = 0.0f;//fator curva
        float transitionDuration = 1.5f;
};

#endif // MOVIMENTS_H
