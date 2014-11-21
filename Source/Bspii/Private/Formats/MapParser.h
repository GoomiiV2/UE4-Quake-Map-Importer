// Parse Quake .map files

#pragma once
#include "../Utils.h"

#define Q1PLANEREGEX "\\( (\\S*) (\\S*) (\\S*) \\) \\( (\\S*) (\\S*) (\\S*) \\) \\( (\\S*) (\\S*) (\\S*) \\) (\\w+) (\\S*) (\\S*) (\\S*) (\\S*) (\\S*)"
#define HL1PLANEREGEX ""
#define Q3PLANEREGEX "\\( (\\S*) (\\S*) (\\S*) \\) \\( (\\S*) (\\S*) (\\S*) \\) \\( (\\S*) (\\S*) (\\S*) \\) "

class MapParser
{
public:
	MapParser(Utils* utl);
	~MapParser();

	void Load(FString path);

protected:
	void ReadEntity(); // returns the number of lines that where read
	void ReadBrush(); // returns the number of lines that where read
	bool IsValidLine(FString line); // not a comment etc.

private:
	Utils* BspiiUtils;
	TArray<FString> Lines;
	int CurrentLineIdx;
	FString CurrentLine;
	bool GetNextline();
};
