package viewer.service.connection;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.List;
import java.util.function.Consumer;

import viewer.service.Logger;

public class ConnectionThread implements Runnable
{
    private Thread thread_;
    private boolean running_;
    private List<Thread> tasks_;
    private Deque<Promise> queue_;
    
    public ConnectionThread()
    {
        thread_ = null;
        running_ = false;
        tasks_ = new ArrayList<Thread>();
        queue_ = new ArrayDeque<Promise>();
    }
    
    public void start()
    {
        thread_ = new Thread(this);
        running_ = true;
        
        thread_.start();
    }
    
    @Override
    public void run()
    {
        while(running_)
        {
            synchronized(queue_)
            {
                if(!queue_.isEmpty())
                {
                    Thread t = new Thread(queue_.pop());
                    
                    tasks_.add(t);
                    
                    Logger.Log("Starting new Task.");
                    
                    t.start();
                }
            }
            
            Sleep(100);
        }
        
        Logger.Log("Master thread done.");
        
        tasks_.forEach(CheckedConsumer(t -> t.join()));
    }
    
    public void stop()
    {
        Logger.Log("Stopping master thread ...");
        
        try
        {
            running_ = false;
            thread_.join();
        }
        catch(InterruptedException e) { }
    }
    
    public void register(Promise p)
    {
        Logger.Log("Promise given.");
        
        synchronized(queue_)
        {
            queue_.addLast(p);
        }
    }
    
    public boolean isRunning() { return running_; }
    
    public static final void Sleep(int ms)
    {
        try
        {
            Thread.sleep(ms);
        }
        catch(InterruptedException e)
        {
        }
    }
    
    private static interface ConsumeChecked<T> { void accept(T t) throws Exception; }
    private static <T> Consumer<T> CheckedConsumer(ConsumeChecked<T> c)
    {
        return new Consumer<T>() {
            @Override
            public void accept(T t)
            {
                try
                {
                    c.accept(t);
                }
                catch(Exception e) { }
            }
        };
    }
}
