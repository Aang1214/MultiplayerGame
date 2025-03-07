/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include "Command.hpp"

Command::Command() : action(), category(static_cast<unsigned int>(ReceiverCategories::kNone))
{
}
