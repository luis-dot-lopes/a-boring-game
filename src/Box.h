#ifndef BOX_H
#define BOX_H

#include <cmath>
#include "SDL.h"

class Box {
    public:

        //public variables:
        //the position and dimensions of the box.
        double x;
        double y;

        double width;
        double height;

        //main constructor:
        //Initialize box with dimensions.
        Box(double x, double y, double width, double height);

        //Functions:
        //  Collision Functions:
        bool has_collided(const Box *other);

        Box* get_intersection(const Box *other);

        //  Converting to SDL_rect
        SDL_Rect as_SDL_rect();

        Box();
        virtual ~Box();

    protected:

    private:
};

#endif // BOX_H
