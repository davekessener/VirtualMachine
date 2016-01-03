package lab6;

public class BoringList<T> implements List<T>
{
    private Node root_ = null;
    
    private T peek(int i) { return peek(root_, i); }
    private T peek(Node n, int i)
    {
        return i == 0 ? n.elem : peek(n.next, i - 1);
    }

    @Override
    public T peekNo(int i)
    {
        return i < 0 || i >= getSize() ? null : peek(i);
    }

    @Override
    public T peekF()
    {
        return peekNo(0);
    }

    @Override
    public T peekL()
    {
        return peekNo(getSize() - 1);
    }

    @Override
    public T getNo(int i)
    {
        T t = peekNo(i);
        
        removeNo(i);
        
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
    
    private void put(int i, T t) { root_ = put(root_, i, t); }
    private Node put(Node n, int i, T t)
    {
        if(i == 0)
            n = new Node(t, n);
        else
            n.next = put(n.next, i - 1, t);
        
        return n;
    }

    @Override
    public boolean putNo(int i, T t)
    {
        if(i < 0 || i > getSize())
            return false;
        
        put(i, t);
        
        return true;
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
        T tt = getNo(i);
        
        putNo(i, t);
        
        return tt;
    }
    
    private void remove(int i) { root_ = remove(root_, i); }
    private Node remove(Node n, int i)
    {
        if(i == 0)
            n = n.next;
        else
            n.next = remove(n.next, i - 1);
        
        return n;
    }

    @Override
    public boolean removeNo(int i)
    {
        if(i < 0 || i >= getSize())
            return false;
        
        remove(i);
        
        return true;
    }
    
    private int find(T t) { return find(root_, t, 0); }
    private int find(Node n, T t, int i)
    {
        return n == null ? -1 : (n.elem.equals(t) ? i : find(n.next, t, i + 1));
    }

    @Override
    public boolean remove(T t)
    {
        return removeNo(find(t));
    }

    @Override
    public void clear()
    {
        root_ = null;
    }

    @Override
    public boolean isEmpty()
    {
        return root_ != null;
    }
    
    private int size() { return size(root_); }
    private int size(Node n)
    {
        return n == null ? 0 : 1 + size(n.next);
    }

    @Override
    public int getSize()
    {
        return size();
    }

    @Override
    public boolean contains(T t)
    {
        return find(t) >= 0;
    }

    private class Node
    {
        public T elem;
        public Node next;
        
        public Node(T e, Node n) { elem = e; next = n; }
    }
}
