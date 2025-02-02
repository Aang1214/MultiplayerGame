/*
Jack Worthington - D00251921
Keven Vokin		 - D00251324
Marek Martinak	 - D00250456
*/

#include "CommandQueue.hpp"

void CommandQueue::Push(const Command& command)
{
    m_queue.push(command);
}

Command CommandQueue::Pop()
{
    Command command = m_queue.front();
    m_queue.pop();
    return command;
}

bool CommandQueue::IsEmpty() const
{
    return m_queue.empty();
}
