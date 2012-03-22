/* NT_list_obj.h - general linked list class for any type--numerical, pointer or class */
/* by Ahmed A. Faisal, 22. 5. 1998(c) */

/*
 * NetTrader - finance management, analysis and simulation system
 * Version:  0.1
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* $Id: nt_list_obj.h,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_list_obj.h,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/04/26 17:10:38  face
 * minor bug fixed
 * and upgrades:
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.4  1999/02/17 17:46:22  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/01/07 23:52:56  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/12/25 21:28:17  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/11/22 22:53:59  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/07/02 18:58:38  face
 * Initial revision
 *
 *
 */

#ifndef _NT_LIST_OBJ_H_
#define _NT_LIST_OBJ_H_

#include "nt_main.h"
#include "nt_types.h"
#include "nt_error_obj.h"

/**The default constructor
 constructs an empty queue. The empty operation empties an existing
 queue. The length operation returns the number of elements currently
 stored in the queue.
 */

/** PURPOSE: A link in a linked list or queue
 REMARKS: The data fields are public, hence users of this class
 should not propagate NT_link_list_o pointers
 */

template<class T> class NT_link_list_o {
public:
	NT_link_list_o(const T&, NT_link_list_o<T>*);

	T _info;
	NT_link_list_o<T>* _next;

	static const T& defval();
};

/*-------------------------------------------------------------------------*/
template<class T> class NT_iterator_list_o;

/** PURPOSE:    A linked list
 */
template<class T> class NT_list_o {
public:
	NT_list_o();
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
	NTbool at_end() const;
	const T& head() const;
	const T& tail() const;
	int length() const;

	NT_list_o(const NT_list_o<T>&);
	const NT_list_o<T>& operator=(const NT_list_o<T>&);
	~NT_list_o();

private:
	void free();
	void copy(const NT_list_o<T>&);

	NT_link_list_o<T>* cursor() const {
		return _pre ? _pre->_next : _head;
	}

	NT_link_list_o<T>* _head;
	NT_link_list_o<T>* _tail;
	NT_link_list_o<T>* _pre; // predecessor of cursor
	int _len;

	friend class NT_iterator_list_o<T> ;
};

/*-------------------------------------------------------------------------*/

/** REMARKS:    1. This iterator is for inspection of a list only. You can
 only modify a list through its own cursor
 2. Modification of a list invalidates all iterators attached
 to it.
 */
template<class T> class NT_iterator_list_o {
public:
	NT_iterator_list_o(const NT_list_o<T> &);

	void next(); // advance current position
	void reset();

	const T& current() const; // info of current element
	NTbool at_end() const;

private:
	NT_link_list_o<T>* cursor() const {
		return _pre ? _pre->_next : _list._head;
	}

	const NT_list_o<T>& _list;
	NT_link_list_o<T>* _pre;
};

//***************************************************************************

/** PURPOSE:    make a default object of type T
 REMARKS:    We use a function because Borland 3.1 doesn't handle static
 data members in templates correctly
 */
template<class T> const T& NT_link_list_o<T>::defval() {
	static T t;
	return t;
}

//............................................................................

/* RECEIVES:   i - the information to store in the link
 n - pointer to next link
 */
template<class T> NT_link_list_o<T>::NT_link_list_o(const T& i,
		NT_link_list_o<T>* n) :
		_info(i), _next(n) {
}

/*-------------------------------------------------------------------------*/

template<class T> NT_list_o<T>::NT_list_o() :
		_head(0), _tail(0), _pre(0), _len(0) {
}

//............................................................................

template<class T> NT_list_o<T>::~NT_list_o() {
	free();
}

//............................................................................

template<class T> NT_list_o<T>::NT_list_o(const NT_list_o<T>& b) {
	copy(b);
}

//............................................................................

template<class T> const NT_list_o<T>& NT_list_o<T>::operator=(
		const NT_list_o<T>& b) {
	if (this != &b) {
		free();
		copy(b);
	}
	return *this;
}

//............................................................................

template<class T> void NT_list_o<T>::free() {
	NT_link_list_o<T>* p = _head;
	while (p != 0) {
		NT_link_list_o<T>* q = p;
		p = p->_next;
		delete q;
	}
}

//............................................................................

template<class T> void NT_list_o<T>::copy(const NT_list_o<T>& b) {
	_len = b._len;
	_pre = _head = 0;

	NT_link_list_o<T>* pa = 0;
	for (NT_link_list_o<T>* pb = b._head; pb != 0; pb = pb->_next) {
		NT_link_list_o<T>* pnew = new NT_link_list_o<T>(pb->_info, 0);
		NT_ASSERT_MEM(pnew);
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
template<class T> void NT_list_o<T>::empty() {
	free();
	_head = 0;
	_tail = 0;
	_pre = 0;
	_len = 0;
}

//............................................................................

/** PURPOSE:    reset the iterator
 */
template<class T> void NT_list_o<T>::reset() {
	_pre = 0;
}

//............................................................................

/* RETURNS:    TRUE iff the iterator is at the end of the list
 */
template<class T> NTbool NT_list_o<T>::at_end() const {
	return cursor() == 0;/* NOT NT_FALSE :) */
}

//............................................................................

/** PURPOSE:    move the iterator to the next position
 */
template<class T> void NT_list_o<T>::next() {
	if (!_pre)
		_pre = _head;
	else if (_pre->_next)
		_pre = _pre->_next;
	// do not advance past the tail of the list
}

//............................................................................

/** RETURNS:    the value stored at the iterator position
 */
template<class T> T& NT_list_o<T>::current() {
	NT_link_list_o<T>* cur = cursor();
	NT_ASSERT_PRECOND(cur);
	return cur->_info;
}

//............................................................................

/** RETURNS:    the value stored at the iterator position
 */
template<class T> const T& NT_list_o<T>::current() const {
	NT_link_list_o<T>* cur = cursor();
	return cur ? cur->_info : NT_link_list_o<T>::defval();
}

//............................................................................

/** RETURNS:    the value stored at the head of the list
 */
template<class T> const T& NT_list_o<T>::head() const {
	return _head ? _head->_info : NT_link_list_o<T>::defval();
}

//............................................................................

/** RETURNS:    the value stored at the tail of the list
 */
template<class T> const T& NT_list_o<T>::tail() const {
	return _tail ? _tail->_info : NT_link_list_o<T>::defval();
}

//............................................................................

/** PURPOSE:    insert before the iterator position
 RECEIVES:   n - the value to insert
 */
template<class T> void NT_list_o<T>::insert(T n) {
	NT_link_list_o<T>* p = new NT_link_list_o<T>(n, cursor());
	NT_ASSERT_MEM(p);

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
template<class T> void NT_list_o<T>::insert_head(T n) {
	NT_link_list_o<T>* p = new NT_link_list_o<T>(n, _head);
	NT_ASSERT_MEM(p);
	_head = p;
	_len++;
}

//............................................................................

/** PURPOSE:    insert after the tail of the list
 RECEIVES:   n - the value to insert
 */
template<class T> void NT_list_o<T>::insert_tail(T n) {
	NT_link_list_o<T>* p = new NT_link_list_o<T>(n, 0);
	NT_ASSERT_MEM(p);
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
template<class T> T NT_list_o<T>::remove() {
	NT_link_list_o<T>* cur = cursor();
	if (!cur)
		return NT_link_list_o<T>::defval();
	if (_tail == cur)
		_tail = _pre;
	if (_pre)
		_pre->_next = cur->_next;
	else
		_head = cur->_next;
	T r = cur->_info;
	delete cur;
	_len--;
	return r;
}

//............................................................................

/** RETURNS:    the number of elements in the list
 */
template<class T> int NT_list_o<T>::length() const {
	return _len;
}

/**-------------------------------------------------------------------------*/

/** RECEIVES:   l - the list to iterate over
 */
template<class T> NT_iterator_list_o<T>::NT_iterator_list_o(
		const NT_list_o<T>& l) :
		_list(l), _pre(0) {
}

//............................................................................

/** PURPOSE:    reset the iterator
 */
template<class T> void NT_iterator_list_o<T>::reset() {
	_pre = 0;
}

//............................................................................

/** RETURNS:    TRUE iff the iterator is at the end of the list
 */
template<class T> NTbool NT_iterator_list_o<T>::at_end() const {
	return cursor() == 0;
}

//............................................................................

/** PURPOSE:    move the iterator to the next position
 */
template<class T> void NT_iterator_list_o<T>::next() {
	if (!_pre)
		_pre = _list._head;
	else if (_pre->_next)
		_pre = _pre->_next;
	// do not advance past the tail of the list
}

//............................................................................

/** RETURNS:    the value stored at the iterator position
 */
template<class T> const T& NT_iterator_list_o<T>::current() const {
	NT_link_list_o<T>* cur = cursor();
	return cur ? cur->_info : NT_link_list_o<T>::defval();
}

#endif /* _NT_LIST_OBJ_H_ */

