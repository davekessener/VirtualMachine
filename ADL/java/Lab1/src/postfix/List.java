package postfix;

public abstract class List<T> implements Cloneable
{
	protected Node append(Node n, T t)
	{
		if(n != null) n.next = append(n.next, t);
		
		return n == null ? new Node(t, null) : n;
	}
	
	protected int length(Node n)
	{
		return n == null ? 0 : 1 + length(n.next);
	}

	protected class Node implements Cloneable
	{
		public T data;
		public Node next;
		
		public Node(T t, Node n)
		{
			data = t;
			next = n;
		}
		
		@Override
		public Node clone()
		{
			return new Node(data, next == null ? null : next.clone());
		}
	}
}
