package com.gaklib;

/**
 * <p>Title: Java GAKLIB</p>
 *
 * <p>Description: Java Library</p>
 *
 * <p>Copyright: (c) 2007</p>
 *
 * <p>Organisation: CRESD GmbH</p>
 *
 * @author Martin Gäckler
 * @version 1.0
 */
public class Lock
{
	private Thread ownerThread = null;
	private int lockCount = 0;

	/**
	 * tryLock
	 * tries to lock the current object
	 *
	 * @return boolean true if lock was possible
	 */
	public synchronized boolean tryLock()
	{
		if( ownerThread == null )
		{
			ownerThread = Thread.currentThread();
			lockCount = 1;
			return true;
		}
		else if( ownerThread == Thread.currentThread() )
		{
			lockCount ++;
			return true;
		}

		return false;
	}

	/**
	 * lock
	 * locks the current object. It does not return until the lock is available
	 */
	public synchronized void lock()
	{
		while( !tryLock() )
		{
			try
			{
				wait();
			}
			catch( Exception e ) {}		// ignore any exceptions
		}
	}

	/**
	 * freeLock
	 * frees the lock object
	 *
	 * @return boolean true if lock object is freed, false if the lock object was
	 *   not owned by the current thread
	 */
	public synchronized boolean freeLock()
	{
		if( ownerThread == Thread.currentThread() )
		{
			lockCount--;
			if( lockCount == 0 )
			{
				ownerThread = null;
				notifyAll();
			}
			return true;
		}
		else
			return false;
	}

}
