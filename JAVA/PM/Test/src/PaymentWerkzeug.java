import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;


public class PaymentWerkzeug
{
    private int price_;
    private MoneyBuffer cash_;
    private PaymentUI ui_;
    private Callback cb_;
    
    public static interface Callback
    {
        public abstract void call(boolean f);
    }
    
    public PaymentWerkzeug(int price) { this(price, null); }
    public PaymentWerkzeug(int price, Callback cb)
    {
        price_ = price;
        cb_ = cb;
        cash_ = new MoneyBuffer();
        ui_ = new PaymentUI(price, cash_);
        
        registerListeners();
    }
    
    public void run()
    {
        ui_.show();
    }
    
    private void pressedOK()
    {
        close(true);
    }
    
    private void pressedCancel()
    {
        close(false);
    }
    
    private void cashChanged()
    {
        updateCash(cash_.getValue());
    }
    
    private void registerListeners()
    {
        ui_.addCashListener(new MoneyField.Listener() {
            @Override
            public void act()
            {
                cashChanged();
            }
        });
        ui_.addCancelListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                pressedCancel();
            }
        });
        ui_.addOKListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                pressedOK();
            }
        });
    }
    
    private void updateCash(int v)
    {
        int r = v - price_;
        ui_.update(r >= 0 ? MoneyBuffer.format(r) : MoneyBuffer.ZERO, r >= 0);
    }
    
    private void close(boolean f)
    {
        ui_.dispose();
        if(cb_ != null) cb_.call(f);
    }
}
