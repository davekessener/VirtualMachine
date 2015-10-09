public interface TI_List<P, T>
{
	public abstract void insert(P p, T t);
	public abstract void delete(P p);
	public abstract P find(T t);
	public abstract T retrieve(P p);
	public abstract void concat(TI_List<? extends T> l);
	public abstract int size( );
}

