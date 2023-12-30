#include "DiscordServerStats.h"

DiscordServerStats::DiscordServerStats()
{
	currentChannel = "no Channel";
}



void DiscordServerStats::filterYear(int inYear)
{
	for (int i = 0; i<allMessages.size(); i++) 
	{
		std::string dateTime = allMessages[i]->dateTime;

		if (dateTime.find(std::to_string(inYear)) == -1)
		{
			allMessages.erase(allMessages.begin() + i);
			i = i - 1;
		}
	}

}

void DiscordServerStats::printResults()
{
	/*
	* List of things we'd like to print:
	* - Total Posts
	* - Music Posted in music channels
	* - Show Flyers Posted in flyer channels
	* - Total Memes Posted
	* - Biggest Poster -> Top 10
	* - Most HC music posted -> top 5
	* - Most music posted -> top 5
	* - Promoter posting the most flyers -> single person
	* - Most reacted Post -> Top 3
	* - Most memes posted -> Top 3
	* - Most used Emojis -> top 3
	*/

	std::cout << "Total Posts in 2023: ";
	std::cout << allMessages.size() << std::endl;

	std::unordered_map<std::string, int> topPosters;
	std::vector<std::string> topPostersUsers;
	std::vector<int> topPostersValues;
	std::unordered_map<std::string, int> topHCMusicPosters;
	std::vector<std::string> topHCMusicPostersUsers;
	std::vector<int> topHCMusicPostersValues;
	std::unordered_map<std::string, int> topMusicPosters;
	std::vector<std::string> topMusicPostersUsers;
	std::vector<int> topMusicPostersValues;
	std::unordered_map<std::string, int> mostFlyers;
	std::vector<std::string> mostFlyersUsers;
	std::vector<int> mostFlyersValues;
	std::unordered_map<std::string, int> biggestMemers;
	std::vector<std::string> biggestMemersUsers;
	std::vector<int> biggestMemersValues;
	std::unordered_map<std::string, int> mostUsedEmojis;
	std::vector<std::string> mostUsedEmojisUsers;
	std::vector<int> mostUsedEmojisValues;


	DiscordMessage* mostReactedPost;
	int postReactionNumber = 0;

	int musicPosted = 0;
	int flyersPosted = 0;
	int memesPosted = 0;
	int totalReactions = 0;


	for (int i = 0; i < allMessages.size(); i++)
	{
		//topPosters Section
		std::string userName = allMessages[i]->userName;
		if (topPosters.find(userName) == topPosters.end())
		{
			topPosters.emplace(userName, 1);
		}
		else
		{
			topPosters.at(userName) = topPosters.at(userName) + 1; // I do not trust ++ with maps. 
		}

		//Emoji section
		int totalPostReactions = 0;
		for (int j = 0; j < allMessages[i]->postReactions.size(); j++)
		{
			totalReactions++;
			totalPostReactions++;

			if (mostUsedEmojis.find(allMessages[i]->postReactions[j]) == mostUsedEmojis.end())
			{
				mostUsedEmojis.emplace(allMessages[i]->postReactions[j], 1);
			}
			else
			{
				mostUsedEmojis.at(allMessages[i]->postReactions[j]) = mostUsedEmojis.at(allMessages[i]->postReactions[j]) + 1;
			}

			if (totalPostReactions > postReactionNumber)
			{
				totalPostReactions = postReactionNumber;
				mostReactedPost = allMessages[i];
			}
		}

		//musicSection
		if ((allMessages[i]->channelPosted == "Music / everything-else" || allMessages[i]->channelPosted == "Music / hardcore-for-hardcore")
			&& allMessages[i]->postType == link)
		{
			musicPosted++;

			if (topMusicPosters.find(allMessages[i]->userName) == topMusicPosters.end())
			{
				topMusicPosters.emplace(allMessages[i]->userName, 1);
			}
			else
			{
				topMusicPosters.at(allMessages[i]->userName) = topMusicPosters.at(allMessages[i]->userName) + 1;
			}

			if (allMessages[i]->channelPosted == "Music / hardcore-for-hardcore")
			{
				if (topHCMusicPosters.find(allMessages[i]->userName) == topHCMusicPosters.end())
				{
					topHCMusicPosters.emplace(allMessages[i]->userName, 1);
				}
				else
				{
					topHCMusicPosters.at(allMessages[i]->userName) = topHCMusicPosters.at(allMessages[i]->userName) + 1;
				}


			}


		}

		//memes
		if (allMessages[i]->channelPosted == "General / memes" && allMessages[i]->postType == image)
		{
			memesPosted++;
			if (biggestMemers.find(allMessages[i]->userName) == biggestMemers.end())
			{
				biggestMemers.emplace(allMessages[i]->userName, 1);
			}
			else
			{
				biggestMemers.at(allMessages[i]->userName) = biggestMemers.at(allMessages[i]->userName) + 1;
			}

		}

		//flyers
		if (allMessages[i]->channelPosted == "Shows / show-announcements" || allMessages[i]->channelPosted == "Shows / non-hc-shows")
		{
			flyersPosted++;
			if (mostFlyers.find(allMessages[i]->userName) == mostFlyers.end())
			{
				mostFlyers.emplace(allMessages[i]->userName, 1);
			}
			else
			{
				mostFlyers.at(allMessages[i]->userName) = mostFlyers.at(allMessages[i]->userName) + 1;
			}

		}
	}



	std::cout << "Total Flyers posted in flyer channels: ";
	std::cout << flyersPosted << std::endl;

	std::cout << "Total Music posted in music channels: ";
	std::cout << musicPosted << std::endl;

	std::cout << "Total Memes posted in the meme channel: ";
	std::cout << memesPosted << std::endl;

	std::cout << "Total Serverwide Reactions: ";
	std::cout << totalReactions << std::endl;

	sortMap(&topPostersUsers, &topPostersValues, topPosters);
	sortMap(&topHCMusicPostersUsers, &topHCMusicPostersValues, topHCMusicPosters);
	sortMap(&topMusicPostersUsers, &topMusicPostersValues, topMusicPosters);
	sortMap(&mostFlyersUsers, &mostFlyersValues, mostFlyers);
	sortMap(&biggestMemersUsers, &biggestMemersValues, biggestMemers);
	sortMap(&mostUsedEmojisUsers, &mostUsedEmojisValues, mostUsedEmojis);

	std::cout << "The top 10 highest posters this year:" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << (i + 1) << ": " << topPostersUsers[i] << " with " << topPostersValues[i] << " posts!" << std::endl;
	}

	std::cout << "The top 5 highest Hardcore music posters this year:" << std::endl;
	for (int i = 0; i < 5; i++)
	{
		std::cout << (i + 1) << ": " << topHCMusicPostersUsers[i] << " with " << topHCMusicPostersValues[i] << " posts!" << std::endl;
	}

	std::cout << "The top 5 highest overall music posters this year:" << std::endl;
	for (int i = 0; i < 5; i++)
	{
		std::cout << (i + 1) << ": " << topMusicPostersUsers[i] << " with " << topMusicPostersValues[i] << " posts!" << std::endl;
	}

	std::cout << "The top 3 users who posted the most flyers (all show channels): " << std::endl;
	for (int i = 0; i < 3; i++)
	{
		std::cout << (i + 1) << ": " << mostFlyersUsers[i] << " with " << mostFlyersValues[i] << " flyers!" << std::endl;
	}

	std::cout << "The biggest pair of meme-ers:";
	std::cout << biggestMemersUsers[0] << " and " << biggestMemersUsers[1] << " , with " << biggestMemersValues[0] << " and " << biggestMemersValues[1] << " memes, respectivley." << std::endl;

	std::cout << "The top 3 most used reactions of 2023 were: " << std::endl;
	for (int i = 0; i<3; i++) 
	{
		std::cout << (i + 1) << " :" << mostUsedEmojisUsers[i] << ", reacted " << mostUsedEmojisValues[i] << " times." << std::endl;

	}




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

	DiscordMessage* insertMessage = new DiscordMessage();
	postType_t currentPost = text;
	std::string messageUserName;
	std::string messageDateTime;
	std::string messageContent = "";
	std::string currentReaction;
	int currentReactionCount;

	while (inLine.size() > 0)
	{
		size_t stringSize = inLine.size();
		std::string currentTag = getHtmlTag(inLine);
		size_t tagSize = currentTag.size();
		inLine = inLine.substr(tagSize, inLine.size() - tagSize);

		if (currentTag == " ") 
		{
			currentTag = getHtmlTag(inLine);
			tagSize = currentTag.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);
		}


		tagType currentTagType = processTag(currentTag, false);


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
		{
			std::string catString = getHtmlTag(inLine);
			if (catString == "</a>") 
			{
				//if we see an Atag, there may be a link in it that is part of a message. 
				processTag(catString, false);
				break;
			}
			else if (catString.find("<span") != -1)
			{
				//This is a mention in a message. 
				tagType possibleMention = processTag(catString, false);
				if (possibleMention == mentionTag) 
				{
					tagSize = catString.size();
					inLine = inLine.substr(tagSize, inLine.size() - tagSize);
					catString = getHtmlTag(inLine);
				}

			}
			else if (currentTag.find("chatlog__sticker") != -1)
			{
				//this is a sticker. We still want to capture it as a post.
				size_t stickerLocation = currentTag.find("title=");
				size_t stickerEnd = currentTag.find(">");
				std::string stickerName = currentTag.substr(stickerLocation + 6, stickerEnd - (stickerLocation + 6));
				messageContent = messageContent + " " + stickerName;
				break;
			}
			messageContent = messageContent + " " + catString;
			tagSize = catString.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);
		}
			break;

			 
		case messageImageTag:{
			if (currentTag.find("<video") != -1)
			{
				std::string sourceString = getHtmlTag(inLine);
				tagSize = sourceString.size();
				size_t urlStart = sourceString.find("src=");
				size_t urlEnd = sourceString.find("alt");
				std::string catString = sourceString.substr(urlStart+4, urlEnd - (urlStart+4));
				messageContent = messageContent + " " + catString;
				inLine = inLine.substr(tagSize, inLine.size() - tagSize);
			}
			else 
			{
				std::string catString;
				size_t urlStart = currentTag.find("src=");
				size_t urlEnd = currentTag.find("alt", urlStart);
				catString = currentTag.substr(urlStart + 4, urlEnd - (urlStart + 4));
				messageContent = messageContent + " " + catString;
			}


		}
			break;

		case messageReactionTag:
		{
			size_t reactionNameStart = currentTag.find("title=");
			size_t reactionNameEnd = currentTag.find(">");
			currentReaction = currentTag.substr(reactionNameStart+6, reactionNameEnd - (reactionNameStart+6));
		}			
			break;

		case messageReactionCountTag:
		{
			std::string value = getHtmlTag(inLine);
			currentReactionCount = stoi(value);
			tagSize = value.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);

			//Then do our insert loop
			for (int i = 0; i<currentReactionCount; i++) 
			{
				insertMessage->addReactions(currentReaction);
			}
		}
			break;

		case mentionTag:
			{
				std::string catString = getHtmlTag(inLine);
				messageContent = messageContent + " " + catString;
				tagSize = catString.size();
				inLine = inLine.substr(tagSize, inLine.size() - tagSize);
			}
			break;

		case ignoreUntilPop: 
		{
			size_t tagStackSize = tagStack.size();
			while (tagStack.size() > (tagStackSize-1)) 
			{
				std::string nextTag = getHtmlTag(inLine);
				tagSize = nextTag.size();
				tagType tossTag = processTag(nextTag, true);
				inLine = inLine.substr(tagSize, inLine.size() - tagSize);

			}
		}
			break;

		case unknownTag:
			if (tagStack.back() == "<span class=chatlog__markdown-preserve>")
			{
				messageContent = messageContent + " " + currentTag;
				break;
				//this is the rest of a message after a mention. Hacky, but should work. 
			}
			std::cout << "unknown tag encountered: ";
			std::cout << currentTag << std::endl;
			std::cout << messageDateTime << std::endl;
			return;
			break;

		case ignoreTag:
			break;

		case ignoreNextTag:
		{ //the element after the edit tag must be skipped
			std::string skipTag = getHtmlTag(inLine); //we expect this to be an <a> tag that we do not care about
			tagSize = skipTag.size();
			inLine = inLine.substr(tagSize, inLine.size() - tagSize);
		}
			break;

		default:
			break;

		}

	}

	insertMessage->userName = messageUserName;
	insertMessage->dateTime = messageDateTime;
	insertMessage->postContent = messageContent;
	insertMessage->postType = currentPost;
	insertMessage->channelPosted = currentChannel;

	allMessages.push_back(insertMessage);


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

tagType DiscordServerStats::processTag(std::string inHTMLTag, bool igUPopFlag)
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
	std::string className = findClassName(inHTMLTag);
	//Div section handeled. 
	if (inHTMLTag.find("<div") != -1) 
	{
		returnTagType = ignoreTag;
		tagStack.push_back(inHTMLTag);
		//now we need to find the class, and base what we do off of that. 

		if (className == "chatlog__reaction")
		{
			returnTagType = messageReactionTag;
			return returnTagType;
		}
		else if (className == "chatlog__message-group" ||
			className == "chatlog__message-container" || className == "chatlog__message" ||
			className == "chatlog__message-aside" || className == "chatlog__reply-symbol" ||
			className == "chatlog__message-primary" ||
			className == "chatlog__reply-content" || className == "chatlog__header" ||
			className == "\"chatlog__content" || className == "chatlog__attachment" ||
			className == "chatlog__reactions" || className == "chatlog__sticker--media" ||
			className == "chatlog__reply-unknown" || className == "\"chatlog__message-container" ||
			className == "\"chatlog__embed-color-pill" || className == "chatlog__embed-content-container" ||
			className == "chatlog__embed-content" || className == "chatlog__embed-text" ||
			className == "chatlog__embed-title" || className == "chatlog__embed-color-pill" ||
			className == "chatlog__embed-author-container" || className == "chatlog__embed-spotify-container" ||
			className == "chatlog__embed-footer" || className == "chatlog__markdown-quote-content" ||
			className == "chatlog__embed-youtube-container" || className == "chatlog__attachment-generic" ||
			className == "chatlog__attachment" || className == "chatlog__attachment-generic-name" ||
			className == "chatlog__attachment-generic-size" || className == "\"chatlog__attachment")
		{
			return returnTagType;
		}
		else if (className == "chatlog__markdown")
		{
			returnTagType = ignoreNextTag;
			return returnTagType;
		}
		else if (className == "chatlog__embed" || className == "chatlog__short-timestamp" ||
			className == "chatlog__reply" || className == "chatlog__attachment-spoiler-caption")
		{
			returnTagType = ignoreUntilPop;
			return returnTagType;

		}
		else if (className == "\"chatlog__markdown-quote\"" || className == "\"chatlog__markdown-quote-border\"" ||
			className == "\"chatlog__markdown-quote-content\"")
		{
			returnTagType = ignoreUntilPop; //for debugging
			return returnTagType;
		}
		else if (className == "chatlog__sticker")
		{
			returnTagType = messageTextTag;
			return returnTagType;

		}
		else if (className == "noClass" && igUPopFlag == false)
		{
			std::cout << "Div with no class name: ";
			std::cout << inHTMLTag << std::endl;
		}
		else if (igUPopFlag == false)
		{
			std::cout << "Unrecognized Div Class: ";
			std::cout << inHTMLTag << std::endl;
			//there are a lot of div classes, and we can probably ignore most of them. 
		}

	}

	if (inHTMLTag.find("</div>") != -1 && inHTMLTag.size() == 6) 
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size()-1).find("<div") != -1)
		{
			tagStack.pop_back(); //if we are currently on a div tag, we can pop that tag off of the stack. 
		}
		return returnTagType;
	}


	//I do feel like I'm repeating myself a but here, but we do need to seperate out our tags. Might consolidate later during optimization, if I do that. 
	if (inHTMLTag.find("<span") != -1) 
	{
		returnTagType = ignoreTag;
		tagStack.push_back(inHTMLTag);

		if (className == "chatlog__markdown-preserve")
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
			return returnTagType;
		}
		else if (className == "chatlog__edited-timestamp")
		{
			returnTagType = ignoreNextTag;
			return returnTagType;
		}
		else if (className == "chatlog__reply-link")
		{
			returnTagType = ignoreNextTag;
			return returnTagType;
		}
		else if (className == "chatlog__reply-edited-timestamp") 
		{
			returnTagType = ignoreNextTag;
			return returnTagType;
		}
		else if (className == "\"chatlog__markdown-mention\"" || className == "chatlog__markdown-mention")
		{
			returnTagType = mentionTag;
			return returnTagType;
		}
		else if (className == "\"chatlog__markdown-spoiler")
		{
			returnTagType = messageTextTag;
			return returnTagType;
		}
		else if (className.find("system-notification") != -1)
		{
			returnTagType = ignoreUntilPop;
			return returnTagType;
		}
		else if (className == "noClass" || className == "chatlog__author-tag")
		{
			returnTagType = ignoreUntilPop;
			return returnTagType;

		}
		else if (!igUPopFlag)
		{
			std::cout << "unrecognized Span class: ";
			std::cout << inHTMLTag << std::endl;

		}

	}

	if (inHTMLTag.find("</span>") != -1 && inHTMLTag.size() == 7)
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<span") != -1)
		{
			tagStack.pop_back(); //if we are currently on a span tag, we can pop that tag off of the stack. 
		}
		return returnTagType;
	}

	if (inHTMLTag.find("<a") != -1) 
	{
		tagStack.push_back(inHTMLTag);
		if (className == "noClass") 
		{
			//an a tag with no class is a link in a message, An A tag with a class is part of an embed. 
			returnTagType = messageTextTag;
			return returnTagType;
		}
;
	}
	if (inHTMLTag.find("</a>") != -1 && inHTMLTag.size() == 4)
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<a") != -1)
		{
			tagStack.pop_back(); //if we are currently on an a tag, we can pop that tag off of the stack. 
		}
		return returnTagType;
	}

	if (inHTMLTag.find("<img") != -1)
	{
		//this is not one we have to put on our stack
		if (className == "chatlog__attachment-media")
		{
			returnTagType = messageImageTag;
			return returnTagType;
		}
		else if (className == "chatlog__avatar" || className == "chatlog__reply-avatar" ||
			className == "chatlog__emoji chatlog__emoji--small" || className == "chatlog__emoji" || 
			className == "\"chatlog__emoji" || className == "chatlog__sticker--media")
		{
			returnTagType = ignoreTag;
			return returnTagType;
		}
		else if (className == "chatlog__embed-generic-image")
		{
			returnTagType = messageImageTag;
			return returnTagType;
		}
		else if (className  == "noClass")
		{
			std::cout << "img with no class, seems wrong" << std::endl;
			return returnTagType;
		}
		else if (!igUPopFlag)
		{
			std::cout << "unrecognized Image class: ";
			std::cout << inHTMLTag << std::endl;

		}

	}

	if (inHTMLTag.find("<video") != -1)
	{
		tagStack.push_back(inHTMLTag);
		

		if (className == "chatlog__attachment-media")
		{
			returnTagType = messageImageTag;
			return returnTagType;
		}
		else if (className == "chatlog__embed-generic-video")
		{
			returnTagType = messageImageTag;
			return returnTagType;

		}
		else if (className == "chatlog__embed-generic-gifv") 
		{
			returnTagType = messageImageTag;
			return returnTagType;
		}


	}

	if (inHTMLTag.find("</video") != -1)
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<video") != -1)
		{
			tagStack.pop_back(); //if we are currently on an a tag, we can pop that tag off of the stack. 
		}
		return returnTagType;

	}

	if (inHTMLTag.find("<em") != -1) 
	{
		tagStack.push_back(inHTMLTag);
		returnTagType = messageTextTag;
		return returnTagType;
	}

	if (inHTMLTag.find("</em") != -1) 
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<em") != -1)
		{
			tagStack.pop_back(); //if we are currently on an a tag, we can pop that tag off of the stack. 
		}
		return returnTagType;
	}

	if (inHTMLTag.find("<iframe") != -1)
	{
		tagStack.push_back(inHTMLTag);
		returnTagType = ignoreTag;
		return returnTagType;
	}

	if (inHTMLTag.find("</iframe") != -1)
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<iframe") != -1)
		{
			tagStack.pop_back(); //if we are currently on an a tag, we can pop that tag off of the stack. 
		}
		return returnTagType;

	}

	if (inHTMLTag.find("<svg") != -1)
	{
		if (className == "chatlog__system-notification-icon")
		{
			returnTagType = ignoreUntilPop;
			return returnTagType;

		}

	}

	if (inHTMLTag.find("</svg") != 1)
	{
		returnTagType = ignoreTag;
		if (tagStack.at(tagStack.size() - 1).find("<svg") != -1)
		{
			tagStack.pop_back(); 
		}
		return returnTagType;

	}



	return returnTagType;
}

void DiscordServerStats::parseTypes()
{
	for (int i = 0; i < allMessages.size(); i++)
	{
		std::string parseString = allMessages[i]->postContent;

		if (parseString.find(".jpg") != -1 || 
			parseString.find(".png") != -1 ||
			parseString.find(".tiff") != -1 ||
			parseString.find(".gif") != -1)
		{
			allMessages[i]->postType = image;
		}

		if (parseString.find("youtube") != -1 ||
			parseString.find("youtu.be") != -1 || 
			parseString.find("bandcamp") != -1 || 
			parseString.find("spotify") != -1 || 
			parseString.find("itunes.apple") != -1 || 
			parseString.find("soundcloud") != -1)
		{
			allMessages[i]->postType = link;
		}

	}

}

//I'm not brushed up on my sorting algorithms. Sorry mom. I'm an embedded FW engineer.
void DiscordServerStats::sortMap(std::vector<std::string> *users, std::vector<int> *values, std::unordered_map<std::string, int> inputMap)
{
	for (auto i : inputMap) 
	{
		int inValue = i.second;
		std::string user = i.first;

		if (values->size() == 0) 
		{
			values->push_back(inValue);
			users->push_back(user);
			continue;
		}
		bool inserted = false;
		for (int j=0; j<values->size(); j++) 
		{
			if (inValue > values->at(j))
			{
				inserted = true;
				values->insert(values->begin()+j, inValue);
				users->insert(users->begin() + j, user);
				break;
			}

		}
		if (!inserted) 
		{
			values->push_back(inValue);
			users->push_back(user);
		}
	}
}


std::string DiscordServerStats::findClassName(std::string inHtmlTag)
{
	std::string className;
	size_t classStart = inHtmlTag.find("class=");
	size_t classEnd = inHtmlTag.find(" ", classStart);
	size_t classEnd2 = inHtmlTag.find(">", classStart);

	if (classEnd != -1)
	{
		className = inHtmlTag.substr(classStart + 6, classEnd - (classStart + 6));
	}
	else if (classEnd2 != -1)
	{
		className = inHtmlTag.substr(classStart + 6, classEnd2 - (classStart + 6));
	}
	else
	{
		className = "noClass";
	}
	return className;
}
