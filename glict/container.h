#ifndef _GLIT_CONTAINER_H
#define _GLIT_CONTAINER_H
#include <vector>
#include "types.h"
using namespace std;
class glitContainer  {
    public:
        glitContainer();
        glitContainer(long guid);
        ~glitContainer();
        
        // the following elements are replaced, not inherited in child  classes
        virtual void Paint();
        
        // the following elements are always inherited from container
        void CPaint();
        void AddObject(glitContainer* object);
        void RemoveObject(glitContainer* object);
        void SetHeight(int h);
        void SetWidth(int w);
        void SetPos(int x, int y);
        void SetPos(glitPos pos);
        void GetPos(int* x, int* y);
        void GetPos(glitPos* pos);
        void GetSize(glitSize* size);
        void SetScissor(); // this one adjusts the clipping window through which something can be seen, and which is set by SetClip
        
        void CastEvent(glitEvents evt, void* wparam, long lparam);
        
        void ReportDebug();


		int height, width;
        int x, y;
        int left, right, top, bottom;
        int clipleft, clipright, cliptop, clipbottom;
        
    private:
        // these should be called only internally
        void SetRect(int left, int top, int right, int bottom);
        void SetClip(int left, int top, int right, int bottom);

    protected:
        vector <glitContainer*> objects;
        /*int height, width;
        int x, y;
        int left, right, top, bottom;
        int clipleft, clipright, cliptop, clipbottom;*/
        char objtype[50];
        glitContainer* parent; 
        unsigned int guid;
};
#endif
