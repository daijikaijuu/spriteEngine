//
//  seAnimation.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 19.02.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "seAnimation.hpp"
#include "../Debug/Debug.hpp"

namespace spriteEngine {
    seAnimation::seAnimation(const std::string &name, const std::vector<uint32_t> &frames, float_t delay) :
        seResource(seRESOURCE_ANIMATION, name),
        m_frames(frames), m_currentFrameIdx(0),
        m_delay(delay),
        m_elapsedTime(0.0f)
    {
        seAssert(m_delay > 0.0f);
        seAssert(!m_frames.empty());
    }

    seAnimation::~seAnimation() {
    }

    void seAnimation::DoAnimation(float_t elapsedTime) {
        seAssert(elapsedTime > 0);

        m_elapsedTime += elapsedTime;
        if (m_elapsedTime > m_delay) {
            m_currentFrameIdx++;
            if (m_currentFrameIdx >= m_frames.size())
                m_currentFrameIdx = 0;
            m_elapsedTime = 0.0f;
        }
    }

    uint32_t seAnimation::CurrentFrame() const {
        try {
            return m_frames.at(m_currentFrameIdx);
        } catch (std::out_of_range &e) {
            LogError << "seAnimation::CurrentFrame() index out of range: m_currentFrameIdx=" << m_currentFrameIdx << eol;
            abort();
        }
    }


    seAnimator::seAnimator(uint32_t defaultSprite) :
        seCollection<seAnimation>(),
        m_currentAnimation(nullptr),
        m_animationRunning(false),
        m_defaultSprite(defaultSprite)
    {
    }

    seAnimator::~seAnimator() {
    }

    void seAnimator::AddItem(seAnimation *animation) {
        seAssert(animation);

        seCollection::AddItem(animation);
    }

    void seAnimator::SetCurrentAnimation(const std::string &name) {
        seAssert(!name.empty());

        m_currentAnimation = GetItem(name);
        seAssert(m_currentAnimation);
        m_currentAnimation->ResetAnimation();
        m_animationRunning = true;
    }

    uint32_t seAnimator::CurrentFrame() const {
        if (m_animationRunning) {
            seAssert(m_currentAnimation);
            return m_currentAnimation->CurrentFrame();
        }
        else
            return m_defaultSprite;
    }

    void seAnimator::StopAnimation() {
        m_animationRunning = false;
        m_currentAnimation = nullptr;
    }

    void seAnimator::DoAnimation(float_t elapsedTime) {
        if (m_animationRunning) {
            seAssert(m_currentAnimation);
            seAssert(elapsedTime > 0);

            m_currentAnimation->DoAnimation(elapsedTime);
        }
    }
}
