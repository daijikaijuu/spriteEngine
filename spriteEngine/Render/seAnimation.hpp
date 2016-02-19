//
//  seAnimation.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 19.02.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seAnimation_hpp
#define seAnimation_hpp

#include "../Resources/seResource.hpp"
#include "../Utils/seCollection.hpp"
#include <vector>

namespace spriteEngine {
    class seAnimation : public seResource {
    public:
        seAnimation(const std::string &name, const std::vector<uint32_t> &frames, float_t delay);
        virtual ~seAnimation();

        void DoAnimation(float_t elapsedTime);
        void ResetAnimation() { m_currentFrameIdx = 0; }
        uint32_t CurrentFrame() const;

    private:
        std::vector<uint32_t> m_frames;
        uint32_t m_currentFrameIdx;
        float_t m_delay;
        float_t m_elapsedTime;
    };

    class seAnimator : public seCollection<seAnimation> {
    public:
        seAnimator(uint32_t defaultSprite = 0);
        virtual ~seAnimator();

        void AddItem(seAnimation *animation);

        seAnimation* CurrentAnimation() { return m_currentAnimation; }
        uint32_t CurrentFrame() const;
        void SetCurrentAnimation(const std::string &name);
        void StopAnimation();
        void DoAnimation(float_t elapsedTime);

    private:
        seAnimation *m_currentAnimation;
        bool m_animationRunning;
        uint32_t m_defaultSprite;
    };
}

#endif /* seAnimation_hpp */
