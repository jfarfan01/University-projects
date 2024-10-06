#include "User.hpp"
#include "Config.hpp"

// #include <map>
// #include <typeinfo>


  bool User::verifyPassword(const std::string& password) const
  {
    if (password_ == password)
    {
      return true;
    } 
    else
    {
      return false;
    }
  }

  bool User::hasContact(const string& name) const 
  {  
    //contacts_.insert(std::pair<User*, Cipher*> (&myUser, &myCipher));
    //contacts_.count(name);
   
    for(auto iterator = contacts_.begin(); iterator != contacts_.end(); ++iterator) 
    {
      // std::cout << "Key: " << iterator->first->name_ << std::endl;
      // std::cout << "Value: " << iterator->second << std::endl;
      if (name == iterator->first->name_) 
      {
        return true;
      }
    }
    return false;
  }


  void User::addContact(User* contact, CipherType shit, Key key)
  {
    Cipher* ciph = nullptr;
    switch (shit)
      {
        case CipherType::ASCII_CIPHER:
          ciph = new AsciiCipher(key);
          break;
        case CipherType::CAESAR_CIPHER:
          // cout << "this should definitely happen" << endl;
          ciph = new CaesarCipher(key);
          break;
        case CipherType::NONE_CIPHER:
          // cout << "fuck you little dumb bitch ass fuck" << endl;
          ciph = new NoneCipher(key);
        default:

          break;
  }
    contacts_.insert(std::pair<User*, Cipher*> (contact, ciph));
  }

  Cipher* User::getCipherForContact(const std::string& name) const
  {

    // cout << "my name is " << name_ << "and I have " << contacts_.size() << " friends" << endl;
  
    Cipher* temp;
    for(auto iterator = contacts_.begin(); iterator != contacts_.end(); ++iterator) 
    {
      // std::cout << "Key: " << iterator->first->name_ << std::endl;
      // std::cout << "Value: " << iterator->second << std::endl;
      if (name == iterator->first->name_) 
      {
        temp = iterator -> second;
        // cout << "name is " << name << " and cipher* is " << temp << endl;
        return temp;
      }
    }
    return nullptr;
  }

    


  bool User::sendMessage(string& recipient, string& filename, const string& plain_text) const
  {
    std::ofstream file;
    file.open (filename);
    if (!file.is_open())
    {
      return false;
    }


    if (!this->hasContact(recipient))
    {
      return false;
    }


    //get object
    Cipher* cipher_type = this-> getCipherForContact(recipient);

    //AsciiCipher::encrypt(plain_text);
    string encrypted_text= cipher_type->encrypt(plain_text);


    file << "Recipient: "<< recipient<<std::endl<<"Sender: "<<name_<<std::endl<<encrypted_text<<endl;
    // IO::printEncryptedMessage(encrypted_text);
    cout<<endl<<"Sending ..."<<endl<<"Recipient: "<<recipient<<endl<<"Sender: "<<name_<<endl<<encrypted_text<<endl;
    file.close();
    return true;

  }

  bool User::readMessage(const std::string& filename) const
  {
    // cout << endl <<  "Reading ..." << endl;
    std::ifstream file;
    file.open (filename);
    if (!file.is_open())
    {
      return false;
    }

    int c = 0;
    string word, recipient,sender,encrypted_text;
    while(file >> word)
    {
      ++c;
      if (c ==2)
      {
        recipient = word;
        // cout << "rec: " << recipient << endl;
      }
      if(c == 4)
      {
        sender = word;
        // cout << "send: " << sender << endl;
      }
      if (c == 5)
      {
        encrypted_text = word;
        // cout << "enc" << encrypted_text << endl;
      }
    }

    if (name_ != recipient)
    {
      return false;
    }

    // cout << "passed the first sanity check " << endl;
    if (!(this->hasContact(sender)))
    {
      return false;
    }



    //cout << "passed some sanity checks" << endl;
    //find out cipher type and print
    Cipher* cipher_type = this-> getCipherForContact(sender);

    // cout << "got here" <<endl;
    // cout << typeid(cipher_type).name() << endl;

    if (cipher_type == nullptr)
    {
      // cout << "cipher is null" << endl;
      return false;
    }


    string decrypted_text= cipher_type->decrypt(encrypted_text);

     //cout << "now did we get here though??" << endl;

    IO::printDecryptedMessage(recipient, sender, decrypted_text);

  


    return true;
  }

 
