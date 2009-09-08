#ifndef __GLICT_PROGRESSBAR_H
#define __GLICT_PROGRESSBAR_H
#include <GLICT/container.h>
#include <GLICT/types.h>

class glictProgressBar : public glictContainer {
	public:
		glictProgressBar();
		~glictProgressBar();

		void Paint();

		void SetValue(float newpcnt);
		void AddValue(float pcnt);
		void SetBGActiveness(bool active) { bgactive = active; }
		void SetBGColor(const glictColor& color) { bgColor = color; }
		void SetFGColor(const glictColor& color) { fgColor = color; }
		void SetBorderColor(const glictColor& color) { borderColor = color; }
		float GetValue();

	private:
		float percent;
		bool bgactive;
		glictColor bgColor;
		glictColor fgColor;
		glictColor borderColor;
};

#endif
