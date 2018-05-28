#include "frameanimation.h"

#include <iostream>

FrameAnimation::FrameAnimation(int frame_count, bool repeat) {
	frame_count_ = frame_count;
	frame_delays_.resize(frame_count_);
	setUniformDelay(DEFAULT_DELAY);
	setRepeat(repeat);
	setSpeed(1.0f);
	reset();
}

FrameAnimation::FrameAnimation(int frame_count, int delay, bool repeat) {
	frame_count_ = frame_count;
	frame_delays_.resize(frame_count_);
	setUniformDelay(delay);
	setRepeat(repeat);
	setSpeed(1.0f);
	reset();
}

FrameAnimation::FrameAnimation(int frame_count, int * delays, bool repeat) {
	frame_count_ = frame_count;
	frame_delays_.resize(frame_count_);
	setDelays(delays);
	setRepeat(repeat);
	setSpeed(1.0f);
	reset();
}

void FrameAnimation::reset() {
	setCurrentFrame(0);
	current_delay_ = frame_delays_[0];
}

int FrameAnimation::getCurrentFrame() const {
	return current_frame_;
}

void FrameAnimation::setCurrentFrame(int frame) {
	current_frame_ = frame;
	current_delay_ = frame_delays_[current_frame_];
}

int FrameAnimation::getFrameCount() const {
	return frame_count_;
}

bool FrameAnimation::hasFinished() const {
	return current_delay_ == ANIMATION_ENDED;
}

bool FrameAnimation::doesRepeat() const {
	return repeat_;
}

void FrameAnimation::setUniformDelay(int delay) {
	for (int i = 0; i < frame_count_; ++i) frame_delays_[i] = delay;
}

void FrameAnimation::setDelay(int frame, int delay) {
	frame_delays_[frame] = delay;
}

void FrameAnimation::setDelays(int * delays) {
	for (int i = 0; i < frame_count_; ++i) setDelay(i, delays[i]);
}

void FrameAnimation::setRepeat(bool repeat) {
	repeat_ = repeat;
}

void FrameAnimation::setSpeed(float speed) {
	speed_ = speed;
} 

float FrameAnimation::getSpeed() const {
	return speed_;
}

float FrameAnimation::getDuration() const {
	return getDurationWithoutSpeed() * speed_;
} 

float FrameAnimation::getDurationWithoutSpeed() const {
	int sum = 0;
	for (int i = 0; i < frame_count_; ++i) {
	    sum += frame_delays_[i];
	}
	return sum;
}

void FrameAnimation::transition(const FrameAnimation& other) {
	// TODO wat als illegal argument? 
	if (other.getCurrentFrame() < getFrameCount()) {
		current_frame_ = other.getCurrentFrame();
		speed_ = other.getSpeed();
		current_delay_ = other.current_delay_;
	}
}

void FrameAnimation::step(float dt) {

	current_delay_ -= (dt * 1000.0f * speed_);

	while (current_delay_ <= 0) {
		current_frame_ = repeat_ ? (current_frame_ + 1)%frame_count_ : 
						current_frame_ < frame_count_ - 1 ? current_frame_ + 1 : 
						frame_count_ - 1;

		if (current_frame_ == frame_count_ - 1 && !repeat_) {
			current_delay_ = ANIMATION_ENDED;
		} else {					
			current_delay_ += frame_delays_[current_frame_];
		}
	}

}