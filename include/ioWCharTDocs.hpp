#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <codecvt>
#include <functional>
#include <memory>

namespace tag
{
    struct wString {};
    struct vecWStr {};
    struct vecInt {};
    struct vecDouble {};
    struct mapWStrVar {};
}

struct variant;

union diff_type
{
    explicit diff_type(tag::wString) {new(&wStr_t) std::wstring();};
    explicit diff_type(tag::vecWStr) {new(&vecWStr_t) std::vector<std::wstring>();};
    explicit diff_type(tag::vecInt) {new(&vecInt_t) std::vector<int>();};
    explicit diff_type(tag::vecDouble) {new(&vecDouble_t) std::vector<double>();};
    explicit diff_type(tag::mapWStrVar) {new(&mapWStrDif_t) std::unordered_map<std::wstring, std::unique_ptr<variant>>();};
    explicit diff_type(int x) : int_t(x) {};
    explicit diff_type(bool x) : bool_t(x) {};
    explicit diff_type(double x) : double_t(x){};

    int                                                            int_t;
    double                                                         double_t;
    bool                                                           bool_t;
    std::wstring                                                   wStr_t;
    std::vector<std::wstring>                                      vecWStr_t;
    std::vector<int>                                               vecInt_t;
    std::vector<double>                                            vecDouble_t;
    std::unordered_map<std::wstring, std::unique_ptr<variant>>     mapWStrDif_t;

    ~diff_type(){};
};

struct variant
{
    explicit variant(tag::wString s) : ws_(true), dT_(s){};
    explicit variant(tag::vecWStr vws) : vecWStr_(true), dT_(vws){};
    explicit variant(tag::vecInt vi) : vecInt_(true), dT_(vi){};
    explicit variant(tag::vecDouble vD) : vecDouble_(true), dT_(vD){};
    explicit variant(tag::mapWStrVar mwsv) : mwsv_(true), dT_(mwsv){};
    explicit variant(int x) : dT_(x){};
    explicit variant(bool x) : dT_(x){};
    explicit variant(double x) : dT_(x){};


    variant& operator[] (const wchar_t* key) //для текущей программы
    {
        return *dT_.mapWStrDif_t[key];
    }

    std::wstring& operator[](const size_t& idx)
    {
        return dT_.vecWStr_t[idx];
    }

    std::wstring& get_wstring()
    {
        return dT_.wStr_t;
    }

    void set_wstring(const std::wstring& str)
    {
        dT_.wStr_t = str;
    }

    size_t map_size() const
    {
        return dT_.mapWStrDif_t.size();
    }

    std::unordered_map<std::wstring, std::unique_ptr<variant>>& get_map()
    {
        return dT_.mapWStrDif_t;
    }

    std::vector<std::wstring>& get_vector()
    {
        return dT_.vecWStr_t;
    }

    bool is_vecWStr() const
    {
        return vecWStr_;
    }

    bool is_map() const
    {
        return mwsv_;
    }

    bool is_wstring() const
    {
        return ws_;
    }

    bool is_empty_map() const
    {
        return dT_.mapWStrDif_t.empty();
    }


    ~variant()
    {
        if(ws_)
        {
            dT_.wStr_t.~basic_string<wchar_t>();
        }
        else if(vecWStr_)
        {
            dT_.vecWStr_t.~vector();
        }
        else if(vecInt_)
        {
            dT_.vecInt_t.~vector();
        }
        else if(vecDouble_)
        {
            dT_.vecDouble_t.~vector();
        }
        else if(mwsv_)
        {
            dT_.mapWStrDif_t.~unordered_map();
        }
        else
        {
            dT_.~diff_type();
        }
    }


    bool            ws_{false};
    bool            vecWStr_{false};
    bool            vecInt_{false};
    bool            vecDouble_{false};
    bool            mwsv_{false};

    diff_type       dT_;
};


class fileContainer
{
 public:
    fileContainer() : container() {};

   void fread(const std::string& filePath)
   {
       std::wifstream file(filePath, std::ios::binary);

       static std::locale empty;
       file.imbue(std::locale(empty, new std::codecvt_utf8_utf16<wchar_t>));

       std::wstringstream wss;
       wss << file.rdbuf();

       std::wstring buffer = wss.str();

       auto bufIt = buffer.begin();

       mapExtr(container, ++bufIt);
   }

   void fwrite(const std::string& filePath)
   {
       std::wofstream file(filePath, std::ios::binary);

       static std::locale empty;
       file.imbue(std::locale(empty, new std::codecvt_utf8_utf16<wchar_t>));

       std::wstring buffer{L"{\n"};
       size_t       spaceCount{2};
       size_t       idx{0};

       std::vector<std::unordered_map<std::wstring, std::unique_ptr<variant>>::iterator> it;
       std::vector<std::unordered_map<std::wstring, std::unique_ptr<variant>>::iterator> end;

       it.push_back(container.begin());
       end.push_back(container.end());

       while(it[0] != end[0])
       {
           if(it[idx] == end[idx])
           {
               spaceCount -= 2;
               --idx;
               ++it[idx];

               if(it[idx] != end[idx])
               {
                   for (size_t i{0}; i < spaceCount; ++i)
                   {
                       buffer += L" ";
                   }

                   buffer += L"},\n";
               }
               else
               {
                   for (size_t i{0}; i < spaceCount; ++i)
                   {
                       buffer += L" ";
                   }

                   buffer += L"}\n";
               }

               it.pop_back();
               end.pop_back();
           }
           else
           {
               for (size_t i{0}; i < spaceCount; ++i)
               {
                   buffer += L" ";
               }

               buffer += L"\"";
               buffer += it[idx]->first;
               buffer += L"\": ";

               if (it[idx]->second->is_map() && !it[idx]->second->is_empty_map()) {
                   it.push_back(it[idx]->second->get_map().begin());
                   end.push_back(it[idx]->second->get_map().end());
                   ++idx;

                   buffer += L"\n";

                   for (size_t i{0}; i < spaceCount; ++i) {
                       buffer += L" ";
                   }

                   buffer += L"{\n";

                   spaceCount += 2;
               }
               else if (it[idx]->second->is_map() && it[idx]->second->is_empty_map())
               {
                   buffer += L"{}";

                   if(++it[idx] != end[idx])
                   {
                       buffer += L",\n";
                   }
                   else
                   {
                       buffer += L"\n";
                   }
               }
               else if (it[idx]->second->is_wstring())
               {
                   buffer += L"\"";
                   buffer += it[idx]->second->get_wstring();
                   buffer += L"\"";

                   if(++it[idx] != end[idx])
                   {
                       buffer += L",\n";
                   }
                   else
                   {
                       buffer += L"\n";
                   }
               }
           }
       }

       buffer += L"}";

       file << buffer;
       file.close();
   }

   variant& operator[](const wchar_t* key)
   {
       return *container[key];
   }

   std::unordered_map<std::wstring, std::unique_ptr<variant>>::iterator find(const std::wstring& key)
   {

       typedef std::unordered_map<std::wstring, std::unique_ptr<variant>>&             mapRef_t;
       typedef std::unordered_map<std::wstring, std::unique_ptr<variant>>::iterator    mapIt_t;
       bool isFind{false};

       std::function<mapIt_t(mapRef_t)> finder = [&key, &finder, &isFind](mapRef_t mRef)
       {
           mapIt_t findIt{mRef.find(key)};
           if(findIt != mRef.end())
           {
               isFind = true;
               return findIt;
           }

           for(auto&& it : mRef)
           {
               if(it.second->is_map()) return finder(it.second->get_map());
               if(isFind) break;
           }
       };

       return finder(container);
   }


   std::unordered_map<std::wstring, std::unique_ptr<variant>>::iterator end()
   {
       return container.end();
   }

    std::unordered_map<std::wstring, std::unique_ptr<variant>>::iterator begin()
    {
       return container.begin();
    }

    std::unordered_map<std::wstring, std::unique_ptr<variant>>& get()
    {
       return container;
    }

private:
   std::unordered_map<std::wstring, std::unique_ptr<variant>> container;

private:
    void vecExtr(std::unique_ptr<variant>& ptr, std::wstring::iterator& bufIt)
    {
        bool vecHasMake{false};
        std::wstring buf;
        while(*bufIt != L']')
        {
            if(*bufIt == L'\"')
            {
                if(!vecHasMake)
                {
                    tag::vecWStr vs;
                    ptr = std::make_unique<variant>(vs);
                    vecHasMake = true;
                }
                ++bufIt;
                while(*bufIt != L'\"')
                {
                    buf += *bufIt;
                    ++bufIt;
                }
                ++bufIt;
                ptr->dT_.vecWStr_t.push_back(buf);
                buf.clear();
            }
            else if(*bufIt >= L'0' && *bufIt <= L'9')
            {
                if(!vecHasMake)
                {
                    tag::vecInt vi;
                    ptr = std::make_unique<variant>(vi);
                    vecHasMake = true;
                }
                while(*bufIt >= L'0' && *bufIt <= L'9')
                {
                    buf += *bufIt;
                    ++bufIt;
                }
                ptr->dT_.vecInt_t.push_back(std::stoi(buf));
                buf.clear();
            }
            else
            {
                ++bufIt;
            }
        }
    }

    void mapExtr (std::unordered_map<std::wstring, std::unique_ptr<variant>>& container_, std::wstring::iterator& bufIt)
    {
        bool isKey{true};
        std::wstring key;
        std::wstring buf;
        while (*bufIt != L'}')
        {
            if (*bufIt == L'\"' && isKey)
            {
                ++bufIt;
                while (*bufIt != L'\"')
                {
                    buf += *bufIt;
                    ++bufIt;
                }
                ++bufIt;
                key = buf;
                container_.insert(std::pair<std::wstring, std::unique_ptr<variant>>(key, nullptr));
                buf.clear();
            }
            else if (*bufIt == L'\"' && !isKey)
            {
                ++bufIt;
                while (*bufIt != L'\"')
                {
                    buf += *bufIt;
                    ++bufIt;
                }
                ++bufIt;
                tag::wString ws;
                container_[key] = std::make_unique<variant>(ws);
                container_[key]->dT_.wStr_t = buf;
                buf.clear();
            }
            else if (*bufIt >= L'0' && *bufIt <= L'9' && !isKey)
            {
                while (*bufIt >= L'0' && *bufIt <= L'9')
                {
                    buf += *bufIt;
                    ++bufIt;
                }
                container_[key] = std::make_unique<variant>(std::stoi(buf));
                buf.clear();
            }
            else if (*bufIt == L'{')
            {
                tag::mapWStrVar msv;
                container_[key] = std::make_unique<variant>(msv);
                mapExtr(container_[key]->dT_.mapWStrDif_t, ++bufIt); //;
                ++bufIt;
            }
            else if(*bufIt == L'[')
            {
                vecExtr(container_[key], ++bufIt);
                ++bufIt;
            }
            else if (*bufIt == L':')
            {
                isKey = false;
                ++bufIt;
            }
            else if (*bufIt == L',')
            {
                isKey = true;
                ++bufIt;
            }
            else
            {
                ++bufIt;
            }
        }
    }
};
