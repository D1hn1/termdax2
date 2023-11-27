#include "termdax-v.2.h"

int main ()
{
	trm::Termdax root;
	trm::TRM_RECTANGLE rect_one;
	rect_one.Push(0,0,(TRM_WINDOW_X / 2),10,true,' ', TRM_NULL, TRM_BG_BLUE);

	while ( true ) {
	
		switch ( root.Event() ) {

			case TRM_EVENT_RESIZE:
				root.utilities.clear();
				rect_one.y++;
				rect_one.Update();

			default:
				root.Draw();

		};

	};

	root.Endtermdax();
	return 0;
}
