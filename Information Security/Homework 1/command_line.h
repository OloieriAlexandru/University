#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <iostream>
#include <string>
#include <vector>
#include <ostream>
#include <unordered_map>
#include <cstring>

#define NUM_ARG_MISSING -4918241

namespace cmd{
    enum commandId{
    	SEND_FILE,
    	LISTALL,
    	PRINT_K3,
    	PRINT_IV,
        CLOSE,
        EMPTYLINE, // returned when the entered line is empty
        WOC, // the type of all invalid commands
        WARG, // the type for commands with invalid arguments
        WCARG, // the type for commands with invalid number of arguments
        WOCOPT // the type for commands with invalid options
    };

    enum commandArgumentType{
        STR,
        NUM
    };

    enum commandOptionType{
        STRING,
        NUMBER,
        BOOLEAN
    };

    std::string commandEnumToString(commandId cmdId);
    std::string commandArgumentTypeEnumToString(commandArgumentType cmdArgumentType);
    std::string commandOptionTypeEnumToString(commandOptionType cmdOptionType);

    class commandArgument;
    class commandOptionDefaultValue;
    class commandOption;
    class commandInfo;
    class commandParser;

    class argumentResult{
    public:
        commandArgumentType             type;
        int                             numberValue;
        std::string                     stringValue;
        std::string                     name;

                                        argumentResult() {}
                                        argumentResult(const cmd::commandArgument& commandArg);

        void                            setValue(void* value);

        friend std::ostream&            operator<<(std::ostream& out, const argumentResult& optResult);
    };

    class optionResult{
    public:
        commandOptionType               type;
        int                             numberValue;
        bool                            booleanValue;
        std::string                     stringValue;
        std::string                     name;

                                        optionResult() {}
                                        optionResult(const std::string optionName, const commandOptionDefaultValue& defaultValue);

        void                            updateValue(void* newValue);

        friend std::ostream&            operator<<(std::ostream& out, const optionResult& optResult);
    };

    class commandResult{
    public:
        commandId                       id;
        std::vector<argumentResult>     arguments;
        std::vector<optionResult>       options;
                                        commandResult();
        void                            initOptions(const commandInfo& cmdInfo);
        void                            updateOption(const std::string& optionName, void* newValue);
        void                            clear();

        std::string                     getStringArgumentValue(const std::string& argName) const;
        int                             getNumberArgumentValue(const std::string& argName) const;
        std::string                     getStringOptionValue(const std::string& optionName) const;
        int                             getNumberOptionValue(const std::string& optionName) const;
        bool                            getBooleanOptionValue(const std::string& optionName) const;

        friend std::ostream&            operator<<(std::ostream& out, const commandResult& cmdResult);
    };

    class commandArgument{
    private:
        std::string                     name;
        commandArgumentType             type;
        int                             numberValue;
        std::string                     stringValue;

        friend class                    argumentResult;
    public:
                                        commandArgument(const std::string& argumentName, cmd::commandArgumentType argumentType);

        friend std::ostream&            operator<<(std::ostream& out, const commandArgument& cmdArgument);
    };

    class commandOptionDefaultValue{
    private:
        commandOptionType               type;
        int                             numberValue;
        bool                            booleanValue;
        std::string                     stringValue;

        friend class                    optionResult;
    public: 
        void                            addValue(commandOptionType optionType, void* value);
        const void*                     getValue() const;

        friend std::ostream&            operator<<(std::ostream& out, const commandOption& cmdOption);
    };

    class commandOption{
    private:
        std::string                     name;
        commandOptionType               type;
        commandOptionDefaultValue       defaultValue;

        friend class                    commandResult;
        friend class                    commandParser;
    public:
                                        commandOption(std::string optionName);

        void                            addDefaultValue(commandOptionType optionType, void* defValue);
    
        friend std::ostream&            operator<<(std::ostream& out, const commandOption& cmdOption);
    };

    class commandInfo{
    private:
        commandId                           id;
        std::vector<cmd::commandArgument>   arguments;
        std::vector<cmd::commandOption>     options;
        std::string                         name;
        std::string                         description;

        friend class                    commandResult;
        friend class                    commandParser;
    public:
                                        commandInfo(commandId commandId, const char* commandName, const char* commandDescription);

        void                            addStringArgument(const char* argumentName);
        void                            addNumberArgument(const char* argumentName);
        void                            addStringOption(const char* optionName, const char* defaultValue);
        void                            addNumberOption(const char* optionName, int defaultValue);
        void                            addBooleanOption(const char* optionName, bool defaultValue);
    
        friend std::ostream&            operator<<(std::ostream& out, const commandInfo& cmdInfo);
    };

    class commandParser{
    private:
        std::vector<commandInfo>                commands;
        std::unordered_map<commandId, int>      commandIndex;

        bool                                    parseArgument(char arguments[], int firstPos, int& lastPos, int cmdIndex, int argumentIndex, cmd::commandResult& result);
        bool                                    parseAndCheckOptionValue(char arguments[], int startPos, int& lastPos, const cmd::commandOption& opt, int position, cmd::commandResult& result);
        bool                                    parseOption(char arguments[], int firstPos, int& lastPos, int cmdIndex, cmd::commandResult& result);
    public:
        commandResult                   parse(const std::string& str);

        void                            addCommand(commandId id, const char* commandName, const char* commandDescription);
        void                            addCommandArgumentString(commandId id, const char* argumentName);
        void                            addCommandArgumentNumber(commandId id, const char* argumentName);
        void                            addCommandOptionString(commandId id, const char* optionName, const char* defaultValue);
        void                            addCommandOptionNumber(commandId id, const char* optionName, int defaultValue);
        void                            addCommandOptionBoolean(commandId id, const char* optionName, bool defaultValue);

        friend std::ostream&            operator<<(std::ostream& out, const commandParser& parser);
    };
}

#endif // COMMAND_LINE_H
