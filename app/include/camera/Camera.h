//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_CAMERA_H
#define PACMANPROJECT_CAMERA_H


namespace representation {
    class Camera {
        unsigned width;
        unsigned height;

    public:
        Camera(const unsigned windowWidth, const unsigned windowHeight)
            : width(windowWidth), height(windowHeight) {
        }
    };
}


#endif //PACMANPROJECT_CAMERA_H
