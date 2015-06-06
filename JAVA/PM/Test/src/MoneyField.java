import java.awt.Toolkit;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JOptionPane;
import javax.swing.JTextField;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

public class MoneyField implements KeyListener
{
    private JTextField tf_;
    private MoneyBuffer eur_;
    private int idx_;
    
    public static abstract class Listener implements DocumentListener
    {
        @Override public void changedUpdate(DocumentEvent e) { act(); }
        @Override public void insertUpdate(DocumentEvent e) { act(); }
        @Override public void removeUpdate(DocumentEvent e) { act(); }
        public abstract void act();
    }
    
    public MoneyField(MoneyBuffer mb)
    {
        tf_ = new JTextField(MoneyBuffer.ZERO);
        eur_ = mb;
        idx_ = 0;

        tf_.setHorizontalAlignment(JTextField.RIGHT);
        tf_.setCaretPosition(1);
        tf_.addKeyListener(this);
    }
    
    public void addListener(Listener l)
    {
        tf_.getDocument().addDocumentListener(l);
    }
    
    public int getValue()
    {
        return eur_.getValue();
    }
    
    public JTextField getTextField()
    {
        return tf_;
    }
    
    @Override
    public void keyPressed(KeyEvent e)
    {
        try
        {
            processInput(e.getKeyChar());

            update();
        }
        catch(BufferOverflowException ex)
        {
            JOptionPane.showMessageDialog(null, ex.getLocalizedMessage(), "BufferOverflowException", JOptionPane.ERROR_MESSAGE);
        }

        e.consume();
    }
    
    private void processInput(char c) throws BufferOverflowException
    {
        if(Character.isDigit(c))
        {
            handleDigit(c - '0');
        }
        else
        {
            handleControlSequence(c);
        }
    }
    
    public void update()
    {
        String s = eur_.toString();
        
        tf_.setText(s);
        tf_.setCaretPosition(s.indexOf('.') + idx_);
    }
    
    private void handleDigit(int d) throws BufferOverflowException
    {
        if(idx_ > 0)
        {
            if(idx_ < 3)
            {
                eur_.setCents(idx_ - 1, d);
                ++idx_;
            }
            else
            {
                beep();
            }
        }
        else
        {
            eur_.insert(d);
        }
    }
    
    private void handleControlSequence(char c)
    {
        switch(c)
        {
            case KeyEvent.VK_BACK_SPACE:
            case KeyEvent.VK_DELETE:
                eur_.clear();
                idx_ = 0;
                break;
            case '.':
                if(idx_ != 0) break;
            case KeyEvent.VK_RIGHT:
                if(idx_ < 3) ++idx_;
                else beep();
                break;
            case KeyEvent.VK_LEFT:
                if(idx_ > 0) --idx_;
                else beep();
                break;
        }
    }
    
    private void beep()
    {
        Toolkit.getDefaultToolkit().beep();
    }

    @Override
    public void keyReleased(KeyEvent e)
    {
        e.consume();
    }

    @Override
    public void keyTyped(KeyEvent e)
    {
        e.consume();
    }
}
