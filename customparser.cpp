// Complete me
#include "customparser.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

const char* error_msg_1 = "Cannot read integer n";
const char* error_msg_2 = "Error in content parsing";
const char* error_msg_3 = "Error in user parsing";

// To Do - Complete this function
void CustomFormatParser::parse(std::istream& is, std::vector<Content*>& content, std::vector<User*>& users)
{
    // Erase any old contents
    content.clear();
    users.clear();

    // TO DO - Add your code below.

    int contentSize;
    is >> contentSize;

    // throw if failed to read in contents size
    if (is.fail())
    {
      throw ParserError(error_msg_1);
    }
    is >> ws;

    //try, check, use
    for (unsigned int i = 0; i < contentSize; i++)
    {
      // getting the content information, using ws to ignore whitespaces
      int id, type, numReviews, totalStars, rating, numEpisodes;
      string name;  
      is >> id >> ws >> type >> ws;
      getline(is, name);
      is >> numReviews >> ws >> totalStars >> ws >> rating >> ws;

      Content* newContent;
      // if its a series, get the number of episodes also
      if (type == 1)
      {
        is >> numEpisodes >> ws;
        newContent = new Series(id, name, numReviews, totalStars, rating, numEpisodes);
      }
      else
      {
        newContent = new Movie(id, name, numReviews, totalStars, rating);
      }

      content.push_back(newContent);
      if (is.fail())
      {
        cout << "error 2" << endl;
        throw ParserError(error_msg_2);
      }

      // checking if there's 0 names in the user's who viewed content
      // if the next line is a digit, that means there were no users who viewed the content
      if (isdigit(is.peek()))
      {
        continue;
      }

      string userNames;
      getline(is, userNames);

      stringstream ss(userNames);
      while (ss >> name)
      {
        newContent->addViewer(name);
      }
    }

    string userNames;
    // continuing until there's no more names, the end of the file
    while (is >> userNames)
    {
      int ratingLimit;
      is >> ratingLimit;
      if (is.fail())
      {
        cout << "error 3" << endl;
        throw ParserError(error_msg_3);
      }

      User* newUser = new User(userNames, ratingLimit);
      is >> ws;
      users.push_back(newUser);
      // checking if there's 0 content ID viewing history 
      // if need item is a letter(start of a username), then there's no viewing history
      if (isalpha(is.peek()))
      {
        continue;
      }
      
      int CIDs;
      while (is >> CIDs)
      {
        newUser->addToHistory(CIDs);
      }
      is.clear();
      is >> ws;


    }
}