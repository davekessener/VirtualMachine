package lab6;

public interface List<T>
{
    public T peekNo(int i);
    public T peekF();
    public T peekL();
    public T getNo(int i);
    public T getF();
    public T getL();
    public boolean putNo(int i, T t);
    public void putF(T t);
    public void putL(T t);
    public T setNo(int i, T t);
    public boolean removeNo(int i);
    public boolean remove(T t);
    public void clear();
    public boolean isEmpty();
    public int getSize();
    public boolean contains(T t);
}
