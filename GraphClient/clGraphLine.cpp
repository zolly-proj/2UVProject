#include "clGraphLine.h"
clGraphLine::clGraphLine()
{
}
clGraphLine::clGraphLine(QString paColor, QString paName, vector<clQuery> paQuery_X, vector<clQuery> paQuery_Y)
{
    setName(paName);
    setColor(paColor);
    setQuery_X(paQuery_X);
    setQuery_Y(paQuery_Y);	
}

clGraphLine::~clGraphLine()
{

}

//Setters
void clGraphLine::setName(QString paName){ meName= paName;}
void clGraphLine::setColor(QString paColor){ meColor = paColor;}
void clGraphLine::setQuery_X(vector<clQuery> paQuery_X){meQuery_X = paQuery_X;}
void clGraphLine::setQuery_Y(vector<clQuery> paQuery_Y){meQuery_Y = paQuery_Y;}

//Getters
QString clGraphLine::getName(){return meName;}
QString clGraphLine::getColor(){return meColor;}
vector<clQuery> clGraphLine::getQuery_X(){return meQuery_X;}
vector<clQuery> clGraphLine::getQuery_Y(){return meQuery_Y;}
