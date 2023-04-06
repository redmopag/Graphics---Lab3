﻿#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "util.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

struct Vertex
{
    Vector3f m_pos; // Координата фигуры
    Vector2f m_tex; // Координата текстуры
    Vector3f m_normal; // Координаты нормали

    Vertex() {}

    Vertex(Vector3f pos, Vector2f tex)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = Vector3f(0.0f, 0.0f, 0.0f); // Координаты по умолчанию 0, 0, 0
    }
};

class Main : public ICallbacks
{
public:

    Main()
    {
        m_pGameCamera = NULL;
        m_pTexture = NULL;
        m_pEffect = NULL;
        m_scale = 0.0f;
        m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        m_directionalLight.AmbientIntensity = -0.1f;
        m_directionalLight.DiffuseIntensity = 0.0f;
        m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);
    }

    ~Main()
    {
        delete m_pEffect;
        delete m_pGameCamera;
        delete m_pTexture;
    }

    bool Init()
    {
        // Векторы, описывающие камеру по-умолчанию
        Vector3f Pos(0.0f, 0.0f, 0.0f);
        Vector3f Target(0.0f, 0.0f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);

        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

        unsigned int Indices[] = { 0, 2, 1,
                                   0, 3, 2 };

        CreateVertexBuffer(Indices, ARRAY_SIZE_IN_ELEMENTS(Indices));
        CreateIndexBuffer(Indices, sizeof(Indices));

        m_pEffect = new LightingTechnique();

        if (!m_pEffect->Init())
        {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        m_pEffect->Enable();

        m_pEffect->SetTextureUnit(0);

        m_pTexture = new Texture(GL_TEXTURE_2D, "D:/Learning/Graphics/Labs/LW-3/Graphics-Lab3/Graphics---Lab3/Graphics - Lab3/test.png");

        if (!m_pTexture->Load()) {
            return false;
        }

        return true;
    }

    void Run()
    {
        GLUTBackendRun(this);
    }

    virtual void RenderSceneCB()
    {
        m_pGameCamera->OnRender();

        glClear(GL_COLOR_BUFFER_BIT);

        m_scale += 0.1f; // Изменяем масштаб для движения

        // Настроивание точки света
        PointLight pl[3];
        pl[0].DiffuseIntensity = 0.5f;
        pl[0].Color = Vector3f(1.0f, 0.0f, 0.0f);
        pl[0].Position = Vector3f(sinf(m_scale) * 10, 1.0f, cosf(m_scale) * 10);
        pl[0].Attenuation.Linear = 0.1f;

        pl[1].DiffuseIntensity = 0.5f;
        pl[1].Color = Vector3f(0.0f, 1.0f, 0.0f);
        pl[1].Position = Vector3f(sinf(m_scale + 2.1f) * 10, 1.0f, cosf(m_scale + 2.1f) * 10);
        pl[1].Attenuation.Linear = 0.1f;

        pl[2].DiffuseIntensity = 0.5f;
        pl[2].Color = Vector3f(0.0f, 0.0f, 1.0f);
        pl[2].Position = Vector3f(sinf(m_scale + 4.2f) * 10, 1.0f, cosf(m_scale + 4.2f) * 10);
        pl[2].Attenuation.Linear = 0.1f;

        m_pEffect->SetPointLights(3, pl);

        // Конвейер для камеры
        Pipeline p;
        // Вращение
        p.Rotate(0.0f, 0.0f, 0.0f);
        // Мировая позиция
        p.WorldPos(0.0f, 0.0f, 1.0f);
        // Настройка позиции камеры
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        // Настройка перспективы
        p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
        m_pEffect->SetWVP(p.GetWVPTrans()); // Настройка мировой позиции
        // вычисление матрицы мировых преобразований, отдельно от матрицы WVP
        const Matrix4f& WorldTransformation = p.GetWorldTrans();
        // Устанавливаем матрицу мировых преобразований
        m_pEffect->SetWorldMatrix(WorldTransformation);
        m_pEffect->SetDirectionalLight(m_directionalLight); // Настройка освящения
        // Настройка отражения
        m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
        m_pEffect->SetMatSpecularIntensity(1.0f);
        m_pEffect->SetMatSpecularPower(32);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        // включаем и выключаем 3 атрибут вершины и указываем смещение
        // нормалей внутри вершинного буфере.
        // Смещение равно 20, так как перед нормалью позиция (12 байт)
        // и координаты текстуры (8 байт)
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        m_pTexture->Bind(GL_TEXTURE0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glutSwapBuffers();
    }

    virtual void IdleCB()
    {
        RenderSceneCB();
    }

    // Функция для обратного вызова, обрабатывающая нажатие клаивиши на клавиатуре
    // Принимает занчение клавиши и координаты мыши в момент нажатия клавиши
    virtual void SpecialKeyboardCB(int Key, int x, int y)
    {
        m_pGameCamera->OnKeyboard(Key);
    }

    // Реагирование на нажатие ввод с клавиши: выход, увеличение освящения, уменьжение освящения
    virtual void KeyboardCB(unsigned char Key, int x, int y)
    {
        switch (Key) {
        case 'q':
            glutLeaveMainLoop();
            break;
        case 'a':
            m_directionalLight.AmbientIntensity += 0.05f;
            break;
        case 's':
            m_directionalLight.AmbientIntensity -= 0.05f;
            break;
        case 'z':
            m_directionalLight.DiffuseIntensity += 0.05f;
            break;
        case 'x':
            m_directionalLight.DiffuseIntensity -= 0.05f;
            break;
        }
    }

    // Реагирование на движение мыши
    virtual void PassiveMouseCB(int x, int y)
    {
        m_pGameCamera->OnMouse(x, y);
    }

private:
    // функция принимает массив индексов, получает вершины треугольников,
    // полагаясь на них, и вычисляет нормали
    void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount,
        Vertex* pVertices, unsigned int VertexCount) {
        // В первом цикле мы только набираем нормали для каждой тройки вершин. Для каждого
        // треугольника она вычисляется
        // как векторное произведение двух сторон, которые получаются из вершин треугольника
        for (unsigned int i = 0; i < IndexCount; i += 3) {
            unsigned int Index0 = pIndices[i];
            unsigned int Index1 = pIndices[i + 1];
            unsigned int Index2 = pIndices[i + 2];
            Vector3f v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
            Vector3f v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
            Vector3f Normal = v1.Cross(v2);
            Normal.Normalize();

            pVertices[Index0].m_normal += Normal;
            pVertices[Index1].m_normal += Normal;
            pVertices[Index2].m_normal += Normal;
        }

        for (unsigned int i = 0; i < VertexCount; i++) {
            pVertices[i].m_normal.Normalize();
        }
    }

    // Создание буфера вершин
    void CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount)
    {
        Vertex Vertices[4] = { Vertex(Vector3f(-10.0f, -2.0f, -10.0f), Vector2f(0.0f, 0.0f)),
                               Vertex(Vector3f(10.0f, -2.0f, -10.0f), Vector2f(1.0f, 0.0f)),
                               Vertex(Vector3f(10.0f, -2.0f, 10.0f), Vector2f(1.0f, 1.0f)),
                               Vertex(Vector3f(-10.0f, -2.0f, 10.0f), Vector2f(0.0f, 1.0f)) };

        unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(Vertices);

        CalcNormals(pIndices, IndexCount, Vertices, VertexCount);

        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    }

    // Создание буфера индексов
    void CreateIndexBuffer(const unsigned int* pIndices, unsigned int SizeInBytes)
    {
        unsigned int Indices[] = { 0, 3, 1,
                                   1, 3, 2,
                                   2, 3, 0,
                                   1, 2, 0 };

        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
    }

    GLuint m_VBO;
    GLuint m_IBO;
    LightingTechnique* m_pEffect;
    Texture* m_pTexture;
    Camera* m_pGameCamera;
    float m_scale;
    DirectionalLight m_directionalLight;
};

int main(int argc, char** argv)
{
    GLUTBackendInit(argc, argv);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "OpenGL tutors")) {
        return 1;
    }

    Main* pApp = new Main();

    if (!pApp->Init()) {
        return 1;
    }

    pApp->Run();

    delete pApp;

    return 0;
}