#include "PC_FileIO.c"

// Global array
string colour1[6][9];
const int FACE_NUM = 6;
const int TILE_NUM = 9;


void readAndTurn(){
	TFileHandle fin;
	bool fileOkay = openReadPC(fin,"output.txt");
	if(!fileOkay)
	{
		displayTextLine(5,"error");
		wait1Msec(5000);
	}
	else
	{
		int lineNum = 0;
		readIntPC(fin, lineNum);
		displayTextLine(3,"%f",lineNum);
		for (int line = 0; line < lineNum; line++){
			string s;
			readTextPC(fin, s);
			displayString(line,s);
		}
		wait1Msec(5000);
		closeFilePC(fin);
	}
}


void writeToFile() {  // No parameters, just use global array
    TFileHandle fout;
    openWritePC(fout, "input.txt");

    int faces[6] = {0,5,1,4,3,2};
		writeLongPC(fout,9);
		for(int i = 0 ; i<FACE_NUM; i++)
		{
			for(int j = 0; j < TILE_NUM-2; j+=3)
			{
				string face_bar;
				int index = faces[i];
				stringFormat(face_bar, "%s%s%s%s%s%s%s","[",colour1[index][j],",",colour1[index][j+1],",",colour1[index][j+2],"]\n");
				writeTextPC(fout,face_bar);
			}
			writeEndlPC(fout);
		}
    closeFilePC(fout);
}


task main() {
    // Initialize array with zeros
		// {0,  0,0,0,0,0,0,0,0}

		string hi[6] = {"W","R","Y","O","B","G"};
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 9; j++) {
            colour1[i][j] = hi[i];
        }
    }

    writeToFile();  // No parameters needed
		readAndTurn();
}
