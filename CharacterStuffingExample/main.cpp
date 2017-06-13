#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

const char DLE = 0x10;
const char STX = 0x02;
const char ETX = 0x03;

string ConvertToFrameAndStuff(string input);
string ConvertFromFrameAndDestuff(string frame);
void PrintFrame(string frame);

int main()
{
	string input;
	cout << "Character Stuffing Example" << endl;

	//Send
	cout << "For testing purposes, replacing 'd' to DLE, 'e' to ETX, 's' to STX" << endl;
	cout << "Input the string you want to transmit " << endl;
	cin >> input;
	cout << endl << "Converting: " << endl;
	cout << input << endl;
	cout << "to: " << endl;

	replace(input.begin(), input.end(), 'd', DLE);
	replace(input.begin(), input.end(), 'e', ETX);
	replace(input.begin(), input.end(), 's', STX);
	PrintFrame(input);
	cout << endl;

	//Trasmit
	string transmit = ConvertToFrameAndStuff(input);
	cout << "Transmitting: " << endl;
	PrintFrame(transmit);
	cout << endl;

	//Receive
	string receive = ConvertFromFrameAndDestuff(transmit);
	cout << "Received frame: " << endl;
	PrintFrame(receive);
	cout << endl;
}

string ConvertToFrameAndStuff(string input)
{
	for (string::iterator it = input.begin(); it != input.end(); ++it)
	{
		//Stuffing DLE's
		if (*it == DLE)
		{
			input.insert(it, DLE);
			it++;
		}
	}

	//Inserting start of frame
	input.insert(0, 1, DLE);
	input.insert(1, 1, STX);

	//Inserting end of frame
	input.insert(input.end(), 1, DLE);
	input.insert(input.end(), 1, ETX);
	return input;
}

string ConvertFromFrameAndDestuff(string frame)
{
	const string errorMsg = "Frame not valid";

	//Validating length
	if (frame.length() < 5) return errorMsg;

	//Checking start of frame
	if (frame[0] != DLE || frame[1] != STX) return errorMsg;

	string::iterator it = frame.begin() + 2; //Start iterating from after frame start characters

	while (!(*(it) == DLE && *(it + 1) == ETX)) //Continue until we detect end of frame and ignoring sequence DLE DLE ETX
	{
		//If we encounter the end of the string after two chars, a correct end of frame (DLE + ETX) was not detected.
		if (it + 2 == frame.end()) return errorMsg; 

		//De-stuff
		if (*it == DLE && *(it + 1) == DLE)
			frame.erase(it);
		
		it++;
	}

	//Removing start characters
	frame.erase(frame.begin());
	frame.erase(frame.begin());
	//Removing end characters
	frame.erase(frame.end() - 1);
	frame.erase(frame.end() - 1);

	return frame;
}

void PrintFrame(string frame)
{
	for (string::iterator it = frame.begin(); it != frame.end(); ++it)
	{
		switch (*it)
		{
		case DLE:
			cout << "<DLE>";
			break;
		case STX:
			cout << "<STX>";
			break;
		case ETX:
			cout << "<ETX>";
			break;
		default:
			cout << *it;
			break;
		}
	}
}
