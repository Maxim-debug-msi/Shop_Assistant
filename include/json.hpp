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
    explicit variant(const tag::wString& s) : ws_(true), dT_(s){};
    explicit variant(const tag::vecWStr& vws) : vecWStr_(true), dT_(vws){};
    explicit variant(const tag::vecInt& vi) : vecInt_(true), dT_(vi){};
    explicit variant(const tag::vecDouble& vD) : vecDouble_(true), dT_(vD){};
    explicit variant(const tag::mapWStrVar& mwsv) : mwsv_(true), dT_(mwsv){};
    explicit variant(const int& x) : int_(true), dT_(x){};
    explicit variant(const bool& x) : dT_(x){};
    explicit variant(const double& x) : dT_(x){};

    variant(const variant& oth) : dT_(oth.dT_){
        this->mwsv_ = oth.mwsv_;
        this->vecWStr_ = oth.vecWStr_;
        this->vecInt_ = oth.vecInt_;
        this->vecDouble_ = oth.vecDouble_;
        this->ws_ = oth.ws_;
    }

    std::wstring& wstring()
    {
        return dT_.wStr_t;
    };

    std::unordered_map<std::wstring, variant*>& map()
    {
        return dT_.mapWStrDif_t;
    };

    std::vector<std::wstring>& vector(const tag::vecWStr& vecWStr_tag)
    {
        return dT_.vecWStr_t;
    };

    std::vector<int>& vector(const tag::vecInt& vecInt_tag)
    {
        return dT_.vecInt_t;
    }

    std::vector<double>& vector(const tag::vecDouble& vecDouble_tag)
    {
        return dT_.vecDouble_t;
    }

    int& integer()
    {
        return dT_.int_t;
    }

    double& real()
    {
        return dT_.double_t;
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

    bool is_vecInt() const
    {
        return vecInt_;
    }

    bool is_vecDouble() const
    {
        return vecDouble_;
    }

    bool is_int()
    {
        return int_;
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

private:
    bool            ws_{false};
    bool            vecWStr_{false};
    bool            vecInt_{false};
    bool            vecDouble_{false};
    bool            mwsv_{false};
    bool            int_{false};
    bool            double_{false};
    bool            bool_{false};

    diff_type       dT_;
};


class json final
{
 public:
   json() : container() {};

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

       mapExtract(container, ++bufIt);
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

               if (it[idx]->second->is_map() && !it[idx]->second->map().empty()) {
                   it.push_back(it[idx]->second->map().begin());
                   end.push_back(it[idx]->second->map().end());
                   ++idx;

                   buffer += L"\n";

                   for (size_t i{0}; i < spaceCount; ++i) {
                       buffer += L" ";
                   }

                   buffer += L"{\n";

                   spaceCount += 2;
               }
               else if (it[idx]->second->is_map() && it[idx]->second->map().empty())
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
                   buffer += it[idx]->second->wstring();
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
                   if (it[idx]->second->vector(tag::vecWStr()).empty())
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
                       buffer += L"\n";

                       for (size_t i{0}; i < spaceCount; ++i) {
                           buffer += L" ";
                       }

                       buffer += L"[\n";

                       spaceCount += 2;

                       for (size_t b{0}, e{it[idx]->second->vector(tag::vecWStr()).size()}; b < e; ++b) {

                           for (size_t i{0}; i < spaceCount; ++i) {
                               buffer += L" ";
                           }

                           buffer += L"\"";
                           buffer += it[idx]->second->vector(tag::vecWStr())[b];
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
                       else
                       {
                           buffer += L"\n";////
                       }
                   }
               }
               else if(it[idx]->second->is_vecInt())
               {
                   if (it[idx]->second->vector(tag::vecInt()).empty())
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
                       buffer += L"\n";

                       for (size_t i{0}; i < spaceCount; ++i) {
                           buffer += L" ";
                       }

                       buffer += L"[\n";

                       spaceCount += 2;

                       for (size_t b{0}, e{it[idx]->second->vector(tag::vecInt()).size()}; b < e; ++b) {

                           for (size_t i{0}; i < spaceCount; ++i) {
                               buffer += L" ";
                           }

                           buffer += std::to_wstring(it[idx]->second->vector(tag::vecInt())[b]);

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
                       else
                       {
                           buffer += L"\n";
                       }
                   }
               }
               else if(it[idx]->second->is_vecDouble())
               {
                   if (it[idx]->second->vector(tag::vecDouble()).empty())
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
                       buffer += L"\n";

                       for (size_t i{0}; i < spaceCount; ++i) {
                           buffer += L" ";
                       }

                       buffer += L"[\n";

                       spaceCount += 2;

                       for (size_t b{0}, e{it[idx]->second->vector(tag::vecDouble()).size()}; b < e; ++b) {

                           for (size_t i{0}; i < spaceCount; ++i) {
                               buffer += L" ";
                           }

                           buffer += std::to_wstring(it[idx]->second->vector(tag::vecDouble())[b]);

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
                       else
                       {
                           buffer += L"\n";
                       }
                   }
               }
               else if(it[idx]->second->is_int())
               {
                   buffer += std::to_wstring(it[idx]->second->integer());

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
                           finder(it.second->map());
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
    void vecExtract(variant* &ptr, std::wstring::iterator& bufIt)
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
                ptr->vector(tag::vecWStr()).push_back(buf);
                buf.clear();
            }
            else if(*bufIt >= L'0' && *bufIt <= L'9')
            {
                int isDouble{0};

                while((*bufIt >= L'0' && *bufIt <= L'9') || *bufIt == L'.')
                {
                    if(*bufIt == L'.')
                    {
                        isDouble = 1;
                    }
                    buf += *bufIt;
                    ++bufIt;
                }

                if(!vecHasMake)
                {
                    switch (isDouble)
                    {
                        case 0:
                        {
                            tag::vecInt vi;
                            ptr = new variant(vi);
                            ptr->vector(tag::vecInt()).push_back(std::stoi(buf));
                            break;
                        }
                        case 1:
                        {
                            tag::vecDouble vd;
                            ptr = new variant(vd);
                            ptr->vector(tag::vecDouble()).push_back(std::stod(buf));
                            break;
                        }
                        default:
                        {
                        }
                    }
                    vecHasMake = true;
                    buf.clear();
                }
            }
            else
            {
                ++bufIt;
            }
        }

        if(!vecHasMake)
        {
            ptr = new variant(tag::vecInt());
        }
    }

    void mapExtract (std::unordered_map<std::wstring, variant*>& container_, std::wstring::iterator& bufIt)
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
                container_[key] = new variant(tag::wString());
                container_[key]->wstring() = buf;
                buf.clear();
            }
            else if (*bufIt >= L'0' && *bufIt <= L'9' && !isKey)
            {
                int isDouble{0};

                while ((*bufIt >= L'0' && *bufIt <= L'9') || *bufIt == L'.')
                {
                    if(*bufIt == L'.')
                    {
                        isDouble = 1;
                    }
                    buf += *bufIt;
                    ++bufIt;
                }

                switch (isDouble)
                {
                    case 0:
                    {
                        container_[key] = new variant(std::stoi(buf));
                        break;
                    }
                    case 1:
                    {
                        container_[key] = new variant(std::stod(buf));
                        break;
                    }
                    default:
                    {
                    }
                }
                buf.clear();
            }
            else if (*bufIt == L'{')
            {
                container_[key] = new variant(tag::mapWStrVar());
                mapExtract(container_[key]->map(), ++bufIt); //;
                ++bufIt;
            }
            else if(*bufIt == L'[')
            {
                vecExtract(container_[key], ++bufIt);
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
