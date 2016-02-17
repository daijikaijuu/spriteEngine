//
//  seCollisionRect.h
//  spriteEngine
//
//  Created by Domrachev Alexandr on 02.02.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seCollisionRect_h
#define seCollisionRect_h

namespace spriteEngine {
    struct seCollisionRect {
        float x, y;
        float width, height;

        seCollisionRect(float x1, float y1, float w, float h) : x(x1), y(y1), width(w), height(h) {}
        seCollisionRect(const seCollisionRect &rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height) {}

        bool Contains(const seCollisionRect rect) const {
            return ((rect.x >= x) && (rect.y >= y) &&
                    (rect.Right() <= Right()) && (rect.Bottom() <= Bottom()));
        }

        bool Intersects(const seCollisionRect rect) const {
            return !((rect.Right() <= x) ||
                     (rect.Bottom() <= y) ||
                     (rect.x >= Right()) ||
                     (rect.y >= Bottom()));
        }

        seCollisionRect Shift(float shiftX, float shiftY) const {
            seCollisionRect result(x + shiftX, y + shiftY, width, height);
            return result;
        }

        float Bottom() const {
            return y + height;
        }

        float Right() const {
            return x + width;
        }
    };
}


#endif /* seCollisionRect_h */
