#include "command_line.h"

std::string cmd::commandEnumToString(cmd::commandId cmdId){
    switch(cmdId){
        case cmd::commandId::SEND_FILE:
        	return "send file";
        case cmd::commandId::CLOSE:
            return "close";
        case cmd::commandId::EMPTYLINE:
            return "empty line command";
        case cmd::commandId::WOC:
            return "wrong command";
        case cmd::commandId::WOCOPT:
            return "wrong command option";
        default:
            break;
    }
    return "";
}

std::string cmd::commandArgumentTypeEnumToString(commandArgumentType cmdArgumentType){
    switch(cmdArgumentType){
        case cmd::commandArgumentType::STR:
            return "string";
        case cmd::commandArgumentType::NUM:
            return "number";
    }
    return "";
}

std::string cmd::commandOptionTypeEnumToString(cmd::commandOptionType cmdOptionType){
    switch(cmdOptionType){
        case cmd::commandOptionType::STRING:
            return "string";
        case cmd::commandOptionType::NUMBER:
            return "number";
        case cmd::commandOptionType::BOOLEAN:
            return "bool";
    }
    return "";
}

// --------- argumentResult ----------------------------------------------------------------------------------//

cmd::argumentResult::argumentResult(const cmd::commandArgument& commandArg) {
    type = commandArg.type;
    name = commandArg.name;
}

void cmd::argumentResult::setValue(void* value) {
    switch(type){
        case STR:
            stringValue = std::string((const char*)value);
            break;
        case NUM:
            numberValue = *((int*)value);
            break;
        default:
            break;
    }
}

namespace cmd{
    std::ostream& operator<<(std::ostream& out, const cmd::argumentResult& argResult){
        out<<"name:"<<argResult.name<<", type:"<<commandArgumentTypeEnumToString(argResult.type);
        out<<", value:";
        switch(argResult.type){
            case cmd::commandArgumentType::STR:
                out<<argResult.stringValue;
                break;
            case cmd::commandArgumentType::NUM:
                out<<argResult.numberValue;
                break;
            default:
                break;
        }
        return out;
    }
}

// --------- optionResult ----------------------------------------------------------------------------------//

void cmd::optionResult::updateValue(void* newValue){
    switch(type){
        case STRING:
            stringValue = std::string((const char*)newValue);
            break;
        case NUMBER:
            numberValue = *((int*)newValue);
            break;
        case BOOLEAN:
            booleanValue = *((bool*)newValue);
            break;
        default:
            break;
    }
}

cmd::optionResult::optionResult(const std::string optionName, const cmd::commandOptionDefaultValue& defaultValue){
    int sz = 0;
    while (sz < optionName.size() && optionName[sz] != ':'){
        ++sz;
    }
    name = optionName.substr(0, sz);
    
    type = defaultValue.type;
    switch(type){
        case STRING:
            stringValue = defaultValue.stringValue;
            break;
        case NUMBER:
            numberValue = defaultValue.numberValue;
            break;
        case BOOLEAN:
            booleanValue = defaultValue.booleanValue;
            break;
        default:
            break;
    }
}

namespace cmd{
    std::ostream& operator<<(std::ostream& out, const cmd::optionResult& optResult){
        out<<"name:"<<optResult.name<<", type:"<<commandOptionTypeEnumToString(optResult.type);
        out<<", value:";
        switch(optResult.type){
            case STRING:
                out<<(optResult.stringValue);
                break;
            case NUMBER:
                out<<(optResult.numberValue);
                break;
            case BOOLEAN:
                out<<(optResult.booleanValue?"true":"false");
                break;
            default:
                break;
        }
        return out;
    }
}

// --------- commandResult ----------------------------------------------------------------------------------//

cmd::commandResult::commandResult(){
    id = cmd::commandId::WOC;
}

void cmd::commandResult::initOptions(const commandInfo& cmdInfo){
    for (int i=0;i<cmdInfo.options.size();++i){
        options.push_back(optionResult(cmdInfo.options[i].name, cmdInfo.options[i].defaultValue));
    }
}

void cmd::commandResult::updateOption(const std::string& optionName, void* newValue){
    for (int i=0;i<options.size();++i){
        if (options[i].name == optionName){
            options[i].updateValue(newValue);
        }
    }
}

void cmd::commandResult::clear(){
    arguments.clear();
    options.clear();
}

namespace cmd{
    std::ostream& operator<<(std::ostream& out, const cmd::commandResult& cmdResult){
        out<<"Command: "<<commandEnumToString(cmdResult.id)<<'\n';
        if (cmdResult.arguments.size()){
            out<<"Arguments: \n";
            for (auto& arg:cmdResult.arguments){
                out<<arg<<'\n';
            }
        }
        if (cmdResult.options.size()){
            out<<"Options:\n";
            for (auto& option:cmdResult.options){
                out<<option<<'\n';
            }
        }
        return out;
    }
}

std::string cmd::commandResult::getStringArgumentValue(const std::string& argName) const {
    for (auto& arg:arguments){
        if (arg.name == argName){
            if (arg.type != cmd::commandArgumentType::STR){
                return "";
            }
            return arg.stringValue;
        }
    }
    return "";
}

int cmd::commandResult::getNumberArgumentValue(const std::string& argName) const {
    for (auto& arg:arguments){
        if (arg.name == argName){
            if (arg.type != cmd::commandArgumentType::NUM){
                return NUM_ARG_MISSING;
            }
            return arg.numberValue;
        }
    }
    return NUM_ARG_MISSING;
}

std::string cmd::commandResult::getStringOptionValue(const std::string& optionName) const{
    for (auto& option:options){
        if (option.name == optionName){
            if (option.type != cmd::commandOptionType::STRING){
                return "";
            }
            return option.stringValue;
        }
    }
    return "";
}

int cmd::commandResult::getNumberOptionValue(const std::string& optionName) const{
    for (auto& option:options){
        if (option.name == optionName){
            if (option.type != cmd::commandOptionType::NUMBER){
                return 0;
            }
            return option.numberValue;
        }
    }
    return 0;
}

bool cmd::commandResult::getBooleanOptionValue(const std::string& optionName) const{
    for (auto& option:options){
        if (option.name == optionName){
            if (option.type != cmd::commandOptionType::BOOLEAN){
                return false;
            }
            return option.booleanValue;
        }
    }
    return false;
}

// --------- commandArgument ----------------------------------------------------------------------------------//

cmd::commandArgument::commandArgument(const std::string& argumentName, cmd::commandArgumentType argumentType) {
    name = argumentName;
    type = argumentType;
}

namespace cmd {
    std::ostream& operator<<(std::ostream& out, const cmd::commandArgument& cmdArgument) {
        out<<"arg:"<<cmdArgument.name;
        return out;
    }
}

// --------- commandOptionDefaultValue ----------------------------------------------------------------------------------//

void cmd::commandOptionDefaultValue::addValue(commandOptionType optionType, void* value){
    type = optionType;
    switch(optionType){
        case STRING:
            stringValue = std::string((const char*)value);
            break;
        case NUMBER:
            numberValue = *((int*)value);
            break;
        case BOOLEAN:
            booleanValue = *((bool*)value);
            break;
        default:
            break;
    }
    return;
}

const void* cmd::commandOptionDefaultValue::getValue() const{
    switch(type){
        case STRING:
            return (const std::string*)&stringValue;
        case NUMBER:
            return (const int*)&numberValue;
        case BOOLEAN:
            return (const bool*)&booleanValue;
        default:
            break;
    }
    return NULL;
}

// --------- commandOption ----------------------------------------------------------------------------------//

cmd::commandOption::commandOption(std::string optionName){
    name = optionName;
}

void cmd::commandOption::addDefaultValue(cmd::commandOptionType optionType, void* defValue){
    type = optionType;
    defaultValue.addValue(optionType, defValue);
}

namespace cmd{
    std::ostream& operator<<(std::ostream& out, const cmd::commandOption& cmdOption){
        out<<"\toption: "<<cmdOption.name<<"    default: ";
        switch(cmdOption.type){
            case STRING:
                out<<*((const std::string*)cmdOption.defaultValue.getValue());
                break;
            case NUMBER:
                out<<*((const int*)cmdOption.defaultValue.getValue());
                break;
            case BOOLEAN:
                out<<(*((const bool*)cmdOption.defaultValue.getValue())?"true" : "false");
                break;
            default:
                break;
        }
        return out;
    }
}

// --------- commandInfo ----------------------------------------------------------------------------------//

cmd::commandInfo::commandInfo(commandId commandId, const char* commandName, const char* commandDescription){
    id = commandId;
    name = std::string(commandName);
    description = std::string(commandDescription);
}

void cmd::commandInfo::addStringArgument(const char* argumentName) {
    std::string name(argumentName);
    cmd::commandArgumentType type = cmd::commandArgumentType::STR;
    arguments.push_back(commandArgument(name, type));
}

void cmd::commandInfo::addNumberArgument(const char* argumentName) {
    std::string name(argumentName);
    cmd::commandArgumentType type = cmd::commandArgumentType::NUM;
    arguments.push_back(commandArgument(name, type));
}

void cmd::commandInfo::addStringOption(const char* optionName, const char* defaultValue){
    std::string name(optionName);
    cmd::commandOption cmdOption(name);

    cmd::commandOptionType type = cmd::commandOptionType::STRING;

    cmdOption.addDefaultValue(type,(void*)(defaultValue));
    options.push_back(cmdOption);
}

void cmd::commandInfo::addNumberOption(const char* optionName, int defaultValue){
    std::string name(optionName);
    cmd::commandOption cmdOption(name);

    cmd::commandOptionType type = cmd::commandOptionType::NUMBER;

    cmdOption.addDefaultValue(type,(void*)(&defaultValue));
    options.push_back(cmdOption);
}

void cmd::commandInfo::addBooleanOption(const char* optionName, bool defaultValue){
    std::string name(optionName);
    cmd::commandOption cmdOption(name);

    cmd::commandOptionType type = cmd::commandOptionType::BOOLEAN;

    cmdOption.addDefaultValue(type,(void*)(&defaultValue));
    options.push_back(cmdOption);
}

namespace cmd{
    std::ostream& operator<<(std::ostream& out, const cmd::commandInfo& cmdInfo){
        out<<cmdInfo.name<<' ';
        for (auto& arg:cmdInfo.arguments){
            out<<arg<<' ';
        }
        out<<"\n\t--> "<<cmdInfo.description<<'\n';
        for (auto& option:cmdInfo.options){
            out<<option<<'\n';
        }
        return out;
    }
}

// --------- commandParser ----------------------------------------------------------------------------------//

bool cmd::commandParser::parseArgument(char arguments[], int firstPos, int& lastPos, int cmdIndex, int argumentIndex, cmd::commandResult& result) {
    bool res = true;
    char old = arguments[lastPos+1];
    int numValue;
    cmd::argumentResult arg(commands[cmdIndex].arguments[argumentIndex]);
    if (arg.type == STR){
        if (arguments[firstPos] == '\"'){
            while (arguments[lastPos] && arguments[lastPos] != '\"'){
                ++lastPos;
            }
            if (arguments[lastPos] != '\"'){
                return false;
            }
        }
    }
    arguments[lastPos+1] = '\0';
    switch(arg.type){
        case STR:
            if (arguments[firstPos] == '\"'){
                arguments[lastPos] = '\0';
                arg.setValue((void*)(arguments+firstPos+1));
                arguments[lastPos] = '\"';
            } else {
                arg.setValue((void*)(arguments+firstPos));
            }
            break;
        case NUM:
            if (!((arguments[firstPos]>='0'&&arguments[firstPos]<='9')||
                (arguments[firstPos]=='-'&&arguments[firstPos+1]>='0'&&arguments[firstPos+1]<='9'))){
                res = false;
            } else {
                numValue = atoi(arguments+firstPos);
                arg.setValue((void*)(&numValue));
            }
            break;
        default:
            break;
    }
    if (res){
        result.arguments.push_back(arg);
    }
    arguments[lastPos+1] = old;
    return res;
}

bool cmd::commandParser::parseAndCheckOptionValue(char arguments[], int startPos, int& lastPos, const cmd::commandOption& opt, int position, cmd::commandResult& result){
    bool res = true;
    char old = arguments[lastPos+1];
    int numValue;
    if (opt.type == STRING){
        if (arguments[startPos] == ':' && arguments[startPos+1] == '\"'){
            while (arguments[lastPos] && arguments[lastPos] != '\"'){
                ++lastPos;
            }
            if (arguments[lastPos] != '\"'){
                return false;
            }
        }
    }
    std::string currentOptionName = opt.name.substr(0,position);
    arguments[lastPos+1] = '\0';
    switch (opt.type){
        case STRING:
            if (arguments[startPos]!=':' || !arguments[startPos+1]){
                res = false;
            } else {
                if (arguments[startPos+1] == '\"'){
                    arguments[lastPos] = '\0';
                    result.updateOption(currentOptionName, (void*)(arguments+startPos+2));
                    arguments[lastPos] = '\"';
                } else {
                    result.updateOption(currentOptionName, (void*)(arguments+startPos+1));
                }
            }
            break;
        case NUMBER:
            if (arguments[startPos]!=':' || !arguments[startPos+1]){
                res = false;
            } else {
                numValue = atoi(arguments+startPos+1);
                result.updateOption(currentOptionName, (void*)(&numValue));
            }
            break;
        case BOOLEAN:
            result.updateOption(currentOptionName, (void*)(&res)); 
            break;
        default:
            break;
    }
    arguments[lastPos+1] = old;
    return res;
}

bool cmd::commandParser::parseOption(char arguments[], int firstPos, int& lastPos, int cmdIndex, cmd::commandResult& result){
    if (cmdIndex >= commands.size() || cmdIndex < 0){
        return false;
    }
    int argumentLen = lastPos - firstPos + 1;

    for (auto& option:commands[cmdIndex].options){
        int i = 0;
        while (i < option.name.size()){
            if (option.name[i] != arguments[firstPos+i] || option.name[i] == ':'){
                break;
            }
            ++i;
        }
        if (i == option.name.size() || option.name[i] == ':'){
            return parseAndCheckOptionValue(arguments, firstPos+i, lastPos, option, i, result);
        } 
    }

    return false;
}

cmd::commandResult cmd::commandParser::parse(const std::string& str){
    cmd::commandResult result;

    char* stringCmd = strdup(str.c_str());
    if (!stringCmd){
        return result; // error ?
    }

    char* arguments = strtok(stringCmd, " \t\n");
    if (!arguments){
        result.id = cmd::commandId::EMPTYLINE;
        return result;
    }
    std::string enteredCommand(arguments);

    int cmdIndex = -1;
    for (int i=0;i<commands.size();++i){
        if (enteredCommand == commands[i].name){
            cmdIndex = i;
            break;
        }
    }

    if (cmdIndex == -1){
        return result; // invalid command
    }

    result.id = commands[cmdIndex].id;
    result.initOptions(commands[cmdIndex]);

    arguments = strtok(NULL, "");

    int count = 0;

    int len = (arguments ? strlen(arguments) : 0), lastPos = 0;
    for (int i=0;i<len;++i){
        while (i<len && arguments[i] != ' '){
            ++i;
        }

        --i;
        if (count < commands[cmdIndex].arguments.size()){
            if (!parseArgument(arguments, lastPos, i, cmdIndex, count, result)){
                result.id = cmd::commandId::WARG;
                result.clear();
                break;
            }
        } else {
            if (!parseOption(arguments, lastPos, i, cmdIndex, result)){
                result.id = cmd::commandId::WOCOPT;
                result.clear();
                break;
            }
        }
        ++i;

        while (i < len && arguments[i] == ' '){
            ++i;
        }
        lastPos = i;
        ++count;
    }
    if (count < commands[cmdIndex].arguments.size() && result.id != cmd::commandId::WARG){
        result.id = cmd::commandId::WCARG;
        result.clear();
    }
    if (stringCmd){
        delete[] stringCmd;
    }
    return result;
}

void cmd::commandParser::addCommand(commandId id, const char* commandName, const char* commandDescription){
    commandIndex[id] = commands.size();
    commands.push_back(cmd::commandInfo(id, commandName, commandDescription));
}

void cmd::commandParser::addCommandArgumentString(commandId id, const char* argumentName) {
    if (!commandIndex.count(id)){
        return;
    }
    int index = commandIndex[id];
    commands[index].addStringArgument(argumentName);
}

void cmd::commandParser::addCommandArgumentNumber(commandId id, const char* argumentName) {
    if (!commandIndex.count(id)){
        return;
    }
    int index = commandIndex[id];
    commands[index].addNumberArgument(argumentName);
}

void cmd::commandParser::addCommandOptionString(commandId id, const char* optionName, const char* defaultValue){
    if (!commandIndex.count(id)){
        return;
    }
    int index = commandIndex[id];
    commands[index].addStringOption(optionName, defaultValue);
}

void cmd::commandParser::addCommandOptionNumber(commandId id, const char* optionName, int defaultValue){
    if (!commandIndex.count(id)){
        return;
    }
    int index = commandIndex[id];
    commands[index].addNumberOption(optionName, defaultValue);
}

void cmd::commandParser::addCommandOptionBoolean(commandId id, const char* optionName, bool defaultValue){
    if (!commandIndex.count(id)){
        return;
    }
    int index = commandIndex[id];
    commands[index].addBooleanOption(optionName, defaultValue);
}

namespace cmd{
    std::ostream& operator<<(std::ostream& out, const cmd::commandParser& parser){
        int no = 1;
        for (auto& command : parser.commands){
            out<<(no++)<<". "<<command;
        }
        return out;
    }
}
