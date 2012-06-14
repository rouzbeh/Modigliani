/**
 * @file Listbj.h
 * General linked list class for any type--numerical, pointer or class
 * @author Ahmed A. Faisal, 22. 5. 1998(c)
 * @version  0.1
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _mbase_list_h_
#define _mbase_list_h_

#include "types.h"
#include "Error.h"

namespace modigliani_base{

/** PURPOSE: A link in a linked list or queue
 REMARKS: The data fields are public, hence users of this class
 should not propagate Link_list pointers
 */

template<class T> class Link_list {
public:
	Link_list(const T&, Link_list<T>*);

	T _info;
	Link_list<T>* _next;

	static T defval;
};

/*-------------------------------------------------------------------------*/
template<class T> class Iterator_list;

/** PURPOSE:    A linked list
 */
template<class T> class List {
public:
	List();
	void insert_head(T n);
	void insert_tail(T n);
	void insert(T n);
	// insert before current position
	T remove(); // remove current position
	T& current(); // info of current element
	void next(); // advance current position
	void reset();
	void empty();

	const T& current() const; // info of current element
	bool at_end() const;
	const T& head() const;
	const T& tail() const;
	int length() const;

	List(const List<T>&);
	const List<T>& operator=(const List<T>&);
	~List();

private:
	void free();
	void copy(const List<T>&);

	Link_list<T>* cursor() const {
		return (_pre ? _pre->_next : _head);
	}

	Link_list<T>* _head;
	Link_list<T>* _tail;
	Link_list<T>* _pre; // predecessor of cursor
	int _len;

	friend class Iterator_list<T> ;
};

/*-------------------------------------------------------------------------*/

/** REMARKS:    1. This iterator is for inspection of a list only. You can
 only modify a list through its own cursor
 2. Modification of a list invalidates all iterators attached
 to it.
 */
template<class T> class Iterator_list {
public:
	Iterator_list(const List<T> &);

	void next(); // advance current position
	void reset();

	const T& current() const; // info of current element
	bool at_end() const;

private:
	Link_list<T>* cursor() const {
		return (_pre ? _pre->_next : _list._head);
	}

	const List<T>& _list;
	Link_list<T>* _pre;
};

//............................................................................

/* RECEIVES:   i - the information to store in the link
 n - pointer to next link
 */
template<class T> Link_list<T>::Link_list(const T& i,
		Link_list<T>* n) :
		_info(i), _next(n) {
}

/*-------------------------------------------------------------------------*/

template<class T> List<T>::List() :
		_head(0), _tail(0), _pre(0), _len(0) {
}

//............................................................................

template<class T> List<T>::~List() {
	free();
}

//............................................................................

template<class T> List<T>::List(const List<T>& b) {
	copy(b);
}

//............................................................................

template<class T> const List<T>& List<T>::operator=(
		const List<T>& b) {
	if (this != &b) {
		free();
		copy(b);
	}
	return (*this);
}

//............................................................................

template<class T> void List<T>::free() {
	Link_list<T>* p = _head;
	while (p != 0) {
		Link_list<T>* q = p;
		p = p->_next;
		delete q;
	}
}

//............................................................................

template<class T> void List<T>::copy(const List<T>& b) {
	_len = b._len;
	_pre = _head = 0;

	Link_list<T>* pa = 0;
	for (Link_list<T>* pb = b._head; pb != 0; pb = pb->_next) {
		Link_list<T>* pnew = new Link_list<T>(pb->_info, 0);
		M_ASSERT_MEM(pnew);
		if (pa)
			pa = pa->_next = pnew;
		else
			pa = _head = pnew;
		if (b._pre == pb)
			_pre = pa;
	}
	_tail = pa;
}

//............................................................................

/* PURPOSE:    empty the list
 */
template<class T> void List<T>::empty() {
	free();
	_head = 0;
	_tail = 0;
	_pre = 0;
	_len = 0;
}

//............................................................................

/** PURPOSE:    reset the iterator
 */
template<class T> void List<T>::reset() {
	_pre = 0;
}

//............................................................................

/* RETURNS:    TRUE iff the iterator is at the end of the list
 */
template<class T> bool List<T>::at_end() const {
	return (cursor() == 0);/* NOT M_FALSE :) */
}

//............................................................................

/** PURPOSE:    move the iterator to the next position
 */
template<class T> void List<T>::next() {
	if (!_pre)
		_pre = _head;
	else if (_pre->_next)
		_pre = _pre->_next;
	// do not advance past the tail of the list
}

//............................................................................

/** RETURNS:    the value stored at the iterator position
 */
template<class T> T& List<T>::current() {
	Link_list<T>* cur = cursor();
	M_ASSERT_PRECOND(cur);
	return (cur->_info);
}

//............................................................................

/** RETURNS:    the value stored at the iterator position
 */
template<class T> const T& List<T>::current() const {
	Link_list<T>* cur = cursor();
	return (cur ? cur->_info : Link_list<T>::defval());
}

//............................................................................

/** RETURNS:    the value stored at the head of the list
 */
template<class T> const T& List<T>::head() const {
	return (_head ? _head->_info : Link_list<T>::defval());
}

//............................................................................

/** RETURNS:    the value stored at the tail of the list
 */
template<class T> const T& List<T>::tail() const {
	return (_tail ? _tail->_info : Link_list<T>::defval());
}

//............................................................................

/** PURPOSE:    insert before the iterator position
 RECEIVES:   n - the value to insert
 */
template<class T> void List<T>::insert(T n) {
	Link_list<T>* p = new Link_list<T>(n, cursor());
	M_ASSERT_MEM(p);

	if (_pre) {
		_pre->_next = p;
		if (_pre == _tail)
			_tail = p;
	} else {
		if (_head == 0)
			_tail = p;
		_head = p;
	}

	_pre = p;
	_len++;
}

//............................................................................

/** PURPOSE:    insert before the head of the list
 RECEIVES:   n - the value to insert
 */
template<class T> void List<T>::insert_head(T n) {
	Link_list<T>* p = new Link_list<T>(n, _head);
	M_ASSERT_MEM(p);
	_head = p;
	_len++;
}

//............................................................................

/** PURPOSE:    insert after the tail of the list
 RECEIVES:   n - the value to insert
 */
template<class T> void List<T>::insert_tail(T n) {
	Link_list<T>* p = new Link_list<T>(n, 0);
	M_ASSERT_MEM(p);
	if (_head == 0)
		_head = _tail = p;
	else {
		_tail->_next = p;
		_tail = p;
	}
	_len++;
}

//............................................................................

/** PURPOSE:    remove the element under the cursor
 RETURNS:    the removed element
 */
template<class T> T List<T>::remove() {
	Link_list<T>* cur = cursor();
	if (!cur)
		return (Link_list<T>::defval());
	if (_tail == cur)
		_tail = _pre;
	if (_pre)
		_pre->_next = cur->_next;
	else
		_head = cur->_next;
	T r = cur->_info;
	delete cur;
	_len--;
	return (r);
}

//............................................................................

/** RETURNS:    the number of elements in the list
 */
template<class T> int List<T>::length() const {
	return (_len);
}

/**-------------------------------------------------------------------------*/

/** RECEIVES:   l - the list to iterate over
 */
template<class T> Iterator_list<T>::Iterator_list(
		const List<T>& l) :
		_list(l), _pre(0) {
}

//............................................................................

/** PURPOSE:    reset the iterator
 */
template<class T> void Iterator_list<T>::reset() {
	_pre = 0;
}

//............................................................................

/** RETURNS:    TRUE iff the iterator is at the end of the list
 */
template<class T> bool Iterator_list<T>::at_end() const {
	return (cursor() == 0);
}

//............................................................................

/** PURPOSE:    move the iterator to the next position
 */
template<class T> void Iterator_list<T>::next() {
	if (!_pre)
		_pre = _list._head;
	else if (_pre->_next)
		_pre = _pre->_next;
	// do not advance past the tail of the list
}

//............................................................................

/** RETURNS:    the value stored at the iterator position
 */
template<class T> const T& Iterator_list<T>::current() const {
	Link_list<T>* cur = cursor();
	return (cur ? cur->_info : Link_list<T>::defval);
}
}
#endif /* _mbase_list_h_ */

