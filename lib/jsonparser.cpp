#include "logging.h"

#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <iostream>
#include <string>
#include <boost/json.hpp>
#include <boost/json/src.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "jsonparser.h"

using namespace boost::json;
 namespace Serf
 {   

    std::string jsonparser::TranslateTextJson(const std::string& key, const std::string& text)
    {
        LOGGING_SOURCES(normal, "Создаем json текст.");
        object obj;        
        obj[key] = text;
        // Преобразуем в string
        return  boost::lexical_cast<std::string>(obj);
    }

    std::string jsonparser::TranslateJsonText(const std::string& key,const std::string& jsontext)
    {
        
        std::stringstream jsomEncoder(jsontext);
        boost::property_tree::ptree root;
        boost::property_tree::read_json(jsomEncoder, root);

        if(root.empty()) return "";
        LOGGING_SOURCES(normal, "Распарсили json в текст " + root.get<std::string>(key));
        return root.get<std::string>(key);
    }

    void jsonparser::StringSorting(const std::string &message)
    {     
        id = ""; 
        json = "";        
        int counter = 0;
        bool flag = true;
        for (auto const &item : message)
        {
            if(flag){    
                if (item == ':'){ 
                    if(counter > 0){
                        flag = false;
                    }else{
                    id += item;
                    counter++;
                    }
                }else{
                id += item;   
                } 
            } else{
                if(item !=' ') json += item;
            }      
        }
        LOGGING_SOURCES(normal, "Распарсил текст на IDname: " + id + " и json: " + json);  
    }

    

 }// namespace pars

