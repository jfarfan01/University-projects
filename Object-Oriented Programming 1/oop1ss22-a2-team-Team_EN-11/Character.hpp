//---------------------------------------------------------------------------------------------------------------------
// Character.hpp
//
// Class of playable character
// 
//
// Author: 11942956, 11812041, 12130995
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef CHARACTER_HPP
#define CHARACTER_HPP



enum CharacterType 
{ 
  NoCharacter = 0, 
  FighterCharacter = 1, 
  ThiefCharacter = 2, 
  SeerCharacter = 3 
  };

class Character
{
  private:
    CharacterType type_; // need to consider if this is more practical than just having it be F, T, S
    

  public:

    //---------------------------------------------------------------------------------------------------------------------
    /// Constructor
    Character(CharacterType type) : type_{type} {};
    
    
    //---------------------------------------------------------------------------------------------------------------------
    /// Getter for character type, returns Charactertype
    CharacterType getCharacterType() 
    {
      return type_;
    }
    
};
#endif

