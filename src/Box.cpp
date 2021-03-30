#include "Box.h"

//Main constructor
Box::Box(double x, double y, double w, double h) : x(x), y(y), width(w), height(h) {
}

//collision between boxes
bool Box::has_collided(const Box *other) {
    return (x <= other->x + other->width  && x + width  >= other->x) &&
           (y <= other->y + other->height && y + height >= other->y);
}

//get the intersection between boxes
//returns Box(0, 0, 0, 0) if there's no intersection
Box* Box::get_intersection(const Box *other) {
    if(!has_collided(other)) {
        return new Box(0, 0, 0, 0);
    } else {
        float maxX = x > other->x ? x : other->x;
        float minXcorner = x > other->x ? other->x + other->width : x + width;
        float maxY = y > other->y ? y : other->y;
        float minYcorner = y > other->y ? other->y + other->height : y + height;
        return new Box(maxX, maxY, abs(minXcorner - maxX), abs(minYcorner - maxY));
    }
}

//transforms the box into a SDL_Rect
SDL_Rect Box::as_SDL_rect() {
    SDL_Rect rect = { ceil(x), ceil(y), ceil(width), ceil(height) };
    return rect;
}

//Default code
Box::Box() {
    //ctor
}

Box::~Box() {
    //dtor
}
