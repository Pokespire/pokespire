#include "Camera.h"
#include "input/Input.h"
#include "utility/misc.h"

using WalrusRPG::Camera;
using namespace WalrusRPG;
using WalrusRPG::Input::Key;

Camera::Camera(signed x, signed y)
    : x(x), y(y), render_area_width(320), render_area_height(240)
{
}

Camera::~Camera()
{
    // TODO if you allocate dynamically members
}

void Camera::update()
{
    // TODO update map's data according to elasped time
    /*
                // Need to think aagain on how to go to a target point and/or we need to
       align the corner OR the center to this point.
                position += velocity * dt;
                velocity += acceleration * dt;
         */

    // if (Input::key_down(Key::K_DOWN))
    //     y++;
    // if (Input::key_down(Key::K_UP))
    //     y--;
    // if (Input::key_down(Key::K_RIGHT))
    //     x++;
    // if (Input::key_down(Key::K_LEFT))
    //     x--;
}

void Camera::set_x(signed x)
{
    this->x = x;
}

signed Camera::get_x() const
{
    return this->x;
}

void Camera::set_y(signed y)
{
    this->y = y;
}

signed Camera::get_y() const
{
    return this->y;
}

void Camera::set_center_x(signed x)
{
    this->x = x - render_area_width / 2;
}

signed Camera::get_center_x() const
{
    return this->x - render_area_height / 2;
}

void Camera::set_center_y(signed y)
{
    this->y = y - render_area_height / 2;
}

signed Camera::get_center_y() const
{
    return this->y - render_area_height / 2;
}


bool Camera::is_visible(const WalrusRPG::Utils::Rect &object) const
{
    if ((in_range(object.x, x, x + (signed) render_area_width) ||
         in_range(object.x + (signed) object.width - 1, x,
                  x + (signed) render_area_width)) &&
        (in_range(object.y, y, y + (signed) render_area_height) ||
         in_range(object.y + (signed) object.height - 1, y,
                  y + (signed) render_area_height)))
    {
        return true;
    }
    else
    {
        return false;
    }
}
