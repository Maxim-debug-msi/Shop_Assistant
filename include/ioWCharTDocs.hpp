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
#include <regex>

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

    diff_type(const diff_type& oth)
    {
        this->mapWStrDif_t = oth.mapWStrDif_t;
        this->wStr_t = oth.wStr_t;
        this->vecWStr_t = oth.vecWStr_t;
        this->vecInt_t = oth.vecInt_t;
        this->vecDouble_t = oth.vecDouble_t;
        this->bool_t = oth.bool_t;
        this->int_t = oth.int_t;
        this->double_t = oth.double_t;
    };

    int                                                            int_t;
    double                                                         double_t;
    bool                                                           bool_t;
    std::wstring                                                   wStr_t;
    std::vector<std::wstring>                                      vecWStr_t;
    std::vector<int>                                               vecInt_t;
    std::vector<double>                                            vecDouble_t;
    std::unordered_map<std::wstring, variant*>                     mapWStrDif_t;

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

    variant(const variant& oth) : dT_(oth.dT_){
        this->mwsv_ = oth.mwsv_;
        this->vecWStr_ = oth.vecWStr_;
        this->vecInt_ = oth.vecInt_;
        this->vecDouble_ = oth.vecDouble_;
        this->ws_ = oth.ws_;
    }


    variant* operator[] (const wchar_t* key) //для текущей программы
    {
        return dT_.mapWStrDif_t[key];
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

    std::unordered_map<std::wstring, variant*>& get_map()
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

    void incWStr()
    {
        std::wstring::iterator it  = dT_.wStr_t.end() - 1;

        while(true)
        {
            if (*it == L'9') {
                *it = L'0';
                --it;
            } else {
                ++(*it);
                break;
            }
        }
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
       std::wstring buffer;
       wss << file.rdbuf();
       buffer = wss.str();

       if(buffer.empty())
       {
           buffer = L"{}";
       }

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

       std::vector<std::unordered_map<std::wstring, variant*>::iterator> it;
       std::vector<std::unordered_map<std::wstring, variant*>::iterator> end;

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
               else if(it[idx]->second->is_vecWStr())
               {
                   if (it[idx]->second->get_vector().empty())
                   {
                        buffer += L"[]";

                       if(++it[idx] != end[idx])
                       {
                           buffer += L",\n";
                       }
                       else
                       {
                           buffer += L"\n";
                       }
                   }
                   else
                   {
                       buffer += L"\n[\n";

                       spaceCount += 2;

                       for (size_t b{0}, e{it[idx]->second->get_vector().size()}; b < e; ++b) {

                           for (size_t i{0}; i < spaceCount; ++i) {
                               buffer += L" ";
                           }

                           buffer += L"\"";
                           buffer += it[idx]->second->get_vector()[b];
                           buffer += L"\"";

                           if (b + 1 < e) {
                               buffer += L",\n";
                           } else {
                               buffer += L"\n";
                           }
                       }
                       spaceCount -= 2;

                       for (size_t i{0}; i < spaceCount; ++i) {
                           buffer += L" ";
                       }

                       buffer += L"]";

                       if(++it[idx] != end[idx])
                       {
                           buffer += L",\n";
                       }
                   }
               }
           }
       }

       buffer += L"}";

       file << buffer;
       file.close();
   }

   variant* operator[](const std::wstring& key)
   {
       return container[key];
   }

   std::unordered_map<std::wstring, variant*>::iterator search(const std::wstring& key)
   {

       typedef std::unordered_map<std::wstring, variant*>&             mapRef_t;
       typedef std::unordered_map<std::wstring, variant*>::iterator    mapIt_t;

       std::unordered_map<std::wstring, variant*>::iterator result;
       bool isFind{false};
/*
       size_t       idx{0};

       std::vector<std::unordered_map<std::wstring, variant*>*> mapPtr;
       std::vector<std::unordered_map<std::wstring, variant*>::iterator> it;
       std::vector<std::unordered_map<std::wstring, variant*>::iterator> end;

       mapPtr.push_back(&container);
       it.push_back(container.begin());
       end.push_back(container.end());

       while(it[0] != end[0])
       {
           it[idx] = mapPtr[idx]->find(key);
           if(it[idx] != end[idx])
           {
               return it[idx];
           }
           else
           {
               for(auto&& map : *mapPtr[idx])
               {
                   if(map.second->is_map() && !map.second->is_empty_map())
                   {
                       mapPtr.push_back(&map.second->get_map());
                       ++idx;
                   }
               }
           }

           if(it[idx]->second->is_map() && !it[idx]->second->is_empty_map())
           {
               it.push_back(it[idx]->second->get_map().begin());
               end.push_back(it[idx]->second->get_map().end());
               ++idx;
           }

           if(it[idx] == end[idx])
           {
               it.pop_back();
               end.pop_back();
               --idx;
               ++it[idx];
           }
           else
           {
               ++it[idx];
           }
       }

       return container.end();
*/

       std::function<void(mapRef_t)> finder = [&key, &finder, &isFind, &result](mapRef_t mRef) -> auto
       {
           if(isFind)
           {
               return;
           }
           else
           {
               mapIt_t findIt{mRef.find(key)};
               if (findIt != mRef.end())
               {
                   result = findIt;
                   isFind = true;
               }
               else
               {
                   for (auto &&it: mRef)
                   {
                       if (it.second->is_map() && !isFind)
                       {
                           finder(it.second->get_map());
                       }
                       else if(isFind) return;
                   }
               }
           }
       };

       finder(container);

       if(isFind) return result;
       else return container.end();
   }

   void insert(const std::pair<std::wstring, variant*>& pair)
   {
       container.insert(pair);
   }

   std::unordered_map<std::wstring, variant*>::iterator end()
   {
       return container.end();
   }

    std::unordered_map<std::wstring, variant*>::iterator begin()
    {
       return container.begin();
    }

    void erase(const std::wstring& key)
    {
       container.erase(key);
    }

private:
   std::unordered_map<std::wstring, variant*> container;

private:
    void vecExtr(variant* &ptr, std::wstring::iterator& bufIt)
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
                    ptr = new variant(vs);
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
                    ptr = new variant(vi);
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

        if(!vecHasMake)
        {
            tag::vecWStr vws;
            ptr = new variant(vws);
        }
    }

    void mapExtr (std::unordered_map<std::wstring, variant*>& container_, std::wstring::iterator& bufIt)
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
                container_.insert(std::pair<std::wstring, variant*>(key, nullptr));
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
                container_[key] = new variant(ws);
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
                container_[key] = new variant(std::stoi(buf));
                buf.clear();
            }
            else if (*bufIt == L'{')
            {
                tag::mapWStrVar msv;
                container_[key] = new variant(msv);
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
