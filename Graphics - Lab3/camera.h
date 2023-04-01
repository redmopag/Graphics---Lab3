#pragma once
#ifndef CAMERA_H
#define	CAMERA_H

#include "math_3d.h"


class Camera
{
public:
    // Устанавливает камеры по умолчанию в начале координат в сторону уменьшения z (как в шутерах)
    // Верхний вектор up устанавливается вверх (снова как в шутерах)
    Camera();

    // Позволяет настроить камеру самостоятельно
    Camera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

    // Обрабатывает событие нажатия на клавиатуру и двигает камеру в соотвествии с нажатой кнопкой
    // Вперёд, назад, влево, вправо
    bool OnKeyboard(int Key);

    // Геттеры для трёх свойств
    //----------------------------------------------------------------------------------------------
    const Vector3f& GetPos() const
    {
        return m_pos;
    }

    const Vector3f& GetTarget() const
    {
        return m_target;
    }

    const Vector3f& GetUp() const
    {
        return m_up;
    }
    //----------------------------------------------------------------------------------------------

private:

    Vector3f m_pos; // Позиция камеры
    Vector3f m_target; // Направление камеры
    Vector3f m_up; // Вертикальный вектор камеры
};

#endif	/* CAMERA_H */
