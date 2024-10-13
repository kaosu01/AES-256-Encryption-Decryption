#include <iostream>
#include <string>
#include "prompt.h"
#include "getmsg.h"

std::string get_message()
{
    while(true)
    {
        display_prompt();
        
        std::string text = "";
        std::getline(std::cin,text);

        if(text == "Exit" || text == "exit")
            { exit(0); }
        else if(text == "")
            { std::cerr << "Enter Something!" << std::endl; continue;}
        else
        {
            return text;
        }
    }
}