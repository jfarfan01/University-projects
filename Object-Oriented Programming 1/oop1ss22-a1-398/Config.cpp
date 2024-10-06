#include "Config.hpp"
#include "Utils.hpp"



bool Config::validateUnPw(const std::string& string) const
{
  return string.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890") ==
  std::string::npos;
}

bool Config::verifyCipher(const string& cy_string) const
{
  if (cy_string == "NONE" || cy_string == "ASCII" || cy_string == "CAESAR")
  {
      return true;
  }

  else
  {
    return false;
  }
}


bool Config::parseFile() 
{
  std::ifstream file(filename_);
  string word;
  bool a = true;
  std::vector<string> words;
  std::getline(file, word);

 
  while(a)
  {
    // std::getline(file, word);
    size_t index = word.find_first_of(";:,");
    string temp = (word.substr(0,index+1));
    if (index ==string::npos)
    {
      return true;
    }
    temp.pop_back();
    words.push_back(temp);
    word.erase(0, index+1);
    //cout<<temp<<endl;

    if (word.length() == 0)
    {
      string name, pw;
      int  c = 0;
      for (string s: words)
      {
        c++;
        //s.pop_back();
        //cout<< c<<": "<<s<<endl;
        
        if (c == 1)
        {
          name =s;
        }

        if (c==2)
        {
          pw = s;
        }
      }
      if(!this->validateUnPw(name)|| !this->validateUnPw(pw))
      {
        return false;
      }


        //cout<<name<<" sdfsd "<<pw<<endl;
      if(name.empty())
      {
        return true;
      }      
      if (this->containsUser(name))
      {
        return false;
      }

      User* uptr =new  User(name, pw);


      User user(name, pw);
      users_.push_back(uptr);
      std::getline(file, word);
      words.clear();
      //delete(uptr);
    }

    if (file.eof())
    {
      break;
    }
 
  }

  // for (auto s: users_)
  // {
  //   cout<< s->getName()<<" pw: "<<s->getPassword()<< endl;
  // }
  bool b = true;
  file.clear();  
  file.seekg(0, std::ios::beg);
  //int counter = 1;
  std::getline(file, word);
  int getlineCounter = 0;

    while(b)
  {
    // std::getline(file, word);
    size_t index = word.find_first_of(";:,");
    string temp = (word.substr(0,index+1));
    if (index ==string::npos)
    {
      return true;
    }
    temp.pop_back();
    words.push_back(temp);
    word.erase(0, index+1);
    //cout<<temp<<endl;

    if (word.length() == 0)
    {
      int  c = 0;
      int internal_counter = 0;
      for (string s: words)
      {
        User* uptr ;
        c++;
        if (c == 1 )
        {
          uptr = this->getUser(s);
          //cout<<"have we? "<< uptr->getName()<<endl;
          if (uptr->hasContact(s))
          {
            return false;
          }
        }
        if(c > 2)
        {
          internal_counter++;
        }
        if (internal_counter > 3)
        {
          internal_counter = 1;
        }
        User* uptr2 ;
        if (internal_counter ==1)
        {
          uptr2 = this->getUser(s);
          //cout<<"contact "<< uptr2->getName()<<endl;
        }
        // if (uptr->getName()== uptr2->getName())
        // {
        //   return false;
        // }
   


        //int number = 0;
        string cipher_type;
        string caesar="CAESAR";
        CipherType cipher_enum;
        // cout << static_cast<std::underlying_type<CipherType>::type>(cipher_enum) << "<- this sbhould always be 1" << endl;
         if (internal_counter ==2)
        { 
          cipher_type =s;       
          cipher_enum = Cipher::getCipherType(cipher_type);
        }
        // cout << static_cast<std::underlying_type<CipherType>::type>(cipher_enum) << "<- this should sometimes be something other than 1"<< endl;
        //cout<<"cipehr string "<<cipher_type<<endl;
        // if (cipher_enum== CipherType::CAESAR_CIPHER)
        // {
        //   cout << "ceasar bro " << uptr2->getName() << endl;
        //   cout << static_cast<std::underlying_type<CipherType>::type>(cipher_enum) << "this should definitely not be 1" << endl;
        // }
      

        uint64_t cy_key;
        if (internal_counter ==3)
        {
      //     //cipher_enum = Cip herType::ASCII_CIPHER;
      // cout << " before calling addcontact the fucking thing is of type" <<endl; 
      // cout << static_cast<std::underlying_type<CipherType>::type>(cipher_enum) << endl;
          if (!Utils::convertHexTo64BitNumber(s,cy_key))
          {
            return false;
          }


          uptr->addContact(uptr2,cipher_enum, cy_key);
        }
        //uptr->addContact(uptr2,cipher_enum, cy_key);

        //cout<<internal_counter<<": "<<s<<endl;
        
      }
 
      
      std::getline(file, word);
      getlineCounter++;
      words.clear();
      //delete(uptr);
    }

    for (User* u: users_)
    {
      if (u->hasContact(u->getName()))
      {
        return false;
      }
    }

    if (file.eof())
    {
      break;
    }

    //all contacts are users
    for (User* u: users_)
    {     
      for(auto iterator = u->getContacts().begin(); iterator != u->getContacts().end(); ++iterator)
      {
        if(!this->containsUser(iterator->first->getName()))
        {
          return false;
        }      
      }
    }
 
    //no repeated contacts
    //  for (User* u: users_)
    // {     
    //   int counter = 0;
    //   for(auto iterator = u->getContacts().begin(); iterator != u->getContacts().end(); ++iterator) 
    //   {
    //     // std::cout << "Key: " << iterator->first->name_ << std::endl;
    //     // std::cout << "Value: " << iterator->second << std::endl;
    //     if (u->getName() == iterator->first->getName()) 
    //     {
    //       counter ++;
    //     }
    //     if (counter > 1)
    //     {
    //       return false;
    //     }
    //   }
    // }

    //symmetry
    // for (User* u: users_)
    // {     
    //   for(auto const& [contact, cipher] : u->getContacts())
    //   {
    //     if(!contact->hasContact(u->getName()))
    //     {
    //       return false;
    //     }
    //   }    
    // }

  }
 



  return true;
}

bool Config::containsUser(const std::string& name) const
{
     for(auto iter = users_.begin(); iter != users_.end(); iter++)
    {

      if (name == (*iter)->getName()) 
      {
        return true;
      }
    }
    return false;
}

User* Config::getUser(const std::string& name) const
{ 
  User* user;
  //std::vector<User*>::iterator ite;

  for(auto iter = users_.begin(); iter != users_.end(); iter++)
  {
    if (name == (*iter)->getName()) 
    {
      user = *iter;
      return user;
    }
  }
  return nullptr;
}


User* Config::registerUser(const std::string& name, const std::string& password)
{
  // User* user_ptr;
  // User user(name,password);
  // user_ptr = &user;
  User* uptr =new  User(name, password);

  users_.push_back(uptr);
  this->setConfigModified();
  return uptr;

}

User* Config::loginUser(const std::string& name, const std::string& password) const
{
  User* user_ptr = this ->getUser(name);
  if (user_ptr->verifyPassword(password))
  {
    return user_ptr;
  }
  else 
  {
    return nullptr;
  }
}


  bool Config::updateConfigFile()
  {
    std::ofstream file;
    file.open (filename_);
    for (User* u: users_)
    {
      file << u->getName()<<";"<<u->getPassword()<<";";
      
      for(auto iterator = u->getContacts().begin(); iterator != u->getContacts().end(); ++iterator)
      {
        file  << iterator->first->getName();
        file << ":" << iterator->second->getCipherTypeString()<<",";
        file <<std::setfill('0') << std::setw(16)<<std::hex <<iterator->second->getKey()<<";";
      }
      file<<endl;
    }
    
    file.close();
    
    return true;
  }

