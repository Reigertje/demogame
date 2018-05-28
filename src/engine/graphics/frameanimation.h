#ifndef FRAMEANIMATION
#define FRAMEANIMATION

#include <limits>
#include <vector>

class FrameAnimation {

	private: 

		static const int ANIMATION_ENDED = std::numeric_limits<int>::max() - 1;

		static const int DEFAULT_DELAY = 100;

		int frame_count_;

		std::vector<int> frame_delays_;

		float current_delay_;

		int current_frame_;

		bool repeat_;

		float speed_;

	public:

		FrameAnimation(int frame_count, bool repeat = true);

		FrameAnimation(int frame_count, int delay, bool repeat = true);

		FrameAnimation(int frame_count, int * delays, bool repeat = true);

		void reset();

		int getCurrentFrame() const;

		int getFrameCount() const;

		bool hasFinished() const;

		bool doesRepeat() const;

		void setCurrentFrame(int frame);

		void setUniformDelay(int delay);

		void setDelay(int frame, int delay);

		void setDelays(int * delays);

		void setRepeat(bool repeat);

		void step(float dt);

		float getDuration() const;

		float getDurationWithoutSpeed() const;

		void setSpeed(float speed);

		float getSpeed() const;

		void transition(const FrameAnimation& other); 

};

#endif