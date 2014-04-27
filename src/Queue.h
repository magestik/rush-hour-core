/*
 * Queue.h
 *
 *  Created on: 27 avr. 2014
 *      Author: bastien
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#ifndef ASSERT
#define ASSERT(expr)
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

template<typename T>
class Queue
{

private:

	struct sNode
	{
		sNode (const T & d) : data(d), next(NULL) { /* ... */ }
		T data;
		sNode * next;
	};

public:

	typedef T type;

	/**
	 * Default constructor
	 */
	inline explicit Queue(void) : m_first(NULL), m_last(NULL), m_count(0) { /* ... */ }

	/**
	 * Destructor
	 */
	inline ~Queue(void)
	{
		while (!empty())
		{
			dequeue();
		}
	}

    /**
     * Add an element at the end
     */
	inline void enqueue(const T & elmt)
	{
		if (0 == m_count)
		{
			m_first = new sNode(elmt);
			m_last	= m_first;
		}
		else
		{
			sNode * node	= new sNode(elmt);
			m_last->next	= node;
			m_last			= node;
		}

		++m_count;
	}

    /**
     * Remove the first element
     */
	inline void dequeue(void)
	{
		ASSERT(m_count > 0);
		sNode * node = m_first;
		m_first = node->next;
		delete node;

		--m_count;
	}

    /**
     * Get the first element
     */
	inline const T & first(void)
	{
		return(m_first->data);
	}

    /**
     * Test whether stack is empty
     */
    inline bool empty(void)
    {
    	return(0 == m_count);
    }

    /**
     * Return the number of element in the stack
     */
    inline unsigned int count(void)
    {
    	return(m_count);
    }

private:

    sNode *			m_first;	// first element
    sNode *			m_last;		// last element
    unsigned int	m_count;	// number of element in the stack

};

#endif /* QUEUE_H_ */
