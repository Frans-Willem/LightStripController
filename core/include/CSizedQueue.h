#ifndef CSIZEDQUEUE_H
#define CSIZEDQUEUE_H
#include <string>

template <class T> class CSizedQueue {
private:
	unsigned int m_nCapacity;
	unsigned int m_nFirstFull;
	unsigned int m_nFirstEmpty;
	T *m_pItems;
public:
	CSizedQueue(unsigned int nCapacity) {
		m_nCapacity = nCapacity + 1;
		m_pItems = new T[m_nCapacity];
		m_nFirstFull = 0;
		m_nFirstEmpty = 0;
	}

	~CSizedQueue() {
		delete[] m_pItems;
	}
	unsigned int GetCapacity() {
		return m_nCapacity - 1;
	}
	unsigned int GetSize() {
		return ((m_nFirstEmpty + m_nCapacity) - m_nFirstFull) % m_nCapacity;
	}
	unsigned int GetAvailable() {
		return ((m_nFirstFull + (m_nCapacity - 1)) - m_nFirstEmpty) % m_nCapacity;
	}

	void Put(T item) {
		unsigned int nItem = m_nFirstEmpty;
		unsigned int nNext = (nItem + 1) % m_nCapacity;
		if (nNext == m_nFirstFull) {
			//Over capacity
			throw std::string("Over capacity");
		}
		m_nFirstEmpty = nNext;
		m_pItems[nItem] = item;
	}

	T Get() {
		if (m_nFirstFull == m_nFirstEmpty) {
			throw std::string("Under capacity");
		}
		unsigned int nItem = m_nFirstFull;
		unsigned int nNext = (nItem + 1) % m_nCapacity;
		m_nFirstFull = nNext;
		return m_pItems[nItem];
	}
};

#endif//CSIZEDQUEUE_H