#include "DiscordServerStats.h"

DiscordServerStats::DiscordServerStats()
{
	currentChannel = "no Channel";
}

void DiscordServerStats::filterYear(int inYear)
{
}

void DiscordServerStats::printResults()
{
}

void DiscordServerStats::addMessage(std::string inLine)
{

	/*Changes to make:
	* We need to take in each HTML tag / line, and parse acordingly. 
	* basically, we need to make a destructive parser that returns the current element. 
	* We can have a big div counter, that counts our div scope. 
	* and as we come across all tags, we can mark what they are for. 
	* we can create a post according to what we find in the tags. 
	*/
	postType_t currentPost = text;
	std::string messageUserName;
	std::string messageDateTime;
	std::string messageContent;
	std::string currentReaction;
	int currentReactionCount;

	while (inLine.size() > 0)
	{
		size_t stringSize = inLine.size();
		std::string currentTag = getHtmlTag(inLine);
		size_t tagSize = currentTag.size();
		inLine = inLine.substr(tagSize, inLine.size() - tagSize);

		tagType currentTagType = processTag(currentTag);

		int j;
		switch (currentTagType) 
		{
		case usernameTag:
			messageUserName = getHtmlTag(inLine);
			tagSize = messageUserName.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);
			break;

		case dateTimeTag:
		{
			std::string skipTag = getHtmlTag(inLine); //we expect this to be an <a> tag that we do not care about
			tagSize = skipTag.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);
			messageDateTime = getHtmlTag(inLine);
			tagSize = messageDateTime.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);
		}
			break;

		case messageTextTag:
			break;
			//img tag could be avatar image. We want to ignore those and not add them to the message. 
		case messageImageTag:
			break;

		case messageReactionTag:
			break;

		case messageReactionCountTag:
			break;

		case unknownTag:
			if (currentTag == " ")
			{
				//we can ignore spaces, they're just in here sometimes. Not sure why. we don't care about white space between tags.
				break;
			}
			std::cout << "unknown tag encountered" << std::endl;
			return;
			break;

		case ignoreTag:
			break;

		default:
			break;

		}

	}

	DiscordMessage* insertMessage = new DiscordMessage(messageUserName, messageDateTime, messageContent, currentChannel, currentPost);

	/*

	size_t usernameLocationTag = inLine.find("<span class=chatlog__author");
	if (usernameLocationTag == -1) 
	{
		std::cout << "Not a discord message" << std::endl;
		return;
	}

	size_t userNameLocationStart = inLine.find(">", usernameLocationTag) +1;
	size_t userNameLocationEnd = inLine.find("<", userNameLocationStart);
	std::string userName = inLine.substr(userNameLocationStart, userNameLocationEnd - userNameLocationStart);

	size_t dateTimeTag = inLine.find("<span class=chatlog__timestamp");
	size_t dateTimeStart = inLine.find("title=\"", dateTimeTag)+7;
	size_t dateTimeEnd = inLine.find("\">", dateTimeStart);
	std::string dateTime = inLine.substr(dateTimeStart, dateTimeEnd - dateTimeStart);


	std::string postContent;
	size_t postContentTag = inLine.find("<span class=chatlog__markdown-preserve");
	if (postContentTag == -1) 
	{
		//then we just look for images instead, this is probably an image post
		postContentTag = inLine.find("<div class=chatlog__attachment");
		if (postContentTag == -1) 
		{
			//We're deep in the iffs, but I'm just bug hunting at this point
			size_t postGifTag = inLine.find("class=chatlog__embed");
			if (postGifTag == -1) 
			{
				std::cout << "Not a discord message" << std::endl;
				std::cout << userName << std::endl;
				std::cout << dateTime << std::endl;
				return;

			}
			else 
			{
				size_t postContentStart = inLine.find("<source src=", postGifTag);
				size_t postContentEnd = inLine.find("alt", postContentStart);
				std::string postContent = inLine.substr(postContentStart+12, postContentEnd-1-(postContentStart+12));
				currentPost = image;
			}

			
		}
		else 
		{
			size_t postContentStart = inLine.find(">", postContentTag + 1);
			size_t postContentEnd = inLine.find("</div>");
			currentPost = image;
		}
	}
	else 
	{
		size_t postContentStart = inLine.find(">", postContentTag) + 1;
		size_t postContentEnd = inLine.find("</span>", postContentStart);

		postContent = inLine.substr(postContentStart, postContentEnd - postContentStart);
	}


	//then we need to decide post type based on post Content
	if (postContent.find("https") != -1 && currentPost == text)
	{
		currentPost = link;
	}


	DiscordMessage* insertMessage = new DiscordMessage(userName, dateTime, postContent, currentChannel, currentPost);

	int postReactionTagStart = inLine.find("class=chatlog__reaction");
	if (postReactionTagStart != -1)
	{

		size_t postReactionTag = 0; 
		postReactionTag = inLine.find("title", postReactionTagStart + 1 + postReactionTag);
		while (postReactionTag != -1)
		{
			
			//for each reaction, we need a name, and a count. 
			size_t postReactionStart = inLine.find("=", postReactionTag);
			size_t postReactionEnd = inLine.find(">", postReactionStart);
			std::string reactionToAdd = inLine.substr(postReactionStart+1, postReactionEnd - (postReactionStart+1));

			//then we need to find count

			size_t postCountTag = inLine.find("count", postReactionEnd);
			size_t postCountStart = inLine.find(">", postCountTag);
			size_t postCountEnd = inLine.find("<");
			std::string postCountString = inLine.substr(postCountStart + 1, postCountEnd - (postCountStart + 1));

			int postCountNum = stoi(postCountString);

			for (int i = 0; i<postCountNum; i++) 
			{
				//add reaction to this post
				insertMessage->addReactions(reactionToAdd);
			}

			postReactionTag = inLine.find("title", postReactionTagStart + 1 + postReactionTag);

		}

	}

	allMessages.push_back(insertMessage);

	*/



}

void DiscordServerStats::changeChannel(std::string inChannel)
{
	currentChannel = inChannel;
}

std::string DiscordServerStats::getHtmlTag(std::string inHTMLBlock)
{
	std::string returnString;

	size_t firstTagIndex = inHTMLBlock.find("<");
	if (firstTagIndex != 0)
	{
		//this is where we have a text element in our HTML Tag
		returnString = inHTMLBlock.substr(0, firstTagIndex);
	}
	else 
	{
		size_t lastTagIndex = inHTMLBlock.find(">");
		returnString = inHTMLBlock.substr(firstTagIndex, lastTagIndex - firstTagIndex + 1);
	}

	return returnString;
}

tagType DiscordServerStats::processTag(std::string inHTMLTag)
{
	/*Tags that we should stack monitor:
	* Div
	* A
	* Span
	* 
	* Tags that do not need to be monitored:
	* img
	* 
	*/
	tagType returnTagType = unknownTag;
	std::string className;
	//Div section handeled. 
	if (inHTMLTag.find("<div") != -1) 
	{
		returnTagType = ignoreTag;
		tagStack.push_back(inHTMLTag);

		//now we need to find the class, and base what we do off of that. 

		size_t classStart = inHTMLTag.find("class=");
		size_t classEnd = inHTMLTag.find(" ", classStart);
		size_t classEnd2 = inHTMLTag.find(">", classStart);
		
		if (classEnd != -1) 
		{
			className = inHTMLTag.substr(classStart + 6, classEnd - (classStart + 6));
		}
		else if (classEnd2 != -1) 
		{
			className = inHTMLTag.substr(classStart + 6, classEnd2 - (classStart + 6));
		}
		else 
		{
			std::cout << "div with no class, seems wrong" << std::endl;
			return returnTagType;
		}

		if (className == "chatlog__reaction") 
		{
			returnTagType = messageReactionTag;
			return returnTagType;
		}
		else if (className == "")
		{

		}

	}

	if (inHTMLTag.find("</div>") != -1 && inHTMLTag.size() == 6) 
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size()-1).find("<div") != -1)
		{
			tagStack.pop_back(); //if we are currently on a div tag, we can pop that tag off of the stack. 
		}
	}


	//I do feel like I'm repeating myself a but here, but we do need to seperate out our tags. Might consolidate later during optimization, if I do that. 
	if (inHTMLTag.find("<span") != -1) 
	{
		returnTagType = ignoreTag;
		tagStack.push_back(inHTMLTag);

		size_t classStart = inHTMLTag.find("class=");
		size_t classEnd = inHTMLTag.find(" ", classStart);
		size_t classEnd2 = inHTMLTag.find(">", classStart);

		if (classEnd != -1)
		{
			className = inHTMLTag.substr(classStart + 6, classEnd - (classStart + 6));
		}
		else if (classEnd2 != -1)
		{
			className = inHTMLTag.substr(classStart + 6, classEnd2 - (classStart + 6));
		}
		else
		{
			std::cout << "span with no class, seems wrong" << std::endl;
			return returnTagType;
		}

		if (className == "chatlog__markdown - preserve")
		{
			returnTagType = messageTextTag;
			return returnTagType;
		}
		else if (className == "chatlog__author") 
		{
			returnTagType = usernameTag;
			return returnTagType;
		}
		else if (className == "chatlog__timestamp") 
		{
			returnTagType = dateTimeTag;
			return returnTagType;
		}
		else if (className == "chatlog__reaction-count")
		{
			returnTagType = messageReactionCountTag;
			returnTagType;
		}

	}

	if (inHTMLTag.find("</span>") != -1 && inHTMLTag.size() == 7)
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<span") != -1)
		{
			tagStack.pop_back(); //if we are currently on a span tag, we can pop that tag off of the stack. 
		}
	}

	if (inHTMLTag.find("<a") != -1) 
	{
		returnTagType = ignoreTag;
		//Atags do not have a class
		tagStack.push_back(inHTMLTag);
	}
	if (inHTMLTag.find("</a>") != -1 && inHTMLTag.size() == 4)
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<a") != -1)
		{
			tagStack.pop_back(); //if we are currently on an a tag, we can pop that tag off of the stack. 
		}
	}

	if (inHTMLTag.find("<img") != -1)
	{
		//this is not one we have to put on our stack
		returnTagType = messageImageTag;
		return returnTagType;
	}



	return returnTagType;
}
