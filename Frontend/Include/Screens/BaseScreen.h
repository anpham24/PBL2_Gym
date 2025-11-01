#ifndef BASESCREEN_H
#define BASESCREEN_H

#include <SFML/Graphics.hpp>

using namespace sf;

/**
 * @brief Lớp cơ sở ảo (Interface) cho tất cả các màn hình.
 * * Định nghĩa các hàm chung mà vòng lặp ứng dụng chính sẽ gọi,
 * cho phép quản lý các màn hình một cách đa hình.
 */
class BaseScreen {
public:
    // Virtual destructor
    virtual ~BaseScreen() {}

    /**
     * @brief Xử lý các sự kiện đầu vào (chuột, bàn phím).
     */
    virtual void handleEvent(Event& event, const Vector2f& mousePos) = 0;

    /**
     * @brief Cập nhật logic của màn hình (ví dụ: con trỏ nhấp nháy).
     */
    virtual void update(float dt) = 0;

    /**
     * @brief Vẽ tất cả các thành phần của màn hình lên cửa sổ.
     */
    virtual void draw(RenderWindow& window) = 0;
};

#endif // BASESCREEN_H