#include "container.h"
#include "types.h"
class glitPanel : public glitContainer {
    public:
        glitPanel();
        ~glitPanel();
        void Paint();
        void SetBGColor(float r, float g, float b, float a);
    private:
        glitColor bgcolor;
};
