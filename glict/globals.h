class glitGlobalsClass {
    public:
        glitGlobalsClass();
        ~glitGlobalsClass();
        float windowTitleBgColor[4];
        float windowTitleColor[4];
        float w,h;
        
        bool renderMode;
};
extern glitGlobalsClass glitGlobals;

#define GLIT_RENDERING false
#define GLIT_SELECTING true
