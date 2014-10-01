package postfix;

import postfix.error.EmptyStack;

public class Stack<T> extends List<T>
{
	private Node first;
	
	public Stack()
	{
		first = null;
	}
	
	public void push(T t)
	{
		first = new Node(t, first);
	}
	
	public T top() throws EmptyStack
	{
		if(first == null) throw new EmptyStack();
		
		return first.data;
	}
	
	public void pop() throws EmptyStack
	{
		if(first == null) throw new EmptyStack();
		
		first = first.next;
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
	public Stack<T> clone()
	{
		Stack<T> s = new Stack<T>();
		s.first = first.clone();
		return s;
	}
}
