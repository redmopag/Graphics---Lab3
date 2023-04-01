#include <GL/freeglut.h>
#include "camera.h"

const static float StepScale = 0.1f;

Camera::Camera()
{
    m_pos = Vector3f(0.0f, 0.0f, 0.0f); // Позиция в начале координат
    m_target = Vector3f(0.0f, 0.0f, 1.0f); // Направление в сторону убывающей z
    m_up = Vector3f(0.0f, 1.0f, 0.0f); // Вверхний вектор смотрит вверх по оси y
}


Camera::Camera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
{
    m_pos = Pos;
    m_target = Target;
    m_target.Normalize(); // Нормализация вектора направления (нужно только направление)
    m_up = Up;
    m_up.Normalize(); // Нужно только направление
}


bool Camera::OnKeyboard(int Key)
{
    bool Ret = false; // Флаг - кнопка для движения камеры не нажата

    // Проверка нажатой кнопки на клавиатуре
    switch (Key) {

    case GLUT_KEY_UP: // Камеры движется вперёд
    {
        // Вектор позиции изменяется по направлению вектора target на велечину StepScale
        m_pos += (m_target * StepScale);
        Ret = true;
    }
    break;

    case GLUT_KEY_DOWN: // Камера движется назад
    {
        // позиция изменяется обратно движению вперёд
        m_pos -= (m_target * StepScale); 
        Ret = true;
    }
    break;

    case GLUT_KEY_LEFT: // Камера движется влево
    {
        Vector3f Left = m_target.Cross(m_up); // Вычисления вектор, направленного влево
        Left.Normalize(); // Приведение его к нормальному виду
        Left *= StepScale; // Изменение величины вектора на StepScale
        m_pos += Left; // Изменение позиции влево на StepScale
        Ret = true;
    }
    break;

    case GLUT_KEY_RIGHT: // Камера движется влево
    {
        // Аналогично движению влево
        Vector3f Right = m_up.Cross(m_target);
        Right.Normalize();
        Right *= StepScale;
        m_pos += Right;
        Ret = true;
    }
    break;
    }

    return Ret;
}