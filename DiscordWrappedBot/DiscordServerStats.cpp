#include "DiscordServerStats.h"

DiscordServerStats::DiscordServerStats()
{
	newLineFlag = false;
	currentChannel = "no Channel";
}

void DiscordServerStats::filterYear(int inYear)
{
}

void DiscordServerStats::findTopTenPosters()
{
}

void DiscordServerStats::findtop5HCMusicPosters()
{
}

void DiscordServerStats::findtop5MusicPosters()
{
}

void DiscordServerStats::findpromoterWithMostFlyers()
{
}

void DiscordServerStats::findtop3ReactedMessages()
{
}

void DiscordServerStats::findtop3MemeLords()
{
}

void DiscordServerStats::findtop3Reactions()
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





}

void DiscordServerStats::changeChannel(std::string inChannel)
{
	currentChannel = inChannel;
}
