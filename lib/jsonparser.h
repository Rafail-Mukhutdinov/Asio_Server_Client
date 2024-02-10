#pragma once
#include <iostream>

namespace Serf
{
    class jsonparser
    {
    public: 
        jsonparser(): id(""), json(""){}       

        std::string TranslateTextJson(const std::string& key, const std::string& text);

        std::string TranslateJsonText(const std::string& key,const std::string& jsontext);

        void StringSorting(const std::string &message);

        std::string GetId(){return id;}
        std::string GetJson(){return json;}

    private:
        std::string id;
        std::string json;    

    };
} // namespace pars
