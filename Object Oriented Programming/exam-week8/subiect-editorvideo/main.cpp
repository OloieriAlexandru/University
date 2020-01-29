#include <iostream>
#include "Video.h"
#include "EditorVideo.h"

int main ()
{
	Video video1 ("Skateboarding", 24);
	Video video2 ("Driving", 10);
	Video video3 ("Biking", 19);
	EditorVideo editor = { video1, video2 };
	editor += video3;
	editor.afiseaza_intervale_video ();
	editor = ((video1 | video3) | video2);
	for (auto vid : editor)
	{
		std::cout << vid << std::endl;
	}
	return 0;
}