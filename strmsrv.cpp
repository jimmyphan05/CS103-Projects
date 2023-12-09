#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;

// To do - Complete this function
StreamService::StreamService() :
  cUser_(NULL) // if no user logged in, user pointer is null
{
    // Update as needed

}

// To do - Complete this function
StreamService::~StreamService()
{
  // deallocating memory from content and users vector
  for (unsigned int i = 0; i < content_.size(); i++)
  {
    delete content_[i];
  }
    for (unsigned int i = 0; i < users_.size(); i++)
  {
    delete users_[i];
  }
  return;
}

// Complete - Do not alter
void StreamService::readAndParseData(std::istream& is, Parser& p)
{
    p.parse(is, content_, users_);
    cout << "Read " << content_.size() << " content items." << endl;
    cout << "Read " << users_.size() << " users." << endl;
}

// To do - Complete this function
void StreamService::userLogin(const std::string& uname)
{
  if (cUser_ != NULL)
  {
    throw runtime_error("Current user should logout first!");
  }
  if (getUserIndexByName(uname) == -1)
  {
    throw invalid_argument("Username not valid!");
  }

  for (unsigned int i = 0; i < users_.size(); i++)
  {
    if (uname == users_[i]->uname)
    {
      cUser_ = users_[i];
      return;
    }
  }
}

// To do - Complete this function
void StreamService::userLogout()
{
  if (cUser_ == NULL)
  {
    return;
  }
  cUser_ = NULL;
}

// To do - Complete this function
std::vector<CID_T> StreamService::searchContent(const std::string& partial) const
{
    std::vector<CID_T> results;
    for(size_t i = 0; i < content_.size(); i++){
        // TO DO - modify this to also push back when the string 'partial'
        //  is contained in the name of the current content. Lookup the string
        //  class documentation to find an appropriate function that can be used
        //  to do this simply.
        if(partial == "*" || content_[i]->name().find(partial) != std::string::npos){
            results.push_back(content_[i]->id());
        }        
    }
    return results;
}

// Complete - Do not alter
std::vector<CID_T> StreamService::getUserHistory() const
{
    throwIfNoCurrentUser();
    return cUser_->history;
}

// To do - Complete this function
void StreamService::watch(CID_T contentID)
{
  throwIfNoCurrentUser();
  if (!isValidContentID(contentID))
  {
    throw range_error("contentID not valid!");
  }
  for (unsigned int i = 0; i < content_.size(); i++)
  {
    if (contentID == content_[i]->id())
    {
      if (content_[i]->rating() > cUser_->ratingLimit)
      {
        throw RatingLimitError("User's rating is too low!");
      }
      cUser_->addToHistory(contentID);
      content_[i]->addViewer(cUser_->uname);
      return;
    }
  }
}

// To do - Complete this function
void StreamService::reviewShow(CID_T contentID, int numStars)
{
  throwIfNoCurrentUser();
  if (!isValidContentID(contentID) || numStars < 0 || numStars > 5)
  {
    throw ReviewRangeError("contentID invalid or number of stars outside 0-5");
  }

  for (unsigned int i = 0; i < content_.size(); i++)
  {
    if (contentID == content_[i]->id())
    {
      content_[i]->review(numStars);
      return;
    }
  }
}

// To do - Complete this function
CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
    // Change this when you are ready, but for now it is a 
    // dummy implementation
    throwIfNoCurrentUser();
    if (!isValidContentID(contentID))
    {
      throw range_error("contentID not valid!");
    }

    // adding users who have watched given CID to a vector
    vector<User*> usersWhoWatched;
    for (unsigned int i = 0; i < users_.size(); i++)
    {
      if (users_[i]->haveWatched(contentID))
      {
        usersWhoWatched.push_back(users_[i]);
      }
    }

    // checking the viewing history of users who watched given CID
    // and seeing if the current user has watched it
    vector<CID_T> cUserHasntWatched;
    for (unsigned int i = 0; i < usersWhoWatched.size(); i++)
    {
      for (unsigned int j = 0; j < usersWhoWatched[i]->history.size(); j++)
      {
        if (!cUser_->haveWatched(usersWhoWatched[i]->history[j]))
        {
          cUserHasntWatched.push_back(usersWhoWatched[i]->history[j]);
        }
      }
    }

    if (cUserHasntWatched.empty())
    {
      return -1;
    }

    // going through the list to see which content was viewed most
    int indexWithHighest;
    int highestCount = 0;
    for (unsigned int i = 0; i < cUserHasntWatched.size(); i++)
    {
      int count = 0;
      for (unsigned int j = 0; j < cUserHasntWatched.size(); j++)
      {
        if (cUserHasntWatched[i] == cUserHasntWatched[j])
        {
          count++;
        }
      }
      if (count > highestCount)
      {
        indexWithHighest = i;
      }
    }

    
    return cUserHasntWatched[indexWithHighest];

}

// To do - Complete this function
void StreamService::displayContentInfo(CID_T contentID) const
{
    // Do not alter this
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }

    // Call the display abitlity of the appropriate content object
    for (unsigned int i = 0; i < content_.size(); i++)
    {
      if (contentID == content_[i]->id())
      {
        content_[i]->display(cout);
        return;
      }
    }
}

// Complete - Do not alter
bool StreamService::isValidContentID(CID_T contentID) const
{
    return (contentID >= 0) && (contentID < (int) content_.size());
}

// Complete - Do not alter
void StreamService::throwIfNoCurrentUser() const
{
    if(cUser_ == NULL){
        throw UserNotLoggedInError("No user is logged in");
    }
}

// Complete - Do not alter
int StreamService::getUserIndexByName(const std::string& uname) const
{
    for(size_t i = 0; i < users_.size(); i++){
        if(uname == users_[i]->uname) {
            return (int)i;
        }
    }
    return -1;
}
