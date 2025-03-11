#include "PC_FileIO.c"

int colour [6][9];
char faces [6] = {'U','F','R','B','L','D'}; //start with original move orientation

void ultrasonic ()
{
	while (SensorValue[S2] <= 2 || SensorValue[S2] >= 9)
	{
		displayTextLine(14,"Cube must be placed in base");
		displayTextLine(15,"for program to run");
	}
	wait1Msec(2000);
}

void flip(int flipTimes, bool updateFace)
{
	const int POS1ANGLE = 130, POS2ANGLE = 195;
	for (int i = 0; i < flipTimes; i++)
	{
		nMotorEncoder(motorA)=0;
		//phase 1
		motor[motorA] = 25;
		while(abs(nMotorEncoder[motorA]) < POS1ANGLE)
		{}
		motor[motorA] = 0;
		wait1Msec(500);
		//phase 2
		motor[motorA] = 25;
		while(abs(nMotorEncoder[motorA]) < POS2ANGLE)
		{}
		motor[motorA] = 0;
		wait1Msec(500);
		motor[motorA] = -25;
		nMotorEncoder(motorA) = 0;
		while(abs(nMotorEncoder[motorA]) < POS2ANGLE-POS1ANGLE)
		{}
		motor[motorA] = 0;
		//wait1Msec(1000);
		motor[motorA] = -25;
		while(abs(nMotorEncoder[motorA]) < POS2ANGLE)
		{}
		motor[motorA] = 0;

		if(updateFace)
		{
			//UPDATE CUBE FACES ARRAY
			char temp = faces[0];
			faces[0] = faces[2];
			// face 1 is the same
			faces[2] = faces[5];
			//face 3 is same
			char temp2 = faces[4];
			faces[4] = temp;
			faces[5] = temp2;
		}
	}
}

void base(int direction, int degree, bool updateFace) // CW: direction = 0, CCW: direction = 1
{
	//270 for 90degree turn
	nMotorEncoder(motorB) = 0;
	if(direction == 0)
	{
		motor[motorB] = 25; // positive is CW
	}
	else
	{
		motor[motorB] = -25; // negative is CCW
	}
	while(abs(nMotorEncoder[motorB]) < degree)
	{}
	motor[motorB] = 0;
	wait1Msec(500);

	if(updateFace && degree%270 == 0){
		//UPDATE CUBE FACES ARRAY
		char temp [6];
		for(int i=0;i<6;i++)
		{
			temp[i] = faces[i];
		}
		int numTurns = degree/270; //either 1 or 2
		if(direction == 1 && numTurns == 1)
		{
			numTurns = -numTurns;
		}
		int newFace = 0;
		for(int i=1; i<5; i++)
		{
			newFace = i+numTurns;
			if(newFace>4)
			{
				newFace -= 4;
			}
			else if(newFace<1)
			{
				newFace += 4;
			}
			faces[i] = temp[newFace];
		}
	}
	wait1Msec(200);
}

void turnWhileHold(int direction, int degree)
{
	//arm holds cube
	const int POS1ANGLE = 130;
	nMotorEncoder(motorA)=0;
	motor[motorA] = 25;
	while(abs(nMotorEncoder[motorA]) < POS1ANGLE)
	{}
	motor[motorA] = 0;
	//turn base
	base(direction, degree, false);
	//remove arm
	nMotorEncoder(motorA)=0;
	motor[motorA] = -25;
	while(abs(nMotorEncoder[motorA]) < POS1ANGLE)
	{}
	motor[motorA] = 0;
}

void turnLayer(char layer, int direction) // front layer is the side with no obstructions, up is facing up
{
	//GET SPECIFIED LAYER TO THE BOTTOM
	if(layer == 'F')//front face
	{
		//turn cube CW by 90 and flip once
		base(0,270,true);
		flip(1,true);
	}
	else if(layer == 'B')//back face
	{
		//turn cube CCW by 90 and flip once
		base(1,270,true);
		flip(1,true);
	}
	else if(layer == 'L')//left face
	{
		//flip once
		flip(1,true);
	}
	else if(layer == 'R')//right face
	{
		//turn cube CW by 180 and flip once
		base(0,540,true);
		flip(1,true);
	}
	else if(layer == 'U')//up face
	{
		//flip twice
		flip(2,true);
	}
	//nothing required for down face

	//TURN THE SPECIFIED LAYER
	const int TURN_DEGREE = 270;
	if(direction == 0)//turn CW
	{
		turnWhileHold(1,TURN_DEGREE);
	}
	else if (direction == 1)//turn CCW
	{
		turnWhileHold(0,TURN_DEGREE);
	}
}

void colourSensor()
{
	const int CENTREANGLE = 440, SIDEANGLE = 360;
	for (int i=0; i<6; i++)
	{
		nMotorEncoder[motorC] = 0;
		motor[motorC] = -80; //moving forward
		while (abs(nMotorEncoder[motorC])< CENTREANGLE)
		{}
		motor[motorC] = 0;
		colour[i][0] = SensorValue[S1]; //stores centre colour
		wait1Msec(1000);

		nMotorEncoder[motorC] = 0;
		motor[motorC] = 80; //moving bacward
		while (abs(nMotorEncoder[motorC])< 90)
		{}
		motor[motorC] = 0;

		for (int j=1; j<8; j++)
		{
			colour[i][1+j] = SensorValue[S1];
			wait1Msec(200);
			base(1,135,false);
			// minor position changes to the sensor for accurate readings
			if(j == 3)
			{
				nMotorEncoder[motorC] = 0;
				motor[motorC] = 80; //moving backward
				while (abs(nMotorEncoder[motorC])< 20)
				{}
				motor[motorC] = 0;
			}
			else if(j == 4)
			{
				nMotorEncoder[motorC] = 0;
				motor[motorC] = -80; //moving forawrd
				while (abs(nMotorEncoder[motorC])< 20)
				{}
				motor[motorC] = 0;
			}
			else if(j == 5)
			{
				nMotorEncoder[motorC] = 0;
				motor[motorC] = 80; //moving bcakward
				while (abs(nMotorEncoder[motorC])< 20)
				{}
				motor[motorC] = 0;
			}
			else if(j == 6)
			{
				nMotorEncoder[motorC] = 0;
				motor[motorC] = -80; //moving forward
				while (abs(nMotorEncoder[motorC])< 20)
				{}
				motor[motorC] = 0;
			}
			else if(j == 7)
			{
				nMotorEncoder[motorC] = 0;
				motor[motorC] = 80; //moving bcakward
				while (abs(nMotorEncoder[motorC]) < 40)
				{}
				motor[motorC] = 0;
			}
			wait1Msec(500);

		}
		nMotorEncoder[motorC] = 0;
		motor[motorC] = 80; //moving backward
		while (abs(nMotorEncoder[motorC]) < 310)
		{}
		motor[motorC] = 0;
		base(1,135,false);
		flip(1,false);
		if (i==3)
		{
			base (0, 270, false);
			flip (1,false);
		}
		if (i==4)
		{
			flip(1,false);
		}
	}
	base(1,270,false);
}

char determineFace(char originalFace) //original face is the face that the algorithm wants to turn
{
	int index = -1;
	for(int i=0;i<6;i++)
	{
		if(originalFace == faces[i]){ // faces is the current orientation of the cube
			index = i;
		}
	}
	char newMove[6] = {'U','F','R','B','L','D'};
	return newMove[index];
}

void randomScramble(int numMoves)
{
	int faceNum = 0;
	int direction = -1;
	char face = 'N'; // Nothing

	TFileHandle fout;
	bool fileOkay = openWritePC(fout,"scramble.txt");

	if(!fileOkay)
	{
		displayTextLine(5,"ERROR");
		wait1Msec(5000);
	}
	else
	{
		writeLongPC(fout,numMoves);
		writeEndlPC(fout);
		for (int i = 0; i < numMoves; i++)
		{
			faceNum = (rand() % (6-1))+1;
			if(faceNum == 1)
			{
				face = 'U';
			}
			else if(faceNum == 2)
			{
				face = 'F';
			}
			else if(faceNum == 3)
			{
				face = 'R';
			}
			else if(faceNum == 4)
			{
				face = 'B';
			}
			else if(faceNum == 5)
			{
				face = 'L';
			}
			else
			{
				face = 'D';
			}
			direction = (rand() % (1-0));
			fileWriteChar(fout,face);
			writeEndlPC(fout);
			writeLongPC(fout,direction);
			writeEndlPC(fout);
		}
		closeFilePC(fout);
	}
}

void readAndTurn(int fileChoice){ //fileChoice: 0 for solve, anything else for scramble
	TFileHandle fin;
	bool fileOkay = false;
	if(fileChoice == 0)
	{
		fileOkay = openReadPC(fin,"output.txt");
	}
	else //randomly scramble
	{
		randomScramble(8); //change number to change number of moves to scramble
		fileOkay = openReadPC(fin,"scramble.txt");
	}
	clearTimer(T1);
	if(!fileOkay)
	{
		displayTextLine(5,"error");
		wait1Msec(5000);
	}
	else
	{
		int lineNum = 0;
		readIntPC(fin, lineNum);
		for (int line = 0; line < lineNum*2-1; line+=2){
			char layer;
			int dir;
			readCharPC(fin, layer);
			readIntPC(fin, dir);
			turnLayer(determineFace(layer),dir);
		}
		float turnTime = time1[T1]/1000.0;
		displayTextLine(1,"Time taken to solve:");
		displayTextLine(2,"%d",turnTime);
		wait1Msec(5000);
		closeFilePC(fin);
	}
}

	task main()
	{
		//configuring all sensors
		SensorType[S2] = sensorEV3_Ultrasonic;
		wait1Msec(50);

		SensorType[S4]= sensorEV3_Color;
		wait1Msec(50);
		SensorMode[S1] = modeEV3Color_Color;
		wait1Msec(50);



		bool end = false;
		while(!getButtonPress(buttonAny))
		{
			displayTextLine(4,"Left or Right Buttons");
			displayTextLine(5,"for Scrambling the Cube");
			displayTextLine(7,"Enter Button for Colour");
			displayTextLine(8,"Sensing and Solving the Cube");
			displayTextLine(10,"Down Button for Exiting");
			displayTextLine(11,"the Program");
			wait1Msec(10);
		}

		while (!end)
	    {

	        // Check for left or right button press to scramble
	        if (getButtonPress(buttonLeft) || getButtonPress(buttonRight))
	        {
	        	ultrasonic();
	            readAndTurn(1);  // Scramble
	            wait1Msec(500);  // Delay after action to avoid multiple triggers too quickly
	        }
	        // Check for Enter button press to color sense and solve
	        else if (getButtonPress(buttonEnter))
	        {
	        	ultrasonic();
	            //colourSensor();
	            displayTextLine(4,"Colour Sensing is");
	            displayTextLine(5,"finished, press the");
	            displayTextLine(6,"Enter Button again");
	            displayTextLine(7,"to solve");
	            displayTextLine(8," ");
	            while(!getButtonPress(buttonEnter))
	            {}
	     		ultrasonic();
	            readAndTurn(0);  // Color sense then solve
	            wait1Msec(500);  // Delay after action to avoid multiple triggers too quickly
	        }
	        // Check for Down button press to end the program
	        else if (getButtonPress(buttonDown))
	        {
	            end = true;  // Ends the program
	        }

	        wait1Msec(50);  // Small delay to prevent excessive CPU usage
	    }
	}