#include <sstream>
#include <iostream>
#include <limits>
#include <vector>
#include <fstream>
using namespace std;


//////////////////////////////////////////////////////////////////////////////
// Class DoubleCSVFile reads lines of comma-separated floating-point variables
// from an input stream and stores the data it has read.

class DoubleCSVFile
{
public:
	// Load doubles from an input stream. Returns 0 on success, 1 on error
	int Load(istream &inputStream)
	{
		string line;
		while (getline(inputStream, line)) // Get a line of data from the file and store in 'line'
		{
			vector<double> lineData;

			// processes the line just read; if processing fails, return with an error
			if (ReadLineOfDoubles(line, lineData)) return 1;
			else fileData.push_back(lineData); // otherwise, add line to stored data
		}
		return 0;
	}

	// Display the stored data to cout
	void DisplayValues()
	{
		unsigned nLines = fileData.size();
		cout << "File has " << fileData.size() << " line(s):" << endl;
		for (unsigned line = 0; line < nLines; line++)
		{
			vector<double> &lineRef = fileData[line];
			cout << "  Line " << line << " has " << lineRef.size() << " item(s):" << endl;
			for (vector<double>::iterator i = lineRef.begin(); i!=lineRef.end(); i++)
				cout << "    " << *i << endl;
		}
	}

protected:
	// Read a line of comma-separated doubles from a string, and store doubles in a vector
	// This method makes an istringstream out of the line that is passed in, and uses the 
	// extraction operator >> on this new input stream to read double variables from it
	int ReadLineOfDoubles(const string &line, vector<double> &lineData)
	{
		istringstream iss(line); // make a new stream just containing the data for this line
		double d;
		char c;

		while (true)
		{
			if (!(iss >> d)) return 1; // if we can't read a double, error
			lineData.push_back(d);

			if (iss >> c) // if there is a character to read after the number...
			{
				// it must be a comma, otherwise there is an error
				if (c != ',') return 1;
			}
			else return 0;// the number last read was the last thing on the line
		}
	}

	// Internal data storage:
	// fileData[i][j] contains the jth number stored on the ith line of the file
	vector<vector<double> > fileData;
};



/////////////////////////////////////////////////////////////
// Class StringCSVFile reads lines of comma-separated strings
// from an input stream and stores the data it has read.

class StringCSVFile
{
public:
	// Load comma-separated strings from an input stream. 
	int Load(istream &inputStream)
	{	
		string line;
		while (getline(inputStream, line)) // Get a line of data and store it in 'line'
		{
			vector<string> stringList; // Temporary list of strings in this line
			ReadLineOfStrings(line, stringList); // Read line into storage
			fileData.push_back(stringList); // store data for this line in data for the file
		}	
	}

	// Display the stored data to cout
	void DisplayValues()
	{
		unsigned nLines = fileData.size();
		cout << "File has " << fileData.size() << " line(s):" << endl;
		for (unsigned line = 0; line < nLines; line++)
		{
			vector<string> &lineRef = fileData[line];
			cout << "  Line " << line << " has " << lineRef.size() << " item(s):" << endl;
			for (vector<string>::iterator i = lineRef.begin(); i!=lineRef.end(); i++)
				cout << "    \"" << *i << "\"" << endl;
		}
	}

protected:
	// Read a line of comma-separated strings from a string, and add the data to a vector
	// This method uses methods of the std::string class (find_first_of, substr, etc.) to
	// split the line into comma-separated parts, and to trim these of whitespace
	int ReadLineOfStrings(const string &line, vector<string> &lineData)
	{
		size_t startIndex = 0, delimiterIndex;

		// find the first comma in the line, starting the search at position 'startIndex'
		while ((delimiterIndex = line.find_first_of(',', startIndex)) != string::npos)
		{
			// store the section of the line string between the current location startIndex,
			// and the comma that we have just found
			lineData.push_back(line.substr(startIndex, delimiterIndex-startIndex));

			// Next time, start searching at the character after the comma we found this time
			startIndex = delimiterIndex+1;
		}
		// loop has exited, so there are no more commas to find. Store the last item on the line
		lineData.push_back(line.substr(startIndex, string::npos));


		// Any whitespace surrounding the commas will have been included the strings we have stored
		// We now "trim" the stored strings to remove space and tab ("\t") characters from the ends

		// For each string that has been read in...
		for (vector<string>::iterator i = lineData.begin(); i != lineData.end(); i++)
		{
			size_t index;
				
			index= i->find_last_not_of(" \t"); // Find the last non-whitepace character...
			if (index != string::npos) *i = i->substr(0, index+1); // ...and cut off the characters after it.

			index = i->find_first_not_of(" \t"); // Find the first non-white-space character...
			if (index != string::npos) *i = i->substr(index); // ...and cut off the characters before it.
			else *i = string(); // If there are no non-whitespace characters, return an empty string
		}
	}

	// Internal data storage:
	// fileData[i][j] contains the jth string stored on the ith line of the file
	vector<vector<string> > fileData;
};



int main()
{
	{
		// Simple reading of whitespace-separated integers and doubles using input streams
		istringstream inputStream("123 456 789 526.234\n10 20 30 40.5");
		int a, b, c;
		double d;
		while (inputStream >> a >> b >> c >> d)
		{
			cout << a << ", " << b << ", " << c << ", " << d << endl;
		}
	}

	{
		// Simple reading of strings
		// (strings are separated by whitespace so cannot include spaces themselves)
		istringstream inputStream("time pressure x-velocity y-velocity");
		string a, b, c, d;
		while (inputStream >> a >> b >> c >> d)
		{
			cout << a << ", " << b << ", " << c << ", " << d << endl;
		}
	}


	{
		// Simple line-by-line parsing

		istringstream inputStream("This is the first line\nthis is the second\nand the third");
		string line;
		int lineCount = 0;

        // Get a line of data from the file and store in 'line'.
		// The carriage return at the end of the line is not stored in 'line'
		// getline returns false (ending the loop) if no more lines can be read
		while (getline(inputStream, line)) 
		{
			cout << "Line " << lineCount << ": " << line << endl;
			lineCount++;
		}
	}

	istringstream streamDoubles("123, 456, 789, 526.234\n10, 20, 30, 4.05e1\n3.14159\n");
	DoubleCSVFile dcsvf;
	if (dcsvf.Load(streamDoubles))
		cout << "Could not loat DoubleCSVFile from input stream" << endl;
	else dcsvf.DisplayValues();


	istringstream streamStrings("samples,\t mean, standard deviation , chi squared\n, ,test,");
	StringCSVFile scsvf;
	scsvf.Load(streamStrings);
	scsvf.DisplayValues();

/*
	// We can just as easily load from standard input as from a string stream
	StringCSVFile scsvfCin;
	scsvfCin.Load(cin);
	scsvfCin.DisplayValues();

	// Or we can load from a file
	ifstream ifs("file_loading.cpp");
	if (ifs.is_open())
	{
		StringCSVFile scsvfFile;
		scsvfFile.Load(ifs);
		scsvfFile.DisplayValues();
		ifs.close();
	}
*/

	// atoi, atof (in cstdlib, these work on const char * and convert to int / double; stoi and stod are C++11 equivalents in string that work on std::strings)

    // see http://gehrcke.de/2011/06/reading-files-in-c-using-ifstream-dealing-correctly-with-badbit-failbit-eofbit-and-perror/
	return 0;
}
