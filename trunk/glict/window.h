#include <string>
#include "container.h"
#include "types.h"
class glitWindow : public glitContainer {
    public:
        glitWindow();
        ~glitWindow();
        void Paint();
        void SetBGColor(float r, float g, float b, float a);
        void SetCaption(std::string caption);
    private:
        glitColor bgcolor;
        std::string caption;
};
