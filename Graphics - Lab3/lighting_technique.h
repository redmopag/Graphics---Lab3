#pragma once
// Методы освящения
#include "technique.h"
#include "math_3d.h"

// Определение направления света
struct DirectionLight
{
    Vector3f Color;
    float AmbientIntensity;
    Vector3f Direction; // Направление
    float DiffuseIntensity; // интенсивность рассеивания
};

// Методы освящения
class LightingTechnique : public Technique
{
public:
    LightingTechnique();
    virtual bool Init(); // Создание шейдеров вызывает методы добавления шейдеров и линковки с проверкой

    // Настройка мировых координат
    void SetWVP(const Matrix4f& WVP);
    // Функция для отправления матрицы мировых преобразований
    void SetWorldMatrix(const Matrix4f& WVP);
    // Настройка текстур
    void SetTextureUnit(unsigned int TextureUnit);
    // Функция назначает параметры направленного света в шейдере
    // Ещё нужна для вектора направления и интенсивности рассеивания
    void SetDirectionalLight(const DirectionLight& Light);

private:
    GLuint m_WVPLocation; // Система координат камеры
    GLuint m_WorldMatrixLocation; // Матрица мировых преобразований
    GLuint m_samplerLocation; // Семплер для текстуры
    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint Direction;
        GLuint DiffuseIntensity;
    } m_dirLightLocation; // Структура, описывывающая направленный свет
};