public class ADList<T> extends TI_List<Node, T>
{
	private static final boolean INTRUSIVE = false;

	private Node head_, tail_;
	private int l_;

	public ADList()
	{
		tail_ = new Node();
		head_ = new Node(null, tail_);
		l_ = 0;
	}

	@Override
	public abstract void insert(Node p, T t)
	{
		if(t == null)
			;// TODO ERROR
		if(p == null)
			;// TODO ERROR

		if(p != tail)
		{
			Node n = new Node(t, p.next);
			p.next = n;
		}
		else
		{
			Node n = new Node();
			tail.elem = t;
			tail.next = n
			tail = n;
		}

		++l_;
	}

	@Override
	public abstract void delete(Node p)
	{
		if(p == null || p == tail || p.next == tail)
			;// TODO ERROR

		p.next = p.next.next;

		--l_;
	}

	@Override
	public abstract Node find(T t)
	{
		if(t == null)
			;// TODO ERROR

		Node n = head;

		tail.elem = t;

		do
		{
			n = n.next;
		} while(!n.elem.equals(t));

		tail.elem = null;

		return n == tail ? null : n;
	}

	@Override
	public abstract T retrieve(Node p)
	{
		if(p == null || p == tail || p.next == tail)
			;// TODO ERROR

		return p.next.elem;
	}

	@Override
	public abstract void concat(TI_List<? extends T> l)
	{
		if(INTRUSIVE)
		{
			Node n = l.head.next;
			tail.elem = n.elem;
			tail.next = n.next;
			tail = l.tail;
			t.tail = new Node();
			t.head.next = t.tail;
		}
		else
		{
			Node n = l.head;
			while(n.next != l.tail)
			{
				insert(tail, n.elem);
				n = n.next;
			}
		}
	}

	@Override
	public abstract int size( )
	{
		return l_;
	}

	public static class Node
	{
		public T elem;
		public Node next;

		public Node()
		{
			this.elem = null;
			this.next = null;
		}

		public Node(T elem, Node next)
		{
			this.elem = elem;
			this.next = next;
		}
	}
}

