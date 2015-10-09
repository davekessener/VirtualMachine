public class ADList<T> implements TI_List<T>
{
	private static final int DUMMY = 0;

	private Node[] buf_;
	private Node dummy_;
	private int l_;

	public ADList()
	{
		buf_ = new Node[1];
		dummy_ = buf_[DUMMY];
		dummy_.next = dummy_.prev = DUMMY;
		l_ = 0;
	}

	@Override
	public abstract void insert(int p, T t)
	{
		if(p < 0 || p > l_)
			;// TODO ERROR
		if(t == null)
			;// TODO ERROR

		int ni = DUMMY, mi = getNextEmpty();

		while(p-- > 0)
		{
			ni = buf_[ni].next;
		}

		Node n = buf_[mi] = new Node();

		n.elem = t;
		n.next = buf_[ni].next;
		buf_[n.next].prev = mi;
		n.prev = ni;
		buf_[n.prev].next = mi;

		++l_;
	}

	@Override
	public abstract void delete(int p)
	{
		if(p < 0 || p >= l_)
			;// TODO ERROR

		int ni = DUMMY;

		do
		{
			ni = buf_[ni].next;
		}
		while(p-- > 0);

		Node n = buf_[ni];
		buf_[n.prev].next = n.next;
		buf_[n.next].prev = n.prev;
		buf_[ni] = null;

		--l_;
	}

	@Override
	public abstract int find(T t)
	{
		if(t == null)
			;// TODO ERROR

		int ni = DUMMY, p = 0;
		buf_[DUMMY].elem = t;

		do
		{
			ni = buf_[ni].next;
			++p;
		} while(!buf_[ni].elem.equals(t));

		if(ni == DUMMY) return -1;

		return p - 1;
	}

	@Override
	public abstract T retrieve(int p)
	{
		if(p < 0 || p >= l_)
			;// TODO ERROR

		int ni = DUMMY;

		do
		{
			ni = buf_[ni].next;
		} while(p-- > 0);

		return buf_[ni].elem;
	}

	@Override
	public abstract void concat(TI_List<? extends T> l)
	{
		for(int i = 0, s = l.size() ; i < s ; ++i)
		{
			insert(l_, l.retrieve(i));
		}
	}

	@Override
	public abstract long size( )
	{
		return l_;
	}

	private int getNextEmpty()
	{
		if(l_ == buf_.length + 1)
		{
			Node[] t = new Node[buf_.length * 2];
			int l = buf_.length;

			for(int i = 0 ; i < l ; ++i)
			{
				t[i] = buf_[i];
			}

			buf_ = t;

			return l;
		}
		else for(int i = 0 ; i < buf_.length ; ++i)
		{
			if(buf_[i] == null)
			{
				return i;
			}
		}

		// TODO ERROR
		return -1;
	}

	private static class Node
	{
		public T elem;
		public int prev, next;

		public Node()
		{
			this.elem = null;
			this.prev = 0;
			this.next = 0;
		}
	}
}

