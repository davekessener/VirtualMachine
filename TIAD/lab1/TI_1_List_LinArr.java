public class ADList<T> implements TI_List<Integer, T>
{
	private Object[] buf_;
	private int i_;

	public ADList()
	{
		buf_ = new Object[0];
		i_ = 0;
	}

	@Override
	public void insert(Integer p, T t)
	{
		if(p < 0 || p > i_)
			;// TODO ERROR
		if(t == null)
			;// TODO ERROR

		resize();

		for(int i = i_ ; i > p ; --i)
		{
			buf_[i] = buf_[i - 1];
		}

		buf_[p] = t;
		++i_;
	}

	@Override
	public void delete(Integer p)
	{
		if(p < 0 || p >= i_)
			;// TODO ERROR

		for(int i = p ; i < i_ - 1 ; ++i)
		{
			buf_[i] = buf_[i + 1];
		}

		buf_[i_ - 1] = null;
		--i_;

		resize();
	}

	@Override
	public Integer find(T t)
	{
		if(t == null)
			;// TODO ERROR

		for(int i = 0 ; i < i_ ; ++i)
		{
			if(buf_[i].equals(t))
				return i;
		}

		return -1;
	}

	@Override
	public T retrieve(Integer p)
	{
		if(p < 0 || p >= i_)
			;// TODO ERROR

		return (T) buf_[p];
	}

	@Override
	public void concat(TI_List<? extends T> l)
	{
		Object[] t = buf_;

		if(buf_.length < i_ + l.i_)
		{
			t = new Object[i_ + l.i_];
			for(int i = 0 ; i < i_ ; ++i)
			{
				t[i] = buf_[i];
			}
		}

		for(int i = 0 ; i < l.i_ ; ++i)
		{
			t[i + i_] = l.buf_[i];
		}

		buf_ = t;
		i_ += l.i_;
	}

	@Override
	public int size( )
	{
		return i_;
	}

	private void resize()
	{
		Object[] t = null;

		if(i_ == buf_.length)
		{
			t = new Object[buf_.length * 2 + 1];
		}
		else if(i_ < buf_.length / 4)
		{
			t = new Object[buf_.length / 2];
		}

		if(t != null)
		{
//			Array.copy(t, buf_, i_);
			for(int i = 0 ; i < i_ ; ++i)
			{
				t[i] = buf_[i];
			}

			buf_ = t;
		}
	}
}

