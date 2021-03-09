/*
* File: stack.h
*
* Author: Caleb Davis
*
* Date: 03-07-21
*
* Description:
*   Basic template stand-alone class implementation for a stack.
*
*/

#ifndef STACK_H
#define STACK_H

template <typename data_type>
class stack
{
public:
	// default constructor
	stack()
		: head(nullptr), curr(nullptr), tail(nullptr)
	{
	}

	// check if stack is empty
	bool empty()
	{
		if (head == nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// return size of stack
	data_type size()
	{
		if (empty())
		{
			return 0;
		}
		else
		{
			// create counter
			int count = 0;

			// begin curr at head
			curr = head;

			// loop through stack incrementing counter
			while (curr != nullptr)
			{
				// increment counter
				count++;

				// go to next item
				curr = curr->next;
			}

			// return counter
			return count;
		}
	}

	// return reference to top of stack (back most item)
	data_type& top()
	{
		return tail->data;
	}

	// push item onto back of stack
	void push(data_type new_data)
	{
		// create new node
		curr = new node;

		// store provided data in new node
		curr->data = new_data;

		// make next nullptr, since node will be new tail
		curr->next = nullptr;

		if (empty()) // first node
		{
			// make head and tail point to new node
			head = tail = curr;

			// make prev nullptr
			curr->prev = nullptr;
		}
		else // already one or more nodes
		{
			// link tail to new node
			tail->next = curr;
			curr->prev = tail;

			// move tail forward to new node
			tail = curr;
		}
	}

	// pop item from back of stack
	data_type pop()
	{
		if (empty())
		{
			// print error message
			std::cout << "Error: Cannot pop. Stack is empty.\n";

			// return empty data_type
			return data_type();
		}
		else if (head == tail) // only one node in stack
		{
			// create variable for data to be removed
			data_type removed_data = tail->data;

			// remove node
			delete tail;

			// reset class node pointers to nullptr
			head = curr = tail = nullptr;

			// return value of data removed
			return removed_data;
		}
		else // more than one node in stack
		{
			// create variable for data to be removed
			data_type removed_data = tail->data;

			// set curr to point to tail
			curr = tail;

			// move tail back
			tail = tail->prev;

			// make tail next nullptr
			tail->next = nullptr;

			// remove old tail
			delete curr;

			// return value of data removed
			return removed_data;
		}
	}

	// print stack
	void print()
	{
		if (empty())
		{
			// print error message
			std::cout << "Error: Cannot print stack. Stack is empty.\n";

			return;
		}
		else // stack has items
		{
			// begin curr at head
			curr = head;

			// loop through stack
			while (curr != nullptr)
			{
				// display curr data
				std::cout << curr->data << " ";

				// progress curr forwards
				curr = curr->next;
			}

			// go to new line
			std::cout << std::endl;
		}
	}

private:
	// struct for nodes
	struct node
	{
		// data for node to hold
		data_type data;

		// pointers to next and previous nodes
		node* next;
		node* prev;
	};

	// node pointers for head, tail, and current nodes
	node* head;
	node* curr;
	node* tail;

};

#endif /* STACK_H */