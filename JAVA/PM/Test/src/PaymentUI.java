import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;


public class PaymentUI
{
    private JFrame frame_;
    private JLabel return_;
    private JButton ok_, cancel_;
    private MoneyField cash_;
    
    public PaymentUI(int price, MoneyBuffer cash)
    {
        assert price > 0 : "Precondition violated: price > 0";

        cash_ = new MoneyField(cash);
        frame_ = createDialog(MoneyBuffer.format(price));
    }
    
    private JFrame createDialog(String price)
    {
        JFrame frame = new JFrame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().setLayout(new GridBagLayout());

        addComponents(frame.getContentPane(), price);
        
        return frame;
    }
    
    public void show()
    {
        frame_.pack();
        frame_.setMaximumSize(frame_.getSize());
        frame_.setLocationRelativeTo(null);
        frame_.setVisible(true);
    }
    
    private void addComponents(Container c, String price)
    {
        c.add(new JLabel("Betrag:"),   
			new GridBagConstraints(0, 0, 1, 1, 0.5, 0.1, GridBagConstraints.FIRST_LINE_START, GridBagConstraints.NONE, new Insets(10, 10, 5, 2), 0, 0));
        c.add(new JLabel("Gezahlt:"),  
			new GridBagConstraints(0, 1, 1, 1, 0.5, 0.1, GridBagConstraints.FIRST_LINE_START, GridBagConstraints.NONE, new Insets(2, 10, 5, 2), 0, 0));
        c.add(new JLabel("Rueckgeld:"),
			new GridBagConstraints(0, 2, 1, 1, 0.5, 0.1, GridBagConstraints.FIRST_LINE_START, GridBagConstraints.NONE, new Insets(2, 10, 0, 2), 0, 0));
        
        c.add(new JLabel(price),     
			new GridBagConstraints(2, 0, 1, 1, 1.0, 0.1, GridBagConstraints.FIRST_LINE_END, GridBagConstraints.NONE, new Insets(10, 0, 5, 10), 0, 0));
        c.add(cash_.getTextField(),
			new GridBagConstraints(1, 1, 10, 1, 0.0, 0.0, GridBagConstraints.FIRST_LINE_END, GridBagConstraints.HORIZONTAL, new Insets(0, 10, 0, 5), 0, 0));
        c.add(return_ = new JLabel(MoneyBuffer.ZERO),
			new GridBagConstraints(2, 2, 1, 1, 1.0, 0.1, GridBagConstraints.FIRST_LINE_END, GridBagConstraints.NONE, new Insets(2, 0, 5, 10), 0, 0));
        
        c.add(ok_ = new JButton("OK"),       
			new GridBagConstraints(1, 3, 1, 1, 1.0, 1.0, GridBagConstraints.LAST_LINE_END, GridBagConstraints.HORIZONTAL, new Insets(5, 0, 10, 5), 0, 0));
        c.add(cancel_ = new JButton("Cancel"),   
			new GridBagConstraints(2, 3, 1, 1, 1.0, 1.0, GridBagConstraints.LAST_LINE_END, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 10, 10), 0, 0));

        update(MoneyBuffer.ZERO, false);
    }
    
    public void addOKListener(ActionListener l)
    {
        ok_.addActionListener(l);
    }
    
    public void addCancelListener(ActionListener l)
    {
        cancel_.addActionListener(l);
    }
    
    public void addCashListener(MoneyField.Listener l)
    {
        cash_.addListener(l);
    }
    
    public void dispose()
    {
        frame_.dispose();
    }
    
    public void update(String r, boolean a)
    {
        return_.setText(r);
        ok_.setEnabled(a);
    }
}
