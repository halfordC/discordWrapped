// DiscordWrappedBot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include "DiscordServerStats.h"

int main(int argc, char** argv)
{
    if (argc < 2) 
    {
        std::cout << "please input filepath" << std::endl;
        return 1;
    }

    std::filesystem::path inPath(argv[1]);

    DiscordServerStats *myStats = new DiscordServerStats();



    for (std::filesystem::recursive_directory_iterator next(inPath), end; next != end; next++) 
    {
        if (next->path().extension() == ".html")
        {
            std::string fileName = next->path().filename().string();
            if (fileName.find("Milwaukee Hardcore") != -1)
            {
                //we have found our file. Now we need to parse. 
                std::ifstream currentFile;
                currentFile.open(next->path());
                if (!currentFile.is_open()) 
                {
                    std::cout << fileName << "File failed to open" << std::endl;
                    return 1;
                }

                std::string line;
                while (getline(currentFile, line))
                {
                    size_t chanelLocation = line.find("<div class=preamble__entry>");
                    if ( chanelLocation != std::string::npos) 
                    {
                        chanelLocation = line.find("<div class=preamble__entry>", chanelLocation+1);
                        //make substring of this line, and send this to stats object
                        int chanelLocationEnd = line.find("</div>", chanelLocation);
                        std::string channelFound = line.substr(chanelLocation+27, chanelLocationEnd-(chanelLocation+27));
                        myStats->changeChannel(channelFound);
                    }
                    else if (line.find("<div class=chatlog__message-group>") != -1)
                    {
                        //we still need to make sure that we have the full message. 
                        while (line.find("</div></div></div></div>") == -1) 
                        {
                            std::string tempString;
                            getline(currentFile, tempString);
                            line = line.append(tempString);
                            //keep grabing lines until we have reached the end of the message. 
                        }
                        //we have found a message.
                        myStats->addMessage(line);

                    }
                }



            }

        }

    }

    //steps:
    /*
    * 1. recursivley search folders for all milwaukee hardcore html files
    * 2. take in contents and parse out messages
    * 3. put messages in workable data structure
    * 4. Count up all of our data structure based on requirnments
    
    */
}
