package de.hawhh.informatik.sml.kino.werkzeuge.payment;
import java.awt.Container;
import java.awt.Dialog;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

import de.hawhh.informatik.sml.kino.materialien.StringLiterals;

/**
 * UI klasse des payment werkzeuges
 * @author dave
 *
 */
class PaymentUI
{
    private JDialog frame_;
    private JLabel return_, price_;
    private JButton ok_, cancel_;
    private MoneyField cash_;
    
    public static abstract class Listener implements ActionListener
    {
        @Override public void actionPerformed(ActionEvent e) { act(); }
        public abstract void act();
    }
    
    /**
     * initialisiert oberflaeche
     * @param cash ein <code>MoneyBuffer</code>-Object,
     *      das den eingegebenen gelbetrag haelt
     */
    public PaymentUI(MoneyField cash)
    {
        cash_ = cash;
        frame_ = createDialog();
    }
    
    /**
     * this method updates the total
     * @param price
     */
    public void setPrice(String price)
    {
        price_.setText(price);
    }
    
    /**
     * Zeige den dialog
     */
    public void show()
    {
        frame_.pack();
        frame_.setLocationRelativeTo(null);
        frame_.setVisible(true);
    }
    
    private JDialog createDialog()
    {
        JDialog frame = new JDialog(null, "", Dialog.ModalityType.APPLICATION_MODAL);
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        frame.getContentPane().setLayout(new GridBagLayout());

        addComponents(frame.getContentPane());
        
        return frame;
    }
    
    private void addComponents(Container c)
    {
        c.add(new JLabel(StringLiterals.Get(StringLiterals.PAYMENT_TOTAL)),   
			new GridBagConstraints(0, 0, 1, 1, 0.5, 0.1, GridBagConstraints.FIRST_LINE_START, GridBagConstraints.NONE, new Insets(10, 10, 5, 2), 0, 0));
        c.add(new JLabel(StringLiterals.Get(StringLiterals.PAYMENT_CASH)),  
			new GridBagConstraints(0, 1, 1, 1, 0.5, 0.1, GridBagConstraints.FIRST_LINE_START, GridBagConstraints.NONE, new Insets(2, 10, 5, 2), 0, 0));
        c.add(new JLabel(StringLiterals.Get(StringLiterals.PAYMENT_CHANGE)),
			new GridBagConstraints(0, 2, 1, 1, 0.5, 0.1, GridBagConstraints.FIRST_LINE_START, GridBagConstraints.NONE, new Insets(2, 10, 0, 2), 0, 0));
        
        c.add(price_ = new JLabel(),     
			new GridBagConstraints(2, 0, 1, 1, 1.0, 0.1, GridBagConstraints.FIRST_LINE_END, GridBagConstraints.NONE, new Insets(10, 0, 5, 10), 0, 0));
        c.add(cash_.getTextField(),
			new GridBagConstraints(1, 1, 10, 1, 0.0, 0.0, GridBagConstraints.FIRST_LINE_END, GridBagConstraints.HORIZONTAL, new Insets(0, 10, 0, 5), 0, 0));
        c.add(return_ = new JLabel(),
			new GridBagConstraints(2, 2, 1, 1, 1.0, 0.1, GridBagConstraints.FIRST_LINE_END, GridBagConstraints.NONE, new Insets(2, 0, 5, 10), 0, 0));
        
        JPanel buttons = new JPanel();
        buttons.setLayout(new GridLayout(1, 2, 10, 0));
        buttons.add(ok_ = new JButton(StringLiterals.Get(StringLiterals.OK)));
        buttons.add(cancel_ = new JButton(StringLiterals.Get(StringLiterals.CANCEL)));
        
        c.add(buttons, new GridBagConstraints(1, 3, 2, 1, 1.0, 1.0, GridBagConstraints.PAGE_END, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 10, 10), 0, 0));
    }
    
    /**
     * fuege listener zum OK-knopf hinzu
     * @require l != null
     */
    public void addOKListener(Listener l)
    {
        assert l != null : "Vorbedingung verletzt: l != null";
        
        ok_.addActionListener(l);
    }
    
    /**
     * fuege listener zum Cancel-knopf hinzu
     * @require l != null
     */
    public void addCancelListener(Listener l)
    {
        assert l != null : "Vorbedingung verletzt: l != null";
        
        cancel_.addActionListener(l);
    }
    
    /**
     * fuege listener zum textfeld hinzu
     * @require l != null
     */
    public void addCashListener(MoneyField.Listener l)
    {
        assert l != null : "Vorbedingung verletzt: l != null";
        
        cash_.addListener(l);
    }
    
    /**
     * Schliesst den Dialog
     */
    public void dispose()
    {
        frame_.dispose();
    }
    
    public void setReturnText(String s)
    {
        return_.setText(s);
    }
    
    public void enableOKButton(boolean f)
    {
        ok_.setEnabled(f);
    }
}
