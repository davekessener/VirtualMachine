package postfix;

import postfix.error.EmptyQueue;

public class Queue<T> extends List<T>
{
	private Node first, last;
	
	public Queue()
	{
		first = last = null;
	}
	
	public void push(T t)
	{
		if(last == null)
		{
			first = last = new Node(t, null);
		}
		else
		{
			last = (last.next = new Node(t, null));
		}
	}
	
	public T top() throws EmptyQueue
	{
		if(first == null) throw new EmptyQueue();
		
		return first.data;
	}
	
	public void poll() throws EmptyQueue
	{
		if(first == null) throw new EmptyQueue();
		
		if((first = first.next) == null) last = null;
	}
	
	public boolean empty()
	{
		return first == null;
	}
	
	public int size()
	{
		return length(first);
	}
	
	@Override
	public Queue<T> clone()
	{
		Queue<T> q = new Queue<T>();
		q.last = q.first = first.clone();
		while(q.last.next != null) q.last = q.last.next;
		return q;
	}
}
