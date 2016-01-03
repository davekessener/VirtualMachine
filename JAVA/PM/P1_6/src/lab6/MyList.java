package lab6;

public class MyList<T> implements List<T>
{
    private T car_;
    private MyList<T> cdr_;
    
    public MyList()
    {
        car_ = null;
        cdr_ = null;
    }
    
    private MyList(T t)
    {
        car_ = t;
        cdr_ = null;
    }
    
    @Override
	public T peekNo(int i)
    {
        return i > 0 ? (cdr_ == null ? null : cdr_.peekNo(i - 1)) : car_;
    }
    
    @Override
	public T peekF()
    {
        return car_;
    }
    
    @Override
	public T peekL()
    {
        return cdr_ == null ? car_ : cdr_.peekL();
    }

    @Override
	public T getNo(int i)
    {
        T t = null;
        
        if(i < 0)
        {
            return null;
        }
        else if(i == 0)
        {
            t = car_;

            car_ = cdr_ == null ? null : cdr_.getNo(0);
        }
        else if(cdr_ != null)
        {
            t = cdr_.getNo(i - 1);
        }
        
        if(cdr_ != null && cdr_.car_ == null)
            cdr_ = null;
        
        return t;
    }
    
    @Override
	public T getF()
    {
        return getNo(0);
    }
    
    @Override
	public T getL()
    {
        return getNo(getSize() - 1);
    }

    @Override
	public boolean putNo(int i, T t)
    {
        if(i < 0)
        {
            return false;
        }
        else if(i == 0)
        {
            if(car_ != null)
            {
                if(cdr_ == null)
                {
                    cdr_ = new MyList<T>(car_);
                }
                else
                {
                    cdr_.putNo(0, car_);
                }
            }
            
            car_ = t;
            
            return true;
        }
        else if(i == 1 && cdr_ == null)
        {
            cdr_ = new MyList<>(t);
            
            return true;
        }
        else
        {
            return cdr_ == null ? false : cdr_.putNo(i - 1, t);
        }
    }
    
    @Override
	public void putF(T t)
    {
        putNo(0, t);
    }

    @Override
	public void putL(T t)
    {
        putNo(getSize(), t);
    }

    @Override
	public T setNo(int i, T t)
    {
        if(i < 0)
        {
            return null;
        }
        else if(i == 0)
        {
            T tt = car_;
            
            car_ = t;
            
            return tt;
        }
        else
        {
            return cdr_ == null ? null : cdr_.setNo(i - 1, t);
        }
    }

    @Override
	public boolean removeNo(int i)
    {
        return getNo(i) != null;
    }
    
    @Override
	public boolean remove(T t)
    {
        if(t == null || car_ == null)
        {
            return false;
        }
        else if(car_.equals(t))
        {
            return removeNo(0);
        }
        else
        {
            return cdr_ == null ? false : cdr_.remove(t);
        }
    }
    
    @Override
	public void clear()
    {
        car_ = null;
        cdr_ = null;
    }
    
    @Override
	public boolean isEmpty()
    {
        return getSize() == 0;
    }
    
    @Override
	public int getSize()
    {
        return car_ == null ? 0 : 1 + (cdr_ == null ? 0 : cdr_.getSize());
    }
    
    @Override
	public boolean contains(T t)
    {
        if(t == null || car_ == null)
        {
            return false;
        }
        else if(car_.equals(t))
        {
            return true;
        }
        else
        {
            return cdr_ == null ? false : cdr_.contains(t);
        }
    }
}
