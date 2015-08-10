// More Message Commands
// by PepsiOtaku, based loosely on Kazesui's DynText plugin
// Version 1.0

#include <DynRPG/DynRPG.h>
#include <sstream>

std::string parseText(std::string in);
char* g_pluginName;

bool onStartup(char* pluginName) {
    g_pluginName = pluginName;
    return true;
}

std::string formatText(std::string s)
{
    size_t size = 2;
    //std::string d = "]";
    while(size < s.size() && s[size++]!=']');
    size -= 2;
    int n = atoi(parseText(s.substr(2, size)).c_str());
    std::string r = s.substr(0,s.size()-1);
    //std::string r = "** NOT FOUND **";

    switch (s[0])
    {
        case 'A': case 'a':
            if (n < RPG::attributes.count())
                return RPG::attributes[n]->name;
            else return r;

        case 'B': case 'b':
            if (n < RPG::battleSettings->battleCommands.count())
                return RPG::battleSettings->battleCommands[n]->name;
            else return r;

        // C,c (Color)

        case 'D': case 'd':
            if (n < RPG::actors.count())
                return RPG::actors[n]->getDegree();
            else return r;

        case 'I':
            return RPG::getItemName(n);

        case 'i':
            return RPG::getItemDescription(n);

        case 'K':
            return RPG::getSkillName(n);

        case 'k':
            return RPG::getSkillDescription(n);

        case 'M':
            if (n < RPG::dbMonsters.count())
                return RPG::dbMonsters[n]->name;
            else return r;

        case 'm':
        {
            std::stringstream ss;
            ss << RPG::dbMonsters[n]->itemId;
            return ss.str();
        }

        // N,n (Hero Name)

        case 'O': case 'o':
            return RPG::getConditionName(n);

        case 'R': case 'r':
            if (n < 88)
                return RPG::vocabulary[n];
            else return r;

        // S,s (Speed)

        case 'T': case 't':
            if (n < RPG::terrains.count())
                return RPG::terrains[n]->name;
            else return r;

        case 'V': case 'v':
        {
            std::stringstream ss;
            ss << RPG::variables[n];
            return ss.str();
        }
        default:
            return r;
    }
}

std::string parseText(std::string in)
{
    std::string out;
    for(size_t i = 0; i < in.size(); i++)
    {
        char d = in[i+1];
        if (in[i] == '\\'
            && (d == 'I' || d == 'i' || d == 'V' || d == 'v' || d == 'D' || d == 'd'
            || d == 'K' || d == 'k' || d == 'O' || d == 'o' || d == 'M' || d == 'm'
            || d == 'A' || d == 'a' || d == 'R' || d == 'r' || d == 'T' || d == 't'
            || d == 'B' || d == 'b' ))
        {
            out += formatText(in.substr(i+1, in.size()));
            while(i < in.size() && in[i++]!=']');
            i--;
            while(i < in.size() && (in[i+1]==']'))i++;
        }
        else
            out += in[i];
    }
    return out;
}

/*bool onEventCommand	(	RPG::EventScriptLine * 	scriptLine, RPG::EventScriptData * 	scriptData,
                        int eventId, int pageId, int lineId, int *nextLineId)
{*/
bool onComment(const char* text, const RPG::ParsedCommentData* parsedData, RPG::EventScriptLine* nextScriptLine,
		       RPG::EventScriptData* scriptData, int eventId, int pageId, int lineId, int* nextLineId)
{
    std::string cmd = parsedData->command;
    if(!cmd.compare("process_cmd") && nextScriptLine->command == RPG::EVCMD_SHOW_MESSAGE)
    {
        std::string txt;
        txt = parseText(nextScriptLine->stringParameter);
        //  Parse and replace actual message line
        nextScriptLine->stringParameter = txt;
        //  Parse the other 3 lines
        for (int i=2;i<=4; i++) {
            if (scriptData->line(lineId+i)->command == RPG::EVCMD_ADD_LINE_TO_MESSAGE) {
                txt = parseText(scriptData->line(lineId+i)->stringParameter);
                scriptData->line(lineId+i)->stringParameter = txt;
            } else break;
        }
        return false;
    }
    return true;
}

