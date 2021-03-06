// -*- C++ -*-

//==========================================================================
/**
 *  @file    Lock.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//==========================================================================

#ifndef ACE_LOCK_H
#define ACE_LOCK_H
#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Lock
 *
 * @brief This is the abstract base class that contains the uniform
 * locking API that is supported by all the ACE synchronization
 * mechanisms.
 *
 * This class is typically used in conjunction with the
 * ACE_Lock_Adapter in order to provide a polymorphic
 * interface to the ACE synchronization mechanisms (e.g.,
 * ACE_Mutex, ACE_Semaphore, ACE_RW_Mutex, etc).  Note that
 * the reason that all of ACE doesn't use polymorphic locks is
 * that (1) they add ~20% extra overhead for virtual function
 * calls and (2) objects with virtual functions can't be placed
 * into shared memory.
 */
class ACE_Export ACE_Lock
{
public:
  /// CE needs a default constructor here.
  ACE_Lock ();

  /// Noop virtual destructor
  virtual ~ACE_Lock () = default;

  /**
   * Explicitly destroy the lock.  Note that only one thread should
   * call this method since it doesn't protect against race
   * conditions.
   */
  virtual int remove () = 0;

  /// Block the thread until the lock is acquired.  Returns -1 on
  /// failure.
  virtual int acquire () = 0;

  /**
   * Conditionally acquire the lock (i.e., won't block).  Returns -1
   * on failure.  If we "failed" because someone else already had the
   * lock, @c errno is set to @c EBUSY.
   */
  virtual int tryacquire () = 0;

  /// Release the lock.  Returns -1 on failure.
  virtual int release () = 0;

  /**
   * Block until the thread acquires a read lock.  If the locking
   * mechanism doesn't support read locks then this just calls
   * acquire().  Returns -1 on failure.
   */
  virtual int acquire_read () = 0;

  /**
   * Block until the thread acquires a write lock.  If the locking
   * mechanism doesn't support read locks then this just calls
   * acquire().  Returns -1 on failure.
   */
  virtual int acquire_write () = 0;

  /**
   * Conditionally acquire a read lock.  If the locking mechanism
   * doesn't support read locks then this just calls acquire().
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, @c errno is set to @c EBUSY.
   */
  virtual int tryacquire_read () = 0;

  /**
   * Conditionally acquire a write lock.  If the locking mechanism
   * doesn't support read locks then this just calls acquire().
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, @c errno is set to @c EBUSY.
   */
  virtual int tryacquire_write () = 0;

  /**
   * Conditionally try to upgrade a lock held for read to a write lock.
   * If the locking mechanism doesn't support read locks then this just
   * calls acquire(). Returns 0 on success, -1 on failure.
   */
  virtual int tryacquire_write_upgrade () = 0;
};

/**
 * @class ACE_Adaptive_Lock
 *
 * @brief An adaptive general locking class that defers the decision of
 * lock type to run time.
 *
 * This class, as ACE_Lock, provide a set of general locking APIs.
 * However, it defers our decision of what kind of lock to use
 * to the run time and delegates all locking operations to the actual
 * lock.  Users must define a constructor in their subclass to
 * initialize @c lock_.
 */
class ACE_Export ACE_Adaptive_Lock : public ACE_Lock
{
public:
  /// You must also override the destructor function to match with how
  /// you construct the underneath @c lock_.
  virtual ~ACE_Adaptive_Lock () = default;

  // = Lock/unlock operations.

  virtual int remove ();
  virtual int acquire ();
  virtual int tryacquire ();
  virtual int release ();
  virtual int acquire_read ();
  virtual int acquire_write ();
  virtual int tryacquire_read ();
  virtual int tryacquire_write ();
  virtual int tryacquire_write_upgrade ();
  void dump () const;

protected:
  /**
   * Create and initialize create the actual lock used in the class.
   * The default constructor simply set the @c lock_ to 0 (null).  You
   * must overwrite this method for this class to work.
   */
  ACE_Adaptive_Lock () = default;

  ACE_Lock *lock_ {};
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Lock.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_LOCK_H */
