

#ifndef TEXTADV_STRINGS_H
#define TEXTADV_STRINGS_H


#include <string>

//room information
const std::string r1name = "Room 1";
const std::string r1desc = "You are in room 1. It's really quite boring, but then, it's just for testing really. There are passages to any direction.";
const std::string r2name = "Blue Room";
const std::string r2desc = "You are the blue room. You know because it's blue. That's about all though. There's a passage to the south.";
const std::string r3name = "Room 542";
const std::string r3desc = "You are in room 542. It's chaotic and humid, with messy wires everywhere. No one knows what's happening here. There's a passage to the west.";
const std::string r4name = "Room 102";
const std::string r4desc = "You are in room 102. The air here is dirty and stuffy, you just want to leave quickly. There's a passage to the north.";
const std::string r5name = "Room 109";
const std::string r5desc = "You are in room 109. It's a multimedia classroom, but its multimedia equipment is broken. There's a passage to the east.";

//object information
const std::string o1name = "Golden apple";
const std::string o1desc = "The shining but pixelated apple may have come from a well-known game. Effect: strength + 10";
const std::string o1key = "f:golden_apple";
const std::string o2name = "Honey cake";
const std::string o2desc = "A derivative product of a yellow bear's favorite food that looks delicious. Effect: strength + 5";
const std::string o2key = "f:honey_cake";
const std::string o3name = "Brownie";
const std::string o3desc = "A brownie made by Alice of which the dough has a moist taste but is not sticky, with a refreshing sweet taste. Effect: strength + 8";
const std::string o3key = "f:brownie";
const std::string o4name = "Sugar Rush";
const std::string o4desc = "The alcohol content of a sugar rush varies from bartender to bartender, but the one in front of you is the non-alcoholic version. Effect: strength + 6";
const std::string o4key = "f:sugar_rush";
const std::string o5name = "AK-47";
const std::string o5desc = "A rusty assault rifle. You can vaguely see a small line written on it: Made in Wa state";
const std::string o5key = "t:ak47";
const std::string o6name = "Single piece scissor";
const std::string o6desc = "A huge, red, only one piece of scissor, you want to know where the other piece is.";
const std::string o6key = "t:single_scissor";
const std::string o7name = "Melon Knife";
const std::string o7desc = "There seems to be no regulation on the shape of the watermelon knife, but everyone knows that it is used to chop watermelons. ";
const std::string o7key = "t:melon_knife";

//command message
const std::string badExit = "You can't go that way.";
const std::string badCommand = "I don't understand that.";

//game message
const std::string roomObjectsMessage = "Object(s) in this room: ";
const std::string inventoryObjectsMessage = "Object(s) in the inventory: ";
const std::string pickObjectMessage0 = "Picked successfully.";
const std::string pickObjectMessage1 = "This object is already in the inventory.";
const std::string pickObjectMessage2 = "Cannot find that object in this room.";
const std::string objectNotFound = "This object doesn't exist.";
const std::string dropObjectMessage0 = "Dropped successfully.";
const std::string dropObjectMessage1 = "This object is already in the room.";
const std::string dropObjectMessage2 = "Cannot find that object in this inventory.";
const std::string terminateMessage0 = "Your strength has run over.";
const std::string eatFoodMessage0 = "You have eaten this food.";
const std::string eatFoodMessage1 = "Cannot find that thing in the inventory.";
const std::string eatFoodMessage2 = "You can't eat that";


#endif //TEXTADV_STRINGS_H
