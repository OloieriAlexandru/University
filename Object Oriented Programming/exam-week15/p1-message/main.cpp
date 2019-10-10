#include <iostream>
#include "Message.h"
#include "Sticker.h"
#include "WhiteBoard.h"

int main ()
{
	Message *wall = new WhiteBoard (0, 0, 0, "MindMap", "violet");
	Message *stick1 = new Sticker (wall, 10, 10, "Vacanta", "rosu");
	Message *stick2 = new Sticker (wall, 10, 50, "Munte", "albastru");
	Message *stick3 = new Sticker (wall, 50, 10, "Tabara", "roz");
	Message *stick4 = new Sticker (wall, 50, 50, "Examen PA", "rosu inchis");
	Message *stick5 = new Sticker (wall, 150, 10, "Examen POO", "maro");
	Message *stick6 = new Sticker (wall, 100, 10, "Familie", "galben");
	Message *stick7 = new Sticker (stick6, 20, 30, "Iresire la Ciric", "verde");
	Message *stick8 = new Sticker (stick7, 20, 20, "Gratar", "rosu aprins");

	wall->Attach (stick1);
	wall->Attach (stick2);
	wall->Attach (stick3);
	wall->Attach (stick4);
	wall->Attach (stick5);
	wall->Attach (stick6);
	stick6->Attach (stick7);
	stick7->Attach (stick8);

	wall->Afiseaza ();
	return 0;
}