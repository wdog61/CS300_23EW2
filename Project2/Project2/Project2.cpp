// Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <string> // atoi
#include <time.h>
#include <vector>
#include <ctype.h>

using namespace std;

/// <summary>
/// Defines a model of a course listing
/// </summary>
struct Course {

	//The id of the course
	string CourseNumber;

	//A string representing the purpose of the course
	string CourseDescription;

	//A collection of course numbers that need to be present in order to take the assigned course
	vector<string> CoursePrerequisites;

	//default constructor
	Course() {}

	//initializes a course with the provided tokens
	Course(vector<string> tokens)
	{
		CourseNumber = tokens[0];
		CourseDescription = tokens[1];

		//if the length is greater then 2 then you have prerequisites
		if (tokens.size() > 2)
		{
			//loop through remaining prerequesisites
			for (int i = 2; i < tokens.size(); i++)
			{
				CoursePrerequisites.push_back(tokens[i]);
			}
		}
	}

	~Course()
	{

	}

	/// <summary>
	/// Generates a useful description of the course to the user.
	/// </summary>
	void Print()
	{
		cout << CourseNumber << ", " << CourseDescription;

		for (int i = 0; i < CoursePrerequisites.size(); i++)
		{
			cout << ", " << CoursePrerequisites[i];
		}

		cout << endl;
	}
};

/// <summary>
/// An object to store a course and tokens that will be parsed
/// </summary>
struct CourseToBeParsed
{
	//The id of the course
	string CourseNumber;

	//The tokens to be parsed
	vector<string> Tokens;

	//The course has prereqs that needs to be checked
	bool CourseHasPreRequisites;

	//Default constructor
	CourseToBeParsed() {}

	/// <summary>
	/// Creates an instance of the object
	/// </summary>
	/// <param name="tokens">The tokens that will be parsed</param>
	CourseToBeParsed(vector<string> tokens) 
	{
		CourseNumber = tokens[0];
		Tokens = tokens;
		CourseHasPreRequisites = tokens.size() > 2;
	}

	/// <summary>
	/// Returns any prerequsite courses
	/// </summary>
	/// <returns>a vector of strings that represent prerequsistes.  Vector will be empty if token length == 2</returns>
	vector<string> GetPrerequisites()
	{	
		vector<string> courses = vector<string>();
		if (Tokens.size() > 2)
		{
			for (int i = 2; i < Tokens.size(); i++)
				courses.push_back(Tokens[i]);
		}

		return courses;
	}
};

/// <summary>
/// A data structure that will act as our data storage
/// </summary>
class HashTable {

private:
	/// <summary>
	/// An internal representation of a bucket in the hashtable
	/// </summary>
	struct Bucket {

		/// <summary>
		/// Represents any course that falls under the assigned key
		/// </summary>
		vector<Course> courses;

		/// <summary>
		/// The key value of the bucket
		/// </summary>
		unsigned int key;


		// default constructor
		Bucket() {
			//Indicates bucket is empty
			key = UINT_MAX;
		}

	};

	/// <summary>
	/// The current size of the HashTable
	/// </summary>
	unsigned int tableSize = 10;

	/// <summary>
	/// a series of keys that map to associated buckets
	/// </summary>
	vector<Bucket> buckets;


public:



	/// <summary>
	/// Default constructor generates an empty hashTable
	/// </summary>
	HashTable();

	/// <summary>
	/// Creates a hash table with a collection of the provided size
	/// </summary>
	/// <param name="size">The number of buckets to model</param>
	HashTable(int size);

	/// <summary>
	/// Default destructor
	/// </summary>
	~HashTable();

	/// <summary>
	/// Inserts a course into the appropriate bucket
	/// </summary>
	/// <param name="course">The course to be added to the system</param>
	void Insert(Course course);

	/// <summary>
	/// Prints all courses in the collection in order (smallest course number to largest)
	/// </summary>
	void PrintAll();

	/// <summary>
	/// Prints the given course
	/// </summary>
	/// <param name="course">The name of the desired course</param>
	void PrintCourse(string course);

	/// <summary>
	/// Retreives a course object associated with the course id given
	/// </summary>
	/// <param name="course">The name of the desired course</param>
	/// <returns>null if not found, otherwise an object representing the course</returns>
	Course FindCourse(string course);

	/// <summary>
	/// Generates an int based off of the given key value
	/// </summary>
	/// <param name="key">An id you wish to hash</param>
	/// <returns>A hashed representation of the key</returns>
	int createHash(string key);
};

/// <summary>
/// Default constructor
/// </summary>
HashTable::HashTable() {

	buckets = vector<Bucket>(tableSize);
}

/// <summary>
/// Creates a hash table with a collection of the provided size
/// </summary>
/// <param name="size">The number of buckets to model</param>
HashTable::HashTable(int size)
{
	tableSize = size;
	buckets = vector<Bucket>(tableSize);
}

/// <summary>
/// Default Destructor
/// </summary>
HashTable::~HashTable() {
	buckets.clear();
}

/// <summary>
/// Create a hash for the supplied string
/// </summary>
/// <param name="key">The string to hash</param>
/// <returns>an int that is a hash</returns>
int HashTable::createHash(string key) {
	//This will go through and perform a hash based on the desired size of the table.

	int hash = 0;

	//Iterate through the string and get the int value of each char
	for (int i = 0; i < key.size(); i++)
	{
		hash = hash + key[i];
	}

	//Hash the sum of the values obtained above
	return hash % tableSize;
}

/// <summary>
/// Inserts a course into the appropriate bucket
/// </summary>
/// <param name="course">The course to be added to the system</param>
void HashTable::Insert(Course course)
{
	int hash = createHash(course.CourseNumber);

	buckets[hash].courses.push_back(course);

}

/// <summary>
/// Prints all courses in the collection in alphabetical order
/// </summary>
void HashTable::PrintAll()
{
	vector<Course> sortedCourses = vector<Course>();

	//iterate through all the buckets in the collection
	for (int bucket = 0; bucket < buckets.size(); bucket++)
	{
		//iterate through all of the courses in the bucket.
		for (int i = 0; i < buckets[bucket].courses.size(); i++)
		{
			//if no courses have been added start
			if (sortedCourses.empty())
			{
				sortedCourses.push_back(buckets[bucket].courses[i]);
			}
			else
			{
				bool _coursePlaced = false;
				//iterate through the sorted courses to see where we need to insert
				for (int sc = 0; sc < sortedCourses.size(); sc++)
				{
					//look for the course that is greater then our course of interest
					if (sortedCourses[sc].CourseNumber > buckets[bucket].courses[i].CourseNumber)
					{
						//insert in front.
						sortedCourses.insert(sortedCourses.begin() + sc, buckets[bucket].courses[i]);
						//exit sourted courses loop
						_coursePlaced = true;
					}

					if (_coursePlaced)
						break;
				}

				//if we didnt place in the middle we will add it to the back
				if(!_coursePlaced)
					sortedCourses.push_back(buckets[bucket].courses[i]);
			}
		}
	}

	//iterate through and print the courses
	for (int i = 0; i < sortedCourses.size(); i++)
		sortedCourses[i].Print();
}

/// <summary>
/// Retreives a course object associated with the course id given
/// </summary>
/// <param name="course">The name of the desired course</param>
/// <returns>a blank course object if not found, otherwise an object representing the course</returns>
Course HashTable::FindCourse(string course)
{

	//iterate through all the buckets in the collection
	for (int bucket = 0; bucket < buckets.size(); bucket++)
	{
		//iterate through all of the courses in the bucket.
		for (int i = 0; i < buckets[bucket].courses.size(); i++)
		{
			//if the course matches print it and return;
			if (buckets[bucket].courses[i].CourseNumber == course)
			{
				return buckets[bucket].courses[i];			
			}
		}
	}

	return Course();

}

/// <summary>
/// Prints the given course
/// </summary>
/// <param name="course">The name of the desired course</param>
void HashTable::PrintCourse(string course)
{
	Course courseOfInterest = FindCourse(course);
	
	if (courseOfInterest.CourseNumber.empty())
	{
		//If you reached here you didnt find the course.  Let the user know.

		cout << "The course " << course << " could not be found." << endl;
		system("pause"); // waits for user to press enter

		return;
	}

	//course has been found display description
	cout << "Course ID: " << courseOfInterest.CourseNumber << endl;
	cout << "Course Description : " << courseOfInterest.CourseDescription << endl;

	//check to see if there are any prereqs required
	if (courseOfInterest.CoursePrerequisites.size() > 0)
	{
		//if so output them
		cout << "Course Prerequisites: " << endl;
		for (int i = 0; i < courseOfInterest.CoursePrerequisites.size(); i++)
		{
			Course prereq = FindCourse(courseOfInterest.CoursePrerequisites[i]);
			
			//belt and suspenders used here incase the prereq isnt found.
			if (prereq.CourseNumber.length() > 0)
			{
				cout << "     ID : " << prereq.CourseNumber << ", Description: " << prereq.CourseDescription << endl;
			}
		}
	}

	system("pause"); // waits for user to press enter

}

/// <summary>
/// Our storage instance
/// </summary>
static HashTable table;

/// <summary>
/// Splits the provided input into multiple strings by comma
/// </summary>
/// <param name="_input">The string you wish to split</param>
/// <returns>A vector of strings.  If the input doesnt contain a comma the vector will be length 1 with the original string</returns>
vector<string> SplitStringByComma(string _input)
{
	vector<string> splits = vector<string>();

	string _buffer = "";

	//iterate through the input string
	for (int i = 0; i < _input.length(); i++)
	{
		//if the token isnt a comma add it to the buffer
		if (_input[i] != ',')
		{
			//screen out invalid charectors
			if (isalpha((unsigned char)_input[i]) || isdigit((unsigned char)_input[i]) || _input[i] == ' ')
			{
				_buffer = _buffer + _input[i];
			}
		}
		else
		{
			//take whats in the buffer, add it to the stack and clear the buffer
			splits.push_back(_buffer);
			_buffer = "";
		}
	}

	//add whatevers left in the buffer to the stack
	if (_buffer != "")
		splits.push_back(_buffer);

	//return;
	return splits;
}

/// <summary>
/// Loads the csv provided by the user.
/// </summary>
bool LoadFile()
{
	//clears out the table in the event somebody decides to run this twice.
	table = HashTable();
	string _fileName = "";
	string _lineBuffer = "";
	vector<CourseToBeParsed> _csvRows = vector<CourseToBeParsed>();
	system("CLS"); //clears the console.
	cout << "Please type in the file you wish to use for course data." << endl;
	cin >> _fileName;

	cout << "Loading csv file " << _fileName << endl;

	//create file stream.
	std::ifstream fileStream(_fileName.c_str());

	//open file stream
	if (fileStream.is_open())
	{
		//loop through the file.
		while (fileStream.good())
		{
			//read the next line in the file
			getline(fileStream, _lineBuffer);

			//if the file is not empty add it to the rows to be parsed
			if (_lineBuffer != "")
			{
				//Split the row into tokens.
				vector<string> splits = SplitStringByComma(_lineBuffer);

				//There are more then one token.  Row is valid for now.  Add it.  If not its not valid and wont be added.
				if(splits.size() > 1)
					_csvRows.push_back(CourseToBeParsed(splits));
			}
		}
		
		//close the file when done.
		fileStream.close();

	}
	else //problem with file.
	{
	
		cout << "File did not open.  Please try again." << endl;
		system("pause"); // waits for user to press enter
		return false;
	}


	//if the file was empty, let the user know and bail out.
	if (_csvRows.size() == 0)
	{
		cout << "The provided file is empty" << endl;
		system("pause"); // waits for user to press enter
		return false;
	}

	//iterate through the courses
	for (int i = 0; i < _csvRows.size(); i++)
	{
		//if the course doesnt have prereqs add it to the hash.
		if (!_csvRows[i].CourseHasPreRequisites)
		{
			table.Insert(Course(_csvRows[i].Tokens));
		}
		else
		{
			vector<string> _preReqs = _csvRows[i].GetPrerequisites();
			vector<string> _preReqsFound = vector<string>();

			//iterate through your prereqs to make sure they exist
			for (int p = 0; p < _preReqs.size(); p++)
			{	
				string _preReq = _preReqs[p];
				for (int j = 0; j < _csvRows.size(); j++)
				{
					//if the prereq is found in the file add it to the list of prereqs found
					if (_csvRows[j].CourseNumber == _preReq)
					{ 
						_preReqsFound.push_back(_preReq);
						break;//break because we dont need to continue for this prereq
					}
				}
			}

			//if we found all the prereqs add the course
			if (_preReqs.size() == _preReqsFound.size())
			{
				table.Insert(Course(_csvRows[i].Tokens));
			}
		}
	}


	return true;
}

/// <summary>
/// Prompts the user for what course they are looking for and prints it.
/// </summary>
void FindAndPrintSelected()
{
	string courseOfInterest = "";
	system("CLS"); //clears the console.

	//get course of interest for user
	cout << "Please type in the course you wish to see" << endl;

	cin >> courseOfInterest;

	table.PrintCourse(courseOfInterest);
}

/// <summary>
/// Executes the main work loop of the program.
/// </summary>
void DoWork()
{
	bool _dataWasLoaded = false;

	//Program will keep performing this loop untill the user exits.
	while (true)
	{
		
		int choice = 0;

		//display menu
		cout << "Menu:" << endl;
		cout << "  1. Load Courses" << endl;
		cout << "  2. Display All Courses" << endl;
		cout << "  3. Find Course" << endl;
		cout << "  9. Exit" << endl;
		cout << "Enter choice: ";

		//capture user menu choice
		cin >> choice;

		//make sure that if user wants to do something, we hav the data for it.
		if (!_dataWasLoaded &&
			choice != 9 &&
			choice != 1)
		{
			cout << "Please load data before displaying anything." << endl;
			system("pause"); // waits for user to press enter
		}
		else
		{
			//Valid menu choice.  We continue.
			switch (choice)
			{

				case 1: //Load data					
					_dataWasLoaded = LoadFile();
					break;
				case 2: //Display all courses
					table.PrintAll();
					break;

				case 3: //Display selected course
					FindAndPrintSelected();
					break;

				case 9: return; //App exit.
			}
		}
	}
}



int main()
{
	table = HashTable();

	DoWork();

	std::cout << "Thanks!!" << endl;
}